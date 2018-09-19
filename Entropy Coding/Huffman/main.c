//
//  main.c
//  Huffman
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


char audio_file[] = "/Users/zhtianxiao/Desktop/audio.dat";
char binary_file[] = "/Users/zhtianxiao/Desktop/binary.dat";
char image_file[] = "/Users/zhtianxiao/Desktop/image.dat";
char text_file[] = "/Users/zhtianxiao/Desktop/text.dat";



typedef struct node {
    int element;
    int frequency;
    struct node *left;
    struct node *right;
}Node;


// Find the smallest frequency of the element
int smallest(Node *buff[], int diff, int size) {
    
    int smallest;
    int temp = 0;
    
   while(buff[temp]->frequency == -1)
        temp++;
    smallest = temp;
    
    if (temp == diff) {
        temp++;
        while (buff[temp]->frequency == -1)
            temp++;
        smallest = temp;
    }
    
    
    for (temp = 0;temp<size;temp++) {
        if (buff[temp]->frequency == -1) {
            continue;
        }
        if (temp == diff) {
            continue;
        }
        if (buff[temp]->frequency < buff[smallest]->frequency) {
            smallest = temp;
        }
        
    }
    return(smallest);
}

int bits_count[256];
int *codetable(Node *tree, char codeword[], int root, int sign){
    if (tree->element != 256) {  // Find leaves
        if(sign == 1)
        printf(" %3d  :  ",tree->element);
        else if(sign == 0)
            printf("%c : ",tree->element);
        for (int i=0;i<root;i++) {
            printf("%c", codeword[i]);  // Output codeword
        }
        bits_count[tree->element] = root;
        printf("\n");
    }
    else{
        codeword[root] = '0';
        codetable(tree->right, codeword, root+1,sign);
        
        codeword[root] = '1';
        codetable(tree->left, codeword, root+1,sign);
    }
    return (bits_count);
    
}


void Huffmantree(Node **tree, int buffer[], int order[], int size) {
    Node *buff[size];
    Node *temp;
    int i,j;
    int smallest_1 = 0;
    int smallest_2 = 0;
    
    for (i=0;i<size;i++) {
        buff[i] = (Node *)malloc(sizeof(Node));
        buff[i]->frequency = buffer[i];
        buff[i]->element = order[i];
        buff[i]->left = NULL;
        buff[i]->right = NULL;
    }
    
    // Merge two smallest frequencies
    for (j=0;j<size-1;j++) {
        smallest_1 = smallest(buff,-1,size);
        smallest_2 = smallest(buff,smallest_1,size);
        temp = buff[smallest_1];
        buff[smallest_1] = (Node *)malloc(sizeof(Node));
        buff[smallest_1]->frequency = temp->frequency + buff[smallest_2]->frequency;
        buff[smallest_1]->element = 256;
        buff[smallest_1]->left = buff[smallest_2];
        buff[smallest_1]->right = temp;
        buff[smallest_2]->frequency = -1;
    }
    
    *tree = buff[smallest_1];
    
}





