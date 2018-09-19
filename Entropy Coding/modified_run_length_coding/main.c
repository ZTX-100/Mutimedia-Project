//
//  main.c
//  modified_run_length_coding
//
//  Created by Tianxiao Zhang on 9/12/17.
//  Copyright © 2017 Tianxiao Zhang. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "bitio.h"
#include "errhand.h"
#define MAX_SIZE 65536

char audio_file[] = "/Users/zhtianxiao/Desktop/audio.dat";
char binary_file[] = "/Users/zhtianxiao/Desktop/binary.dat";
char image_file[] = "/Users/zhtianxiao/Desktop/image.dat";
char text_file[] = "/Users/zhtianxiao/Desktop/text.dat";


unsigned long *rl_encoder(unsigned long *data, unsigned long len) {
    
    int rlen;
    unsigned long *encoded_data = (unsigned long *)malloc(sizeof(unsigned long)*(len*2+1));
    
    unsigned long i = 0;
    unsigned long j = 0;
    for(i=0;i<len;i++) {
        
        rlen = 1;
        while(i+1 < len && data[i] == data[i+1]) {
            rlen++;
            i++;
        }
        
        // This encoding process is more complex than basic run length coding
        // When rlen is 1, there are two cases: the data larger than 127 or not
        // That is MSB is 1 or not
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
        else {
        if (rlen > 127) {
            while(rlen > 127) {
                encoded_data[j++] = 128+127;
                encoded_data[j++] = data[i];
                rlen = rlen - 127;
            }
        }
        if (rlen <= 127 && rlen != 0) {
            encoded_data[j++] = 128+rlen;
            encoded_data[j++] = data[i];
        }
        }
    }
    
    encoded_data[j] = 256;
    return encoded_data;
}


void rl_decoder(unsigned long *data, unsigned long *coded_data, unsigned long len)
{
    
    unsigned long length;
    unsigned long i;
    unsigned long j;
    length = len;
    
    // The decoding process is the same as encoding process
    // If the element is larger than 127, it is count number
    // and it counts the element next.
    // If the element is less than 128, it is element, without count number.
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


int main(int argc, const char * argv[]) {
    
    // Binary.dat
    
    unsigned long binary_length = 0;
    unsigned long binary[MAX_SIZE];
    unsigned long decoded_binary[MAX_SIZE+1];
    double compressed_ratio = 0.0;
    BIT_FILE *binary_dat;
    binary_dat = OpenInputBitFile(binary_file);
    while(!feof(binary_dat->file)) {
        binary[binary_length] = InputBits(binary_dat, 8);
        
        binary_length++;
    }
    binary_length--; // Original file length in Byte
    
    unsigned long *encoded_binary = rl_encoder(binary, binary_length);
    
    unsigned long t = 0;
    printf("\n\n");
    printf("Encoded run length code of binary.dat is : \n");
    while (encoded_binary[t] != 256) {
        printf("%lu ",encoded_binary[t]);
        t++;
    }
    
    compressed_ratio = (double)(binary_length-t)/(double)binary_length*(double)100;
    printf("\n\n");
    printf("The length of unencoded binary.dat is %lu\n",binary_length);
    printf("The length of encoded binary.dat is %lu\n",t);
    printf("The compressed ratio of binary,dat is %f%%\n",compressed_ratio);
    
    rl_decoder(decoded_binary, encoded_binary, t);
    printf("\n\n");
    printf("Decoded run length code of binary.dat is : \n");
    
    t = 0;
    while(decoded_binary[t] != 256) {
        printf("%lu ",decoded_binary[t]);
        t++;
    }
    printf("\n\n");
    
    
    // Text.dat
    
    unsigned long text_length = 0;
    unsigned long text[MAX_SIZE];
    unsigned long decoded_text[MAX_SIZE+1];
    BIT_FILE *text_dat;
    text_dat = OpenInputBitFile(text_file);
    while(!feof(text_dat->file)) {
        text[text_length] = InputBits(text_dat, 8);
        text_length++;
    }
    text_length--;
    
    unsigned long *encoded_text = rl_encoder(text, text_length);
    
    t = 0;
    printf("\n\n");
    printf("Encoded run length code of text.dat is : \n");
    while (encoded_text[t] != 256) {
        printf("%lu ",encoded_text[t]);
        t++;
    }
    
    compressed_ratio = (double)(text_length-t)/(double)(text_length)*(double)100;
    printf("\n\n");
    printf("The length of unencoded text.dat is %lu\n",text_length);
    printf("The length of encoded text.dat is %lu\n",t);
    printf("The compressed ratio of text.dat is %f%%\n",compressed_ratio);
    
    rl_decoder(decoded_text, encoded_text, t);
    printf("\n\n");
    printf("Decoded run length code of text.dat is : \n");
    
    t = 0;
    while(decoded_text[t] != 256) {
        printf("%c",(char)decoded_text[t]);
        t++;
    }
    printf("\n\n");
    
    
    // Audio.dat
    
    unsigned long audio_length = 0;
    unsigned long audio[MAX_SIZE];
    unsigned long decoded_audio[MAX_SIZE+1];
    BIT_FILE *audio_dat;
    audio_dat = OpenInputBitFile(audio_file);
    while(!feof(audio_dat->file)) {
        audio[audio_length] = InputBits(audio_dat, 8);
        audio_length++;
    }
    audio_length--;
    
    unsigned long *encoded_audio = rl_encoder(audio, audio_length);
    
    t = 0;
    printf("\n\n");
     printf("Encoded run length code of audio.dat is : \n");
    while (encoded_audio[t] != 256) {
        printf("%lu ",encoded_audio[t]);
        t++;
    }
    
    compressed_ratio = (double)(t-audio_length)/(double)audio_length*(double)100;
    printf("\n\n");
    printf("The length of unencoded audio.dat is %lu\n",audio_length);
    printf("The length of encoded audio.dat is %lu\n",t);
    printf("The compressed ratio of audio.dat is -%f%%\n",compressed_ratio);
    
    rl_decoder(decoded_audio, encoded_audio, t);
    printf("\n\n");
    printf("Decoded run length code of audio.dat is : \n");
    
    t = 0;
    while(decoded_audio[t] != 256) {
        printf("%lu ",decoded_audio[t]);
        t++;
    }
    printf("\n\n");
    
    
    // Image.dat
    
    unsigned long image_length = 0;
    unsigned long image[MAX_SIZE];
    unsigned long decoded_image[MAX_SIZE+1];
    BIT_FILE *image_dat;
    image_dat = OpenInputBitFile(image_file);
    while(!feof(image_dat->file)) {
        image[image_length] = InputBits(image_dat, 8);
        image_length++;
    }
    image_length--;
    
    unsigned long *encoded_image = rl_encoder(image, image_length);
    
    t = 0;
    printf("\n\n");
     printf("Encoded run length code of image.dat is : \n");
    while (encoded_image[t] != 256) {
        printf("%lu ",encoded_image[t]);
        t++;
    }
    
    compressed_ratio = (double)(t-image_length)/(double)image_length*(double)100;
    printf("\n\n");
    printf("The length of unencoded image.dat is %lu\n",image_length);
    printf("The length of encoded image.dat is %lu\n",t);
    printf("The compressed ratio of image.dat is -%f%%\n",compressed_ratio);
    
    rl_decoder(decoded_image, encoded_image, t);
    printf("\n\n");
    printf("Decoded run length code of image.dat is : \n");
    
    t = 0;
    while(decoded_image[t] != 256) {
        printf("%lu ",decoded_image[t]);
        t++;
    }
    printf("\n\n");
    
    
    return 0;
}
