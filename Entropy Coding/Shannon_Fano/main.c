//
//  main.c
//  Shannon_Fano
//
//  Created by Tianxiao Zhang on 9/8/17.
//  Copyright © 2017 Tianxiao Zhang. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "bitio.h"
#include "errhand.h"

char audio_file[] = "/Users/zhtianxiao/Desktop/audio.dat";
char binary_file[] = "/Users/zhtianxiao/Desktop/binary.dat";
char image_file[] = "/Users/zhtianxiao/Desktop/image.dat";
char text_file[] = "/Users/zhtianxiao/Desktop/text.dat";

//***************************  shannnon_fano  ******************************//
//INPUT: the first value position, the last value position, value frequency //
//OUTPUT: shannon_fano code relative to the value frequency                 //
//
void shannon_fano(int first, int final, int floor, int element[], char encoder[][256]) {
    
    int start;
    int end;
    long start_sum;
    long end_sum;
    start = first;  // The start position of value;
    end = final;    // The end position of value;
    start_sum = element[start];  // Add value frequency from the start value;
    end_sum = element[end];   // Add value frequency from the end value;
    
    while(start < (end-1)) {  // Find nearly 50%--50% point;
        if(start_sum >= end_sum) {
            end_sum += element[--end];
        }
        if(start_sum < end_sum) {
            start_sum += element[++start];
        }
    }
    
     if (start == first) {    // The first 50% part is encoded as 0;
        encoder[first][floor] = '0';
    }
    else if ((start-first) > 0) {
        for (int t=first;t<=start;t++) {
            encoder[t][floor] = '0';
        }
        
    // Do shannon_fano for the first 50% part again;
    shannon_fano(first, start, floor+1, element, encoder);
    }

      if (end == final) {   // The second 50% part is encoded as 1;
        encoder[final][floor] = '1';
    }
    else if ((final-end) > 0) {
        for(int t=end;t<=final;t++) {
            encoder[t][floor] = '1';
        }
        
    // Do shannon_fano for the second 50% part again;
    shannon_fano(end, final, floor+1, element, encoder);
    }
      
}


