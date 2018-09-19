//
//  main.c
//  QMCoder(pre_processing_image)
//
//  Created by Tianxiao Zhang on 9/26/17.
//  Copyright © 2017 Tianxiao Zhang. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "bitio.h"
#include "errhand.h"

#define QMputc(BP, m_File) if(bFirst) {fputc(BP, m_File);} else {bFirst = 1;};
#define SIZE 256

int lsz[46] = {
    0x59EB, 0x5522, 0x504F, 0x4B85,
    0x4639, 0x415E, 0x3C3D, 0x375E,
    0x32b4, 0x2E17, 0x299A, 0x2516,
    0x1EDF, 0x1AA9, 0x174E, 0x1424,
    0x119C, 0x0F6B, 0x0D51, 0x0BB6,
    0x0A40, 0x0861, 0x0706, 0x05CD,
    0x04DE, 0x040F, 0x0363, 0x02D4,
    0x025C, 0x01F8, 0x01A4, 0x0160,
    0x0125, 0x00F6, 0x00CB, 0x00AB,
    0x008F, 0x0068, 0x004E, 0x003B,
    0x002C, 0x001A, 0x000D, 0x0006,
    0x0003, 0x0001
};

int nmps[46] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38,
    39, 40, 41, 42, 43, 44, 45, 45
};

int nlps[46] = {
    -1, 0, 1, 2, 3, 4, 5, 6, 6, 8, 8,
    10, 11, 11, 13, 13, 15, 15, 16, 18,
    18, 19, 20, 21, 23, 23, 24, 25, 26,
    27, 28, 29, 30, 31, 32, 34, 34, 35,
    36, 37, 38, 38, 40, 41, 42, 44
};


char image_file[] = "/Users/zhtianxiao/Desktop/image.dat";

char image_QMcoded_file_1[] = "/Users/zhtianxiao/Desktop/QMcoded_image_1.dat";
char image_QMcoded_file_2[] = "/Users/zhtianxiao/Desktop/QMcoded_image_2.dat";
char image_QMcoded_file_3[] = "/Users/zhtianxiao/Desktop/QMcoded_image_3.dat";
char image_QMcoded_file_4[] = "/Users/zhtianxiao/Desktop/QMcoded_image_4.dat";

FILE *m_File;
unsigned long  C_register;
unsigned long  A_interval;
unsigned       ct;
unsigned long  sc;
unsigned       Qe;
unsigned char  BP;
unsigned       Cx;
int            count;
int            MPS;
int            next_MPS;
int            next_st;
int            cur_st;
int            bFirst;
int            max_context;
unsigned char  *st_table;
unsigned char  *mps_table;


void QM(FILE *FP)
{
    m_File = FP;
    max_context = 256;
    st_table = (unsigned char *) calloc(max_context, sizeof(unsigned char));
    mps_table= (unsigned char *) calloc(max_context, sizeof(unsigned char));
}



void reset()
{
    for (int i = 0; i < max_context; i++)
    {
        st_table[i] = 0;
        mps_table[i] = 0;
    }
}



void Initialize() {
    
    sc = 0;
    A_interval = 0x10000;
    C_register = 0;
    ct = 11;
    count = -1;
    bFirst = 0;
    BP = 0;
}



void Stuff_0() {
    
    if(BP == 0xFF) {
        QMputc(BP, m_File);
        count++;
        BP = 0;
    }
}



void Output_stacked_zeros() {
    
    while(sc > 0) {
        QMputc(BP, m_File);
        count++;
        BP = 0;
        sc--;
    }
}



void Output_stacked_0xffs() {
    
    while(sc > 0) {
        QMputc(BP, m_File);
        count++;
        BP = 0xFF;
        QMputc(BP, m_File);
        count++;
        BP = 0;
        sc--;
    }
}



void Byte_out() {
    
    unsigned long t = C_register >> 19;
    
    if(t > 0xFF) {
        BP++;
        Stuff_0();
        Output_stacked_zeros();
        QMputc(BP, m_File);
        count++;
        BP = t;
    }
    else {
        if(t == 0xFF) {
            sc++;
        }
        else {
            Output_stacked_0xffs();
            QMputc(BP, m_File);
            count++;
            BP = t;
        }
    }
    C_register &= 0x7FFFF;
}



void renormalize() {
    
    do {
        A_interval <<= 1;
        C_register <<= 1;
        ct--;
        if(ct == 0) {
            Byte_out();
            ct = 8;
        }
    } while(A_interval < 0x8000);
}



void Code_MPS() {
    
    A_interval -= Qe;
    
    if(A_interval < 0x8000) {
        
        if(A_interval < Qe) {
            C_register += A_interval;
            A_interval = Qe;
        }
        
        next_st = nmps[cur_st];
        renormalize();
        
    }
}



