//
//  main.c
//  Burrows_Wheeler_Transform
//
//  Created by Tianxiao Zhang on 9/14/17.
//  Copyright © 2017 Tianxiao Zhang. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "bitio.h"
#include "errhand.h"
#define MAX_SIZE 65536
#define N 20

char text_file[] = "/Users/zhtianxiao/Desktop/text.dat";


int *rl_encoder(int *data, unsigned long len) {
    
    int rlen;
    int *encoded_data = (int *)malloc(sizeof(int)*(len*2+1));
    
    unsigned long i = 0;
    unsigned long j = 0;
    for(i=0;i<len;i++) {
        
        rlen = 1;
        while(i+1 < len && data[i] == data[i+1]) {
            rlen++;
            i++;
        }
        
        if (rlen == 1) {
            if (data[i] < 128) {
                encoded_data[j++] = data[i];
                continue;
            }
            else if (data[i] >= 128) {
                encoded_data[j++] = 128+1;
                encoded_data[j++] = data[i];
            }
        }
        
        if (rlen > 127) {
            while(rlen > 127) {
                encoded_data[j++] = 128+127;
                encoded_data[j++] = data[i];
                rlen = rlen - 127;
            }
        }
        else if (rlen <= 127 && rlen != 0) {
            encoded_data[j++] = 128+rlen;
            encoded_data[j++] = data[i];
        }
    }
    
    encoded_data[j] = 256;
    return encoded_data;
}


void rl_decoder(int *data, int *coded_data, unsigned long len)
{
    
    unsigned long length;
    unsigned long i;
    unsigned long j;
    length = len;
    
    for(i=0;i<length;i++) {
        
        if(coded_data[i] > 127) {
            for(j=0;j<(coded_data[i]-128);j++) {
                *data++ = coded_data[i+1];
            }
            i++;
        }
        
        else if (coded_data[i] < 128) {
            *data++ = coded_data[i];
        }
        
    }
    
    *data = 256;
}


// Construct the matrix for BWT
void matrix(int a[N], int b[N][N]) {
    int i = 0;
    int j = 0;
    int c[2*N];
    for (int t=0;t<N;t++) {
        c[t] = a[t];
    }
    for (int t=0;t<N;t++) {
        c[N+t] = a[t];
    }
    
    for(j=0;j<N;j++) {
        for (i=0;i<N;i++) {
            b[j][i] = c[i+j];
        }
    }
    
}

// Write "compare()" function to be used in "qsort()" function
int compare(const int *x, const int *y) {
    int result;
    result = memcmp(x,y,N);
    return result;
}

// The BWT encoded array
// First element is the index
// The other is the last column of the element
void bwt(int a[N], int c[N+1]) {
    int index = 0;
    int j = 0;
    int i = 0;
    int b[N][N];
    matrix(a, b);
    // Sort the matrix
    qsort(b,N,N*sizeof(int),(int (*)(const void *,const void *)) compare);
    
    // Get the index
    for(j=0;j<N;j++) {
        if (!compare(b[j],a)) {
            index = j;
            break;
        }
    }
    
    c[0] = index;
    // Get the last column of the matrix
    for (i=1;i<N+1;i++) {
        c[i] = b[i-1][N-1];
    }
    
}

void ibwt(int c[N+1], int a[N]) {
    
    int b[N];
    int d[N];
    for(int i=0;i<N;i++) {
        b[i] = c[i+1];
        d[i] = c[i+1];
    }
    
    // Sort the last column of the matrix to get the first colume
    qsort(b, N, sizeof(int), (int (*)(const void *,const void *)) compare);
    
    a[0] = b[c[0]];
    int temp = 0;
    for (int j=1;j<N;j++) {
        int count = 0;
        
        if(j == 1) {
            for(int i=0;i<c[0]+1;i++) {
                if(b[i] == a[0]) {
                    count++;
                }
            }
            for(int i=0;i<N;i++) {
                if (d[i] == a[0]) {
                    count--;
                    if(!count) {
                        a[j] = b[i];
                        temp = i;
                        break;
                    }
                }
            }
        }
        
        count = 0;
        if(j>1) {
            // Under the rule: if the element is the n-th of first colume
            // its corresponding element is also n-th element of the last colume
            for(int i=0;i<temp+1;i++) {
                if(b[i] == a[j-1]) {
                    count++;
                }
            }
            
            for(int i=0;i<N;i++) {
                if(d[i] == a[j-1]) {
                    count--;
                    if(!count) {
                        a[j] = b[i];
                        temp = i;
                        break;
                    }
                }
            }
        }
        
    }
    
}



int main(int argc, const char * argv[]) {
    
    unsigned long text_length = 0;
    int text[MAX_SIZE];
    int bwt_text[MAX_SIZE];
    int ibwt_text[MAX_SIZE];
    int decoded_text[MAX_SIZE+1];
    int group;
    int bwt_length = 0;
    BIT_FILE *text_dat;
    text_dat = OpenInputBitFile(text_file);
    // Read element from the file
    while(!feof(text_dat->file)) {
        text[text_length] = (int)InputBits(text_dat, 8);
        text_length++;
    }
    text_length--; // The length of original file
    

    group = (int)text_length / N;

    for(int i=0;i<group;i++) {
        bwt_length += N+1;
        bwt(text+(N*i), bwt_text+((N+1)*i));
        }
    
    // If the number of the left elements is less than 20
    if(text_length % N != 0) {
        for (int i=0;i<text_length-(group*N);i++) {
            bwt_text[(N+1)*group+i] = text[N*group+i];
        }
        bwt_length += text_length-(group*N);
    }
    
    int *encoded_text = rl_encoder(bwt_text, bwt_length);
    
    unsigned long t = 0;
    printf("\n\n");
    printf("Encoded run length code of text.dat after BWT is : \n");
    while (encoded_text[t] != 256) {
        printf("%d ",encoded_text[t]);
        t++;
    }
    printf("\n\n");
    
    printf("The length of unBWT text.dat is %lu\n",text_length);
    printf("The length of BWT text.dat is %lu",t);
    
    rl_decoder(decoded_text, encoded_text, t);
    printf("\n\n");
    printf("Decoded run length code of text.dat is : \n");
    
    for(int j=0;j<group;j++) {
        ibwt(decoded_text+((N+1)*j), ibwt_text+N*j);
    }
    // Deal with the left elements whose number is less than 20
    if (text_length % N != 0) {
        for(int j=0;j<text_length-(group*N);j++) {
            ibwt_text[N*group+j] = decoded_text[(N+1)*group+j];
        }
    }
    
    printf("\n");
    for(int i=0;i<text_length;i++) {
        printf("%c", ibwt_text[i]);
    }
    printf("\n\n");
    
    return 0;
}