int main(int argc, const char * argv[]) {
    
    // BINARY DATA
    int temp;
    int temper;
    unsigned long bits_length = 0;
    unsigned long file_length = 0;
    double compressed_ratio = 0.0;
    unsigned long num_binary;
    int buff_binary[256] = {0};
    int order_binary[256];
    int code_num_binary[256] = {0};
    double code_length_binary = 0.0;
    long binary_length = 0;
    double pr_binary;
    double binary_entropy = 0;
    
    printf("Shannon_Fano coding result----------------------------------\n");
    
    BIT_FILE *binary_dat;
    binary_dat = OpenInputBitFile(binary_file);
    
    while(!feof(binary_dat->file)) {
        num_binary = InputBits(binary_dat, 8);  // Read from binary file 1 byte each time;
        buff_binary[num_binary]++;  // Count value frequency;
        binary_length++;  // Count file length in byte;
    }
    binary_length--;
    
    for (int t=0;t<256;t++) {
        order_binary[t] = t;
    }
    
    // We sort the value frequency in descending order, we also sort
    // the value corresponding to value frequency;
    for (int i=0;i<256;i++)
    {
        for (int j=0;j<(256-i-1);j++)
        {
            if (buff_binary[j] < buff_binary[j+1])
            {
                temp = buff_binary[j];
                buff_binary[j] = buff_binary[j+1];
                buff_binary[j+1] = temp;
                
                temper = order_binary[j];
                order_binary[j] = order_binary[j+1];
                order_binary[j+1] = temper;
            }
        }
    }
    
    // Initialize the encoder table;
    char encoder_binary[256][256];
    for(int i=0;i<256;i++) {
        for(int j=0;j<256;j++) {
            encoder_binary[i][j] = 'N';
        }
    }
 
    shannon_fano(0, 255, 0, buff_binary, encoder_binary);
    
    printf("\n");
    printf("Shannon_Fano codeword of binary.dat :\n");
    printf("\n");
    printf("     values      frequency         code\n");
    for (int k=0;k<256;k++) {
        if (buff_binary[k] != 0) {
            printf("      %3d         %f           ",order_binary[k],(double)buff_binary[k]/(double)binary_length);  // Output values and corresponding frequency;
            for (int t=0;t<256;t++) {
                if (encoder_binary[k][t] != 'N') {
                    printf("%c",encoder_binary[k][t]); // Output codeword;
                    code_num_binary[k]++;  // Calculate each codeword length in bit
                }
            }
            printf("\n");
            pr_binary = (double)buff_binary[k] / (double)binary_length;
            binary_entropy += -(pr_binary*log2(pr_binary)); // Calculate entropy;
        }
    }
    
    // The total bits used after Shannon_Fano coding
    for (int k=0;k<256;k++) {
         bits_length += code_num_binary[k] * buff_binary[k];
    }
    
    // Calculate average codeword length used by Shannon_Fano
    code_length_binary = (double)bits_length / (double)binary_length;
    
    // The total file length in byte after Shannon_Fano coding
    if(bits_length % 8 == 0) {
        
        file_length = bits_length/8;
    }
    else {
        file_length = bits_length/8 + 1;
    }
    
    compressed_ratio = (double)(binary_length-file_length)/(double)binary_length*(double)100;
    
    printf("\n");
    printf("The size of the compressed file binary.dat is %lu\n",file_length);
    printf("THe compressed ratio of the input file binary.dat is %f%%\n",compressed_ratio);
    printf("The size of the input file binary.dat is %ld Bytes\n",binary_length);
    printf("The entropy of the input file binary.dat is %f\n",binary_entropy);
    printf("Shannon_Fano coding average codeword length is %f\n",code_length_binary);
    printf("\n\n");

    
    // TEXT DATA
    
    int buff_text[128] = {0};
    int order_text[128];
    int code_num_text[128] = {0};
    double code_length_text = 0.0;
    int k;
    unsigned long ch;
    long text_length = 0;
    double text_entropy = 0.0;
    double pr_text;
    BIT_FILE *text_dat;
    text_dat = OpenInputBitFile(text_file);
    while(!feof(text_dat->file)) {
        
        ch = InputBits(text_dat, 8);
        buff_text[ch]++;
        text_length++;

    }
    text_length--;
    
    for (int t=0;t<128;t++) {
        order_text[t] = t;
    }
    
    for (int i=0;i<128;i++)
    {
        for (int j=0;j<(128-i-1);j++)
        {
            if (buff_text[j] < buff_text[j+1])
            {
                temp = buff_text[j];
                buff_text[j] = buff_text[j+1];
                buff_text[j+1] = temp;
                
                temper = order_text[j];
                order_text[j] = order_text[j+1];
                order_text[j+1] = temper;
            }
        }
    }
    
    char encoder_text[128][256];
    for (int i=0;i<128;i++) {
        for (int j=0;j<256;j++) {
            encoder_text[i][j] = 'N';
        }
    }
    
    shannon_fano(0, 127, 0, buff_text, encoder_text);
    
    printf("\n");
    printf("Shannon_Fano codeword of text.dat :\n");
    printf("\n");
    printf("characters             frequency            code\n");
    for(k=0;k<128;k++) {
        if (buff_text[k] != 0) {
            printf("    %c                  %f             ",order_text[k],(double)buff_text[k]/(double)text_length);
            for (int t=0;t<256;t++) {
                if (encoder_text[k][t] != 'N'){
                    printf("%c",encoder_text[k][t]);
                    code_num_text[k]++;
                }
            }
            printf("\n");
            pr_text = (double)buff_text[k] / (double)text_length;
            text_entropy += -(pr_text*log2(pr_text));
        }
    }
    
    bits_length = 0;
    file_length = 0;
    compressed_ratio = 0.0;
    for (int k=0;k<128;k++) {
        bits_length += code_num_text[k] * buff_text[k];
    }
    
    // Calculate average codeword length used by Shannon_Fano
    code_length_text = (double)bits_length / (double)text_length;
    
    if(bits_length % 8 == 0) {
        
        file_length = bits_length/8;
    }
    else {
        file_length = bits_length/8 + 1;
    }
    
    compressed_ratio = (double)(text_length-file_length)/(double)text_length*(double)100;
    
    printf("\n");
    printf("The size of the compressed file text.dat is %lu\n",file_length);
    printf("THe compressed ratio of the input file text.dat is %f%%\n",compressed_ratio);
    printf("The size of the input file text.dat is %ld Bytes\n",text_length);
    printf("The entropy of the input file text.dat is %f\n",text_entropy);
    printf("Shannon_Fano coding average codeword length is %f\n",code_length_text);
    printf("\n\n");
    
    
    // AUDIO DATA
    
    int buff_audio[256] = {0};
    int order_audio[256];
    int code_num_audio[256] = {0};
    double code_length_audio = 0.0;
    long audio_length = 0;
    double pr_audio;
    double audio_entropy = 0.0;
    unsigned long num_audio;
    
    BIT_FILE *audio_dat;
    audio_dat = OpenInputBitFile(audio_file);
    
    while(!feof(audio_dat->file)) {
        num_audio = InputBits(audio_dat, 8);
        buff_audio[num_audio]++;
        audio_length++;
    }
    audio_length--;
    
    for (int t=0;t<256;t++) {
        order_audio[t] = t;
    }
    
    for (int i=0;i<256;i++)
    {
        for (int j=0;j<(256-i-1);j++)
        {
            if (buff_audio[j] < buff_audio[j+1])
            {
                temp = buff_audio[j];
                buff_audio[j] = buff_audio[j+1];
                buff_audio[j+1] = temp;
                
                temper = order_audio[j];
                order_audio[j] = order_audio[j+1];
                order_audio[j+1] = temper;
            }
        }
    }
    
    char encoder_audio[256][256];
    for(int i=0;i<256;i++) {
        for(int j=0;j<256;j++) {
            encoder_audio[i][j] = 'N';
        }
    }
    
    shannon_fano(0, 255, 0, buff_audio, encoder_audio);
    
    printf("\n");
    printf("Shannon_Fano codeword of audio.dat :\n");
    printf("\n");
    printf("     values       frequency           code\n");
    for (int k=0;k<256;k++) {
        if (buff_audio[k] != 0) {
            printf("      %3d         %f           ",order_audio[k],(double)buff_audio[k]/(double)audio_length);
            for (int t=0;t<256;t++) {
                if (encoder_audio[k][t] != 'N') {
                    printf("%c",encoder_audio[k][t]);
                    code_num_audio[k]++;
                }
            }
            printf("\n");
            pr_audio = (double)buff_audio[k] / (double)audio_length;
            audio_entropy += -(pr_audio*log2(pr_audio));
        }
    }
    
    bits_length = 0;
    file_length = 0;
    compressed_ratio = 0.0;
    for (int k=0;k<256;k++) {
        bits_length += code_num_audio[k] * buff_audio[k];
    }
    
    // Calculate average codeword length used by Shannon_Fano
    code_length_audio = (double)bits_length / (double)audio_length;
    
    if(bits_length % 8 == 0) {
        
        file_length = bits_length/8;
    }
    else {
        file_length = bits_length/8 + 1;
    }
    
    compressed_ratio = (double)(audio_length-file_length)/(double)audio_length*(double)100;
    
    printf("\n");
    printf("The size of the compressed file audio.dat is %lu\n",file_length);
    printf("THe compressed ratio of the input file audio.dat is %f%%\n",compressed_ratio);
    printf("The size of the input file audio.dat is %ld\n",audio_length);
    printf("The entropy of the input file audio.dat is %f\n",audio_entropy);
    printf("Shannon_Fano coding average codeword length is %f\n",code_length_audio);
    printf("\n\n");
    
    
    // IMAGE DATA
    
    int buff_image[256] = {0};
    int order_image[256];
    int code_num_image[256] = {0};
    double code_length_image = 0.0;
    long image_length = 0;
    double pr_image;
    double image_entropy = 0.0;
    unsigned long num_image;
    BIT_FILE *image_dat;
    image_dat = OpenInputBitFile(image_file);
    
    while(!feof(image_dat->file)) {
        num_image = InputBits(image_dat, 8);
        buff_image[num_image]++;
        image_length++;
    }
    image_length--;
    
    for(int t=0;t<256;t++) {
        order_image[t] = t;
    }
    
    for (int i=0;i<256;i++)
    {
        for (int j=0;j<(256-i-1);j++)
        {
            if (buff_image[j] < buff_image[j+1])
            {
                temp = buff_image[j];
                buff_image[j] = buff_image[j+1];
                buff_image[j+1] = temp;
                
                temper = order_image[j];
                order_image[j] = order_image[j+1];
                order_image[j+1] = temper;
            }
        }
    }

    char encoder_image[256][256];
    for(int i=0;i<256;i++) {
        for(int j=0;j<256;j++) {
            encoder_image[i][j] = 'N';
        }
    }
    
    shannon_fano(0, 255, 0, buff_image, encoder_image);
    
    printf("\n");
    printf("Shannon_Fano codeword of image.dat :\n");
    printf("\n");
    printf("     values         frequency           code\n");
    for (int k=0;k<256;k++) {
        if (buff_image[k] != 0) {
            printf("      %3d           %f           ",order_image[k],(double)buff_image[k]/(double)image_length);
            for (int t=0;t<256;t++) {
                if (encoder_image[k][t] != 'N') {
                    printf("%c",encoder_image[k][t]);
                    code_num_image[k]++;
                }
            }
            printf("\n");
            pr_image = (double)buff_image[k] / (double)image_length;
            image_entropy += -(pr_image*log2(pr_image));
        }
    }
    
    bits_length = 0;
    file_length = 0;
    compressed_ratio = 0.0;
    for (int k=0;k<256;k++) {
        bits_length += code_num_image[k] * buff_image[k];
    }
    
    // Calculate average codeword length used by Shannon_Fano
    code_length_image = (double)bits_length / (double)image_length;
    
    if(bits_length % 8 == 0) {
        
        file_length = bits_length/8;
    }
    else {
        file_length = bits_length/8 + 1;
    }
    
    compressed_ratio = (double)(file_length-image_length)/(double)image_length*(double)100;
    
    printf("\n");
    printf("The size of the compressed file text.dat is %lu\n",file_length);
    printf("THe compressed ratio of the input file image.dat is -%f%%\n",compressed_ratio);
    printf("The size of the input file image.dat is %ld\n",image_length);
    printf("The entropy of the input file image.dat is %f\n",image_entropy);
    printf("Shannon_Fano coding average codeword length is %f\n",code_length_image);
    printf("\n\n");
    
    
    return 0;
}