void Code_LPS() {
    
    A_interval -= Qe;
    
    if(A_interval >= Qe) {
        
        C_register += A_interval;
        A_interval = Qe;
    }
    
    if(nlps[cur_st] == -1) {
        
        next_MPS = 1 - MPS;
        next_st = 0;
    }
    else if (nlps[cur_st] != -1) {
        next_st = nlps[cur_st];
    }
    renormalize();
    
}



void Clear_final_bits() {
    
    unsigned long t;
    t = C_register + A_interval - 1;
    t &= 0xFFFF0000;
    
    if(t < C_register)
        t += 0x8000;
    
    C_register = t;
}



void Flush() {
    
    Clear_final_bits();
    C_register <<= ct;
    Byte_out();
    C_register <<= 8;
    Byte_out();
    QMputc(BP, m_File);
    count++;
    QMputc(0xFF, m_File);
    count++;
    QMputc(0xFF, m_File);
    count++;
}



void encode(int symbol, int context)
{
    
    if (context >= max_context)
    {
        unsigned char *new_st, *new_mps;
        new_st = (unsigned char *) calloc(max_context*2, sizeof(unsigned char));
        new_mps= (unsigned char *) calloc(max_context*2, sizeof(unsigned char));
        memcpy(new_st, st_table, max_context*sizeof(unsigned char));
        memcpy(new_mps,mps_table,max_context*sizeof(unsigned char));
        max_context *= 2;
        free(st_table);
        free(mps_table);
        st_table = new_st;
        mps_table = new_mps;
    }
    
    next_st = cur_st = st_table[context];
    next_MPS = MPS = mps_table[context];
    Qe  = lsz[st_table[context]];
    
    if (MPS == symbol)
        Code_MPS();
    else
        Code_LPS();
    
    st_table[context] = next_st;
    mps_table[context] = next_MPS;
    
}



int Counting()
{
    if (ct == 0)
    {
        return(count*8);
    }
    else
    {
        return(count*8+(8-ct));
    }
}


/* We scan the image with traditional zigzag and store the scanned element to B */
void zigzag_1(int A[SIZE][SIZE], int B[SIZE*SIZE]){
    int i=0;
    int j=0;
    int x;
    int y;
    int size = SIZE;
    int prevrd = 0;
    int prevsn = 1;
    int k = 1;
    int done = -1;
    int num = 0;
    
    while(size > 1){
        
        k = 1;
        
        while(1){
            
            if(prevrd == 0){
                x = i;
                y = j;
                B[num++] = A[x][y];
                j = j + 1;
            }
            
            else {
                x = i;
                y = j;
                B[num++] = A[x][y];
                i = i + 1;
            }
            prevrd = !(prevrd);
            
            k++;
            
            if(k == size){
                done += 1;
                break;
            }
            
            if(prevsn == 1){
                x = i;
                y = j;
                while((y!=0) && (x!=size-1)){
                    B[num++] = A[x][y];
                    x++;
                    y--;
                }
                i = x;
                j = y;
            }
            
            else {
                x = i;
                y = j;
                while((x!=0) && (y!=size-1)){
                    B[num++] = A[x][y];
                    x--;
                    y++;
                }
                i = x;
                j = y;
            }
            prevsn = !(prevsn);
        }
        
        if(done){
            break;
        }
        
        if(prevsn == 1){
            x = i;
            y = j;
            
            while((y!=0) && (x!=size-1)){
                B[num++] = A[x][y];
                x++;
                y--;
            }
            i = x;
            j = y;
        }
        else {
            x = i;
            y = j;
            
            while((x!=0) && (y!=size-1)){
                B[num++] = A[x][y];
                x--;
                y++;
            }
            i = x;
            j = y;
        }
        prevsn = !(prevsn);
        prevrd = !prevrd;
    }
    
    B[num++] = A[i][j];
}


/* Scan the image with one line, when we scan to the end of the row,
 we do not scan the first element of the next row, but scan the last
 element of the next row, then to the beginning of the next row */
void zigzag_2(int A[SIZE][SIZE], int B[SIZE*SIZE]) {
    
    int size = SIZE;
    for(int i=0;i<size;i++) {
        
        if(i % 2 == 0) {
            for(int m=0;m<size;m++) {
                B[i*size+m] = A[i][m];
            }
        }
        else {
            for(int n=0;n<size;n++) {
                B[i*size+n] = A[i][size-n-1];
            }
        }
    }
}



/* Calculate transpose of the image matrix */
void inverse(int A[SIZE][SIZE], int B[SIZE][SIZE]) {
    int size = SIZE;
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            B[i][j] = A[j][i];
        }
    }
}



