//
//  main.c
//  run_length_coding
//
//  Created by Tianxiao Zhang on 9/11/17.
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

// The function for run length encoding
unsigned long *rl_encoder(unsigned long *data, unsigned long len) {
    
    int rlen;
    // Malloc the worst case of length
    unsigned long *encoded_data = (unsigned long *)malloc(sizeof(unsigned long)*(len*2+1));
    
    unsigned long i = 0;
    unsigned long j = 0;
    for(i=0;i<len;i++) {

        rlen = 1;
        while(i+1 < len && data[i] == data[i+1]) {
            rlen++;
            i++;
        }
        
        // If seccessive repetition is larger larger than 255
        // I count the element again as a new element after 255
        if (rlen > 255) {
        while(rlen > 255) {
        encoded_data[j++] = 255;
        encoded_data[j++] = data[i];
        rlen = rlen - 255;
        }
        }
        if (rlen <= 255 && rlen != 0) {
        encoded_data[j++] = rlen;
        encoded_data[j++] = data[i];
        }
    }
    
    encoded_data[j] = 256;
    return encoded_data;
}


void rl_decoder(unsigned long *data, unsigned long *coded_data, unsigned long len)
{
 
    unsigned long length;
    unsigned long i;
    unsigned long c;
    // The decoding process is easy
    // one count number, one element, one count number, one element...
    for(length = len ; length>0 ; length -= 2 ) {
        i = *coded_data++;
        c = *coded_data++;
        while(i-- > 0)
            *data++ = c;
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
    binary_length--;
    
    unsigned long *encoded_binary = rl_encoder(binary, binary_length);
    
    unsigned long t = 0;
    printf("\n\n");
    printf("Encoded run length code of binary.dat is : \n");
    // If encoded file does not end
    while (encoded_binary[t] != 256) {
        printf("%lu ",encoded_binary[t]);
        t++;
    }
    
    compressed_ratio = (double)(binary_length-t)/(double)binary_length*(double)100;
    
    printf("\n\n");
    printf("The length of unencoded binary.dat is %lu\n",binary_length);
    printf("The length of encoded binary.dat is %lu\n",t);
    printf("The compressed ratio of binary.dat is %f%%\n",compressed_ratio);
    
    rl_decoder(decoded_binary, encoded_binary, t);
    printf("\n\n");
    printf("Decoded run length code of binary.dat is : \n");
    
    t = 0;
    // If decoded file does not end
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
    
    compressed_ratio = (double)(t-text_length)/(double)text_length*(double)100;
    printf("\n\n");
    printf("THe length of unencoded text.dat is %lu\n",text_length);
    printf("The length of encoded text.dat is %lu\n",t);
    printf("The compressed ratio of text.dat is -%f%%\n",compressed_ratio);

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
    printf("THe length of unencoded audio.dat is %lu\n",audio_length);
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
    printf("THe length of unencoded image.dat is %lu\n",image_length);
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