int main(int argc, const char * argv[]) {
    
    // BINARY DATA
    Node *tree_binary;
    char code_binary[256];
    int count_binary = 0;
    int root_binary = 0;
    double average_binary = 0.0;
    double pr_binary = 0.0;
    double binary_entropy = 0.0;
    int temp;
    int temper;
    unsigned long num_binary;
    int buff_binary[256] = {0};
    int order_binary[256];
    long binary_length = 0;
    unsigned long bits_length = 0;
    unsigned long file_length = 0;
    double compressed_ratio = 0.0;
    
    printf("Huffman Coding result-----------------------------\n");
    
    BIT_FILE *binary_dat;
    binary_dat = OpenInputBitFile(binary_file);
    
    // If the file does not end, we continue reading data from the file;
    while(!feof(binary_dat->file)) {
        num_binary = InputBits(binary_dat, 8); // Read data from the file 1 Byte each time;
        buff_binary[num_binary]++;
        binary_length++;
    }
    binary_length--;
    
    for (int t=0;t<256;t++) {
        order_binary[t] = t;
    }
    
    // Sort the frequency in desceding and also their corresponding element;
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
    
    // Count element numbers;
    for (int i=0;i<256;i++) {
        if(buff_binary[i] != 0) {
            count_binary++;
        }
    }
    
    // Put the elements and their frequencies into new arraies;
    int freq_binary[count_binary];
    int data_binary[count_binary];
    for (int j=0;j<count_binary;j++) {
        freq_binary[j] = buff_binary[j];
        data_binary[j] = order_binary[j];
    }
    int *bits_binary;
    printf("\n\n");
    printf("Huffman code for binary.dat : \n");
    printf("value     code\n");
    Huffmantree(&tree_binary, freq_binary, data_binary, count_binary);
    bits_binary = codetable(tree_binary, code_binary, root_binary,1);
    
    bits_length = 0;
    file_length = 0;
    compressed_ratio = 0.0;
    for(int t=0;t<count_binary;t++) {
        bits_length += (bits_binary[data_binary[t]]*freq_binary[t]);
        
    }
    average_binary = (double)bits_length/(double)binary_length;
    
    // Compute the compressed file length in Byte;
    if(bits_length % 8 == 0)
        file_length = bits_length/8;
    else
        file_length = bits_length/8 + 1;
    
    for(int k=0;k<count_binary;k++) {
        pr_binary = (double)buff_binary[k] / (double)binary_length;
        binary_entropy += -(pr_binary*log2(pr_binary)); // Calculate entropy;
    }
    
    // Compute compressed ratio;
    compressed_ratio = (double)(binary_length-file_length)/(double)binary_length*(double)100;
    printf("\n");
    printf("The size of the compressed file binary.dat is %lu\n",file_length);
    printf("THe compressed ratio of the input file binary.dat is %f%%\n",compressed_ratio);
    printf("The size of the input file binary.dat is %ld Bytes\n",binary_length);
    printf("The entropy of the input file binary.dat is %f\n",binary_entropy);
    printf("Huffman coding average codeword length is %f\n",average_binary);
    printf("\n\n");

    
    // TEXT DATA
    
    Node *tree_text;
    char code_text[256];
    int count_text = 0;
    int root_text = 0;
    double average_text = 0.0;
    double pr_text = 0.0;
    double text_entropy = 0.0;
    int buff_text[128] = {0};
    int order_text[128];
    unsigned long ch;
    long text_length = 0;
    
    
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
    
    for (int i=0;i<128;i++) {
        if(buff_text[i] != 0) {
            count_text++;
        }
    }
    
    int freq_text[count_text];
    int data_text[count_text];
    for (int j=0;j<count_text;j++) {
        freq_text[j] = buff_text[j];
        data_text[j] = order_text[j];
    }
    
    int *bits_text;
    printf("\n\n");
    printf("Huffman code for text.dat : \n");
    printf("value     code\n");
    Huffmantree(&tree_text, freq_text, data_text, count_text);
    bits_text = codetable(tree_text, code_text, root_text,0);
    
    bits_length = 0;
    file_length = 0;
    compressed_ratio = 0.0;
    for(int t=0;t<count_text;t++) {
        bits_length += (bits_text[data_text[t]]*freq_text[t]);
        
    }
    average_text = (double)bits_length/(double)text_length;
    
    if(bits_length % 8 == 0)
        file_length = bits_length/8;
    else
        file_length = bits_length/8 + 1;
    
    for(int k=0;k<count_text;k++) {
        pr_text = (double)buff_text[k] / (double)text_length;
        text_entropy += -(pr_text*log2(pr_text)); // Calculate entropy;
    }
    
    compressed_ratio = (double)(text_length-file_length) / (double)text_length * (double)100;
    printf("\n");
    printf("The size of the compressed file text.dat is %lu\n",file_length);
    printf("THe compressed ratio of the input file text.dat is %f%%\n",compressed_ratio);
    printf("The size of the input file text.dat is %ld Bytes\n",text_length);
    printf("The entropy of the input file text.dat is %f\n",text_entropy);
    printf("Huffman coding average codeword length is %f\n",average_text);
    printf("\n\n");

    
    // AUDIO DATA
    
    Node *tree_audio;
    char code_audio[256];
    int count_audio = 0;
    int root_audio = 0;
    double average_audio = 0.0;
    double pr_audio = 0.0;
    double audio_entropy = 0.0;
    int buff_audio[256] = {0};
    int order_audio[256];
    long audio_length = 0;
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
    
    for (int i=0;i<256;i++) {
        if(buff_audio[i] != 0) {
            count_audio++;
        }
    }
    
    
    
    int freq_audio[count_audio];
    int data_audio[count_audio];
    for (int j=0;j<count_audio;j++) {
        freq_audio[j] = buff_audio[j];
        data_audio[j] = order_audio[j];
    }
    
    int *bits_audio;
    printf("\n\n");
    printf("Huffman code for audio.dat : \n");
    printf("value     code\n");
    Huffmantree(&tree_audio, freq_audio, data_audio, count_audio);
    bits_audio = codetable(tree_audio, code_audio, root_audio,1);
   
    bits_length = 0;
    file_length = 0;
    compressed_ratio = 0.0;
    for(int t=0;t<count_audio;t++) {
        bits_length += (bits_audio[data_audio[t]]*freq_audio[t]);
        
    }
    average_audio = (double)bits_length/(double)audio_length;
    
    if(bits_length % 8 == 0)
        file_length = bits_length/8;
    else
        file_length = bits_length/8 + 1;
    
    for(int k=0;k<count_audio;k++) {
    pr_audio = (double)buff_audio[k] / (double)audio_length;
    audio_entropy += -(pr_audio*log2(pr_audio)); // Calculate entropy;
    }
    
    compressed_ratio = (double)(audio_length-file_length) / (double)audio_length * (double)100;
    printf("\n");
    printf("The size of the compressed file audio.dat is %lu\n",file_length);
    printf("The compressed ratio of the input file audio.dat is %f%%\n",compressed_ratio);
    printf("The size of the input file audio.dat is %ld Bytes\n",audio_length);
    printf("The entropy of the input file audio.dat is %f\n",audio_entropy);
    printf("Huffman coding average codeword length is %f\n",average_audio);
    printf("\n\n");
    
    
    // IMAGE DATA
    
    Node *tree_image;
    char code_image[256];
    int count_image = 0;
    int root_image = 0;
    double average_image = 0.0;
    double pr_image = 0.0;
    double image_entropy = 0.0;
    int buff_image[256] = {0};
    int order_image[256];
    long image_length = 0;
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
    
    for (int i=0;i<256;i++) {
        if(buff_image[i] != 0) {
            count_image++;
        }
    }
    
    int freq_image[count_image];
    int data_image[count_image];
    for (int j=0;j<count_image;j++) {
        freq_image[j] = buff_image[j];
        data_image[j] = order_image[j];
    }
    
    int *bits_image;
    printf("\n\n");
    printf("Huffman code for image.dat : \n");
    printf("value     code\n");
    Huffmantree(&tree_image, freq_image, data_image, count_image);
    bits_image = codetable(tree_image, code_image, root_image,1);

    bits_length = 0;
    file_length = 0;
    compressed_ratio = 0.0;
    for(int t=0;t<count_image;t++) {
        bits_length += (bits_image[data_image[t]]*freq_image[t]);
        
    }
    average_image = (double)bits_length/(double)image_length;
    
    if(bits_length % 8 == 0)
        file_length = bits_length/8;
    else
        file_length = bits_length/8 + 1;
    
    for(int k=0;k<count_image;k++) {
        pr_image = (double)buff_image[k] / (double)image_length;
        image_entropy += -(pr_image*log2(pr_image)); // Calculate entropy;
    }
    
    compressed_ratio = (double)(image_length-file_length) / (double)image_length * (double)100;
    printf("\n");
    printf("The size of the compressed file image.dat is %lu\n",file_length);
    printf("THe compressed ratio of the input file image.dat is %f%%\n",compressed_ratio);
    printf("The size of the input file image.dat is %ld Bytes\n",image_length);
    printf("The entropy of the input file image.dat is %f\n",image_entropy);
    printf("Huffman coding average codeword length is %f\n",average_image);
    printf("\n\n");
    
    
    
    return 0;
}