int main(int argc, const char * argv[]) {
    /*********************************************************************************/
    /************************** Beginning Image Data *********************************/
    /*********************************************************************************/
    
    
    /**********************************************/
    /********* Image zigzag pre-processing ********/
    /**********************************************/
    int size = SIZE;
    
    BIT_FILE *image_dat;
    image_dat = OpenInputBitFile(image_file);
    
    int image[size][size];
    int inverse_image[size][size];
    int zigzag_image_1[size*size];
    int zigzag_image_2[size*size];
    int isinverse;
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            image[i][j] = (int)InputBits(image_dat,8);
        }
    }

    /* If we set isinverse = 1, we use the transpose of the image,
     if we set isinverse = 0, we use original image */
    isinverse = 1;
    if(isinverse == 0) {
    zigzag_1(image,zigzag_image_1);
    zigzag_2(image,zigzag_image_2);
    }
    else if(isinverse == 1) {
    inverse(image, inverse_image);
    zigzag_1(inverse_image, zigzag_image_1);
    zigzag_2(inverse_image, zigzag_image_2);
    }
    
    /**********************************************/
    /********** Image bit plane coding ************/
    /**********************************************/
    int bitplane_1[8][size*size];
    int bitplane_2[8][size*size];
    int mask;
    int c;
    for(int i=0;i<size*size;i++) {
        
        mask = 0x80;
        c = 0;
        while(mask != 0) {
            
            bitplane_1[c][i] = (mask & zigzag_image_1[i]) ? 1 : 0;
            bitplane_2[c][i] = (mask & zigzag_image_2[i]) ? 1 : 0;
            mask >>= 1;
            c++;
        }
        
    }

    
    /*******************************************************************************/
    /**************************** Image QM coding **********************************/
    /*******************************************************************************/
    int n = 3;
    int buffer[n];
    int context = 0;
    double compressed_ratio;
    
    /****************************************/
    /********** zigzag 1 QM coding **********/
    /****************************************/
    BIT_FILE *image_qm_dat_1;
    image_qm_dat_1 = OpenOutputBitFile(image_QMcoded_file_1);
    
    for (int t=0;t<n;t++) {
        buffer[t] = 0;
    }
    
    QM(image_qm_dat_1->file);
    reset();
    Initialize();
    for(int i=0;i<8;i++) {
        
        for(int j=0;j<size*size;j++) {
            encode(bitplane_1[i][j], context);
            
            /* Calcuiate the context */
            for(int p=0;p<n-1;p++) {
                buffer[p] = buffer[p+1];
            }
            
            buffer[n-1] = bitplane_1[i][j];
            context = 0;
            
            for(int q=0;q<n;q++) {
                context += (int)exp2(q)*buffer[q];
            }
            
        }
    }
    Flush();
    
    compressed_ratio = ((double)(size*size*8 - Counting()) / (double)(size*size*8))*(double)100;
    printf("\n\n");
    printf("The results of image.dat for zigzag_1 : \n");
    printf("The QM coded bit number using bit plane coding with context rule n = %d is %d\n",n,Counting());
    printf("The QM coded comressed ratio using bit plane coding with context rule n = %d is %f%%\n",n,compressed_ratio);
    printf("\n\n");
    
    
    
    /****************************************/
    /********** zigzag 2 QM coding **********/
    /****************************************/
    context = 0;
    
    BIT_FILE *image_qm_dat_2;
    image_qm_dat_2 = OpenOutputBitFile(image_QMcoded_file_2);
    
    for (int t=0;t<n;t++) {
        buffer[t] = 0;
    }
    
    QM(image_qm_dat_2->file);
    reset();
    Initialize();
    for(int i=0;i<8;i++) {
        
        for(int j=0;j<size*size;j++) {
            encode(bitplane_2[i][j], context);
            
             /* Calcuiate the context */
            for(int p=0;p<n-1;p++) {
                buffer[p] = buffer[p+1];
            }
            
            buffer[n-1] = bitplane_1[i][j];
            context = 0;
            
            for(int q=0;q<n;q++) {
                context += (int)exp2(q)*buffer[q];
            }
            
        }
    }
    Flush();
    
    compressed_ratio = ((double)(size*size*8 - Counting()) / (double)(size*size*8))*(double)100;
    printf("\n\n");
    printf("The results of image.dat for zigzag_2 : \n");
    printf("The QM coded bit number using bit plane coding with context rule n = %d is %d\n",n,Counting());
    printf("The QM coded comressed ratio using bit plane coding with context rule n = %d is %f%%\n",n,compressed_ratio);
    printf("\n\n");
    
    
    /*******************************************************************************/
    /*************************** Ending Image Data *********************************/
    /*******************************************************************************/
    
    return 0;
}



