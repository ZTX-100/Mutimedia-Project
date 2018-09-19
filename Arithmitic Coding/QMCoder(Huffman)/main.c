//
//  main.c
//  QMCoder(Huffman)
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
#define MAX_SIZE 65537

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

char audio_file[] = "/Users/zhtianxiao/Desktop/audio.dat";
char binary_file[] = "/Users/zhtianxiao/Desktop/binary.dat";
char image_file[] = "/Users/zhtianxiao/Desktop/image.dat";
char text_file[] = "/Users/zhtianxiao/Desktop/text.dat";

char audio_QMcoded_file[] = "/Users/zhtianxiao/Desktop/QMcoded_audio.dat";
char binary_QMcoded_file[] = "/Users/zhtianxiao/Desktop/QMcoded_binary.dat";
char image_QMcoded_file[] = "/Users/zhtianxiao/Desktop/QMcoded_image.dat";
char text_QMcoded_file[] = "/Users/zhtianxiao/Desktop/QMcoded_text.dat";

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



void Huffmancode(Node *tree, char codetable[][64], char codeword[], int root) {
    
    if(tree->element == 256){
        codeword[root] = '0';
        Huffmancode(tree->right, codetable, codeword, root+1);
        
        codeword[root] = '1';
        Huffmancode(tree->left, codetable, codeword, root+1);
    }
    
    else if(tree->element != 256) {
        memcpy(codetable[tree->element],codeword,root+1);
    }
}



/* Build the Huffman tree from bottom to root */
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
    
    /* Merge two smallest frequencies */
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
    /**********************************************************************************/
    /************************** Beginning Binary Data *********************************/
    /**********************************************************************************/
    
    
    /*********************************************/
    /*********** Binary Huffman coding ***********/
    /*********************************************/
    Node *tree_binary;
    int count_binary = 0;
    int root_binary = 0;
    int temp;
    int temper;
    int element_binary[MAX_SIZE];
    int buff_binary[256] = {0};
    int order_binary[256];
    int binary_size = 0;
    
    BIT_FILE *binary_dat;
    binary_dat = OpenInputBitFile(binary_file);
    
    // If the file does not end, we continue reading data from the file;
    while(!feof(binary_dat->file)) {
        element_binary[binary_size] = (int)InputBits(binary_dat, 8);
        buff_binary[element_binary[binary_size]]++;
        binary_size++;
    }
    binary_size--;
    
    for (int t=0;t<256;t++) {
        order_binary[t] = t;
    }
    
    /* Sort the frequency in desceding and also their corresponding element */
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
    
    /* Count element numbers */
    for (int i=0;i<256;i++) {
        if(buff_binary[i] != 0) {
            count_binary++;
        }
    }
    
    /* Put the elements and their frequencies into new arraies */
    int freq_binary[count_binary];
    int data_binary[count_binary];
    for (int j=0;j<count_binary;j++) {
        freq_binary[j] = buff_binary[j];
        data_binary[j] = order_binary[j];
    }
    
    
    int bits[MAX_SIZE*8];
    int counting = 0;
    char binary_codetable[256][64];
    char binary_codeword[64];
    
    for(int i=0;i<256;i++) {
        for(int j=0;j<64;j++) {
            binary_codetable[i][j] = 'N';
        }
    }
    
    Huffmantree(&tree_binary, freq_binary, data_binary, count_binary);
    Huffmancode(tree_binary, binary_codetable, binary_codeword, root_binary);
    
    /* Read the Huffman coded data bit by bit */
    for(int i=0;i<binary_size;i++) {
        int j = 0;
        while(binary_codetable[element_binary[i]][j] != 'N') {
            if(binary_codetable[element_binary[i]][j] == '0') {
                bits[counting] = 0;
                counting++;
            }
            else if(binary_codetable[element_binary[i]][j] == '1') {
                bits[counting] = 1;
                counting++;
            }
            j++;
        }
      
        
    }
    counting--;
    
    /**********************************************/
    /************** Binary QM coding **************/
    /**********************************************/
    int n = 3;
    int buffer[n];
    int context = 0;
    double compressed_ratio;
    
    BIT_FILE *binary_qm_dat;
    binary_qm_dat = OpenOutputBitFile(binary_QMcoded_file);
    
    for (int t=0;t<n;t++) {
        buffer[t] = 0;
    }
    
    QM(binary_qm_dat->file);
    reset();
    Initialize();
    for(int i=0;i<counting;i++) {
        
            encode(bits[i], context);
            for(int p=0;p<n-1;p++) {
                buffer[p] = buffer[p+1];
            }
            
            buffer[n-1] = bits[i];
            context = 0;
            
            for(int q=0;q<n;q++) {
                context += (int)exp2(q)*buffer[q];
            }
        
    }
    Flush();
    
    compressed_ratio = ((double)(binary_size*8 - Counting()) / (double)(binary_size*8))*(double)100;
    printf("\n\n");
    printf("The results of binary.dat : \n");
    printf("The QM coded bit number using Huffman coding with context rule n = %d is %d\n",n,Counting());
    printf("The QM coded comressed ratio using Huffman coding with context rule n = %d is %f%%\n",n,compressed_ratio);
    printf("\n\n");
    /*******************************************************************************/
    /************************** Ending Binary Data *********************************/
    /*******************************************************************************/
    
    
    /*******************************************************************************/
    /************************** Beginning Text Data ********************************/
    /*******************************************************************************/
    
    
    /*********************************************/
    /******** Text Huffman coding coding *********/
    /*********************************************/
    Node *tree_text;
    int count_text = 0;
    int root_text = 0;
    int element_text[MAX_SIZE];
    int buff_text[128] = {0};
    int order_text[128];
    int text_size = 0;
    char text_codetable[128][64];
    
    BIT_FILE *text_dat;
    text_dat = OpenInputBitFile(text_file);
    
    /* If the file does not end, we continue reading data from the file */
    while(!feof(text_dat->file)) {
        element_text[text_size] = (int)InputBits(text_dat, 8);
        buff_text[element_text[text_size]]++;
        text_size++;
    }
    text_size--;
    
    for (int t=0;t<128;t++) {
        order_text[t] = t;
    }
    
    /* Sort the frequency in desceding and also their corresponding element */
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
    
    /* Count element numbers */
    for (int i=0;i<128;i++) {
        if(buff_text[i] != 0) {
            count_text++;
        }
    }
    
    /* Put the elements and their frequencies into new arraies */
    int freq_text[count_text];
    int data_text[count_text];
    for (int j=0;j<count_text;j++) {
        freq_text[j] = buff_text[j];
        data_text[j] = order_text[j];
    }
    
    
    counting = 0;
    char text_codeword[64];
    
    for(int i=0;i<128;i++) {
        for(int j=0;j<64;j++) {
            text_codetable[i][j] = 'N';
        }
    }
    
    Huffmantree(&tree_text, freq_text, data_text, count_text);
    Huffmancode(tree_text, text_codetable, text_codeword, root_text);
    /* Read the Huffman coded data bit by bit */
    for(int i=0;i<text_size;i++) {
        int j = 0;
        while(text_codetable[element_text[i]][j] != 'N') {
            if(text_codetable[element_text[i]][j] == '0') {
                bits[counting] = 0;
                counting++;
            }
            else if(text_codetable[element_text[i]][j] == '1') {
                bits[counting] = 1;
                counting++;
            }
            j++;
        }
        }
        counting--;
    /**********************************************/
    /*************** Text QM coding ***************/
    /**********************************************/
    context = 0;
    
    BIT_FILE *text_qm_dat;
    text_qm_dat = OpenOutputBitFile(text_QMcoded_file);
    
    for (int t=0;t<n;t++) {
        buffer[t] = 0;
    }
    
    QM(text_qm_dat->file);
    reset();
    Initialize();
    for(int i=0;i<counting;i++) {
        
        encode(bits[i], context);
        for(int p=0;p<n-1;p++) {
            buffer[p] = buffer[p+1];
        }
        
        buffer[n-1] = bits[i];
        context = 0;
        
        for(int q=0;q<n;q++) {
            context += (int)exp2(q)*buffer[q];
        }
        
    }
    Flush();
    
    compressed_ratio = ((double)(text_size*8 - Counting()) / (double)(text_size*8))*(double)100;
    printf("\n\n");
    printf("The results of text.dat : \n");
    printf("The QM coded bit number using Huffman coding with context rule n = %d is %d\n",n,Counting());
    printf("The QM coded comressed ratio using Huffman coding with context rule n = %d is %f%%\n",n,compressed_ratio);
    printf("\n\n");
    /*****************************************************************************/
    /************************** Ending Text Data *********************************/
    /*****************************************************************************/
    
    
    /*****************************************************************************/
    /************************* Beginning Audio Data ******************************/
    /*****************************************************************************/
    
    
    /*********************************************/
    /*********** Audio Huffman coding ************/
    /*********************************************/
    Node *tree_audio;
    int count_audio = 0;
    int root_audio = 0;
    int element_audio[MAX_SIZE];
    int buff_audio[256] = {0};
    int order_audio[256];
    int audio_size = 0;
    char audio_codetable[256][64];
    
    BIT_FILE *audio_dat;
    audio_dat = OpenInputBitFile(audio_file);
    
    /* If the file does not end, we continue reading data from the file */
    while(!feof(audio_dat->file)) {
        element_audio[audio_size] = (int)InputBits(audio_dat, 8);
        buff_audio[element_audio[audio_size]]++;
        audio_size++;
    }
    audio_size--;
    
    for (int t=0;t<256;t++) {
        order_audio[t] = t;
    }
    
    /* Sort the frequency in desceding and also their corresponding element */
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
    
    /* Count element numbers */
    for (int i=0;i<256;i++) {
        if(buff_audio[i] != 0) {
            count_audio++;
        }
    }
    
    /* Put the elements and their frequencies into new arraies */
    int freq_audio[count_audio];
    int data_audio[count_audio];
    for (int j=0;j<count_audio;j++) {
        freq_audio[j] = buff_audio[j];
        data_audio[j] = order_audio[j];
    }
    
    

        counting = 0;
        char audio_codeword[64];
    
        for(int i=0;i<256;i++) {
            for(int j=0;j<64;j++) {
                audio_codetable[i][j] = 'N';
            }
        }
        
        Huffmantree(&tree_audio, freq_audio, data_audio, count_audio);
        Huffmancode(tree_audio, audio_codetable, audio_codeword, root_audio);
    /* Read the Huffman coded data bit by bit */
        for(int i=0;i<audio_size;i++) {
            int j = 0;
            while(audio_codetable[element_audio[i]][j] != 'N') {
                if(audio_codetable[element_audio[i]][j] == '0') {
                    bits[counting] = 0;
                    counting++;
                }
                else if(audio_codetable[element_audio[i]][j] == '1') {
                    bits[counting] = 1;
                    counting++;
                }
                j++;
            }
            }
            counting--;
    
    /**********************************************/
    /*************** Audio QM coding **************/
    /**********************************************/
    context = 0;
    
    BIT_FILE *audio_qm_dat;
    audio_qm_dat = OpenOutputBitFile(audio_QMcoded_file);
    
    for (int t=0;t<n;t++) {
        buffer[t] = 0;
    }
    
    QM(audio_qm_dat->file);
    reset();
    Initialize();
    for(int i=0;i<counting;i++) {
        
        encode(bits[i], context);
        for(int p=0;p<n-1;p++) {
            buffer[p] = buffer[p+1];
        }
        
        buffer[n-1] = bits[i];
        context = 0;
        
        for(int q=0;q<n;q++) {
            context += (int)exp2(q)*buffer[q];
        }
        
    }
    Flush();
    
    compressed_ratio = ((double)(audio_size*8 - Counting()) / (double)(audio_size*8))*(double)100;
    printf("\n\n");
    printf("The results of binary.dat : \n");
    printf("The QM coded bit number using Huffman coding with context rule n = %d is %d\n",n,Counting());
    printf("The QM coded comressed ratio using Huffman coding with context rule n = %d is %f%%\n",n,compressed_ratio);
    printf("\n\n");
    /********************************************************************************/
    /************************** Ending Audio Data ***********************************/
    /********************************************************************************/
    
    
    /********************************************************************************/
    /*************************** Beginning Image Data *******************************/
    /********************************************************************************/
    
    
    /*********************************************/
    /************ Image Huffman coding ***********/
    /*********************************************/
    Node *tree_image;
    int count_image = 0;
    int root_image = 0;
    int element_image[MAX_SIZE];
    int buff_image[256] = {0};
    int order_image[256];
    int image_size = 0;
    char image_codetable[256][64];
    
    BIT_FILE *image_dat;
    image_dat = OpenInputBitFile(image_file);
    
    /* If the file does not end, we continue reading data from the file */
    while(!feof(image_dat->file)) {
        element_image[image_size] = (int)InputBits(image_dat, 8);
        buff_image[element_image[image_size]]++;
        image_size++;
    }
    image_size--;
    
    for (int t=0;t<256;t++) {
        order_image[t] = t;
    }
    
    /* Sort the frequency in desceding and also their corresponding element */
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
    
    /* Count element numbers */
    for (int i=0;i<256;i++) {
        if(buff_image[i] != 0) {
            count_image++;
        }
    }
    
    /* Put the elements and their frequencies into new arraies */
    int freq_image[count_image];
    int data_image[count_image];
    for (int j=0;j<count_image;j++) {
        freq_image[j] = buff_image[j];
        data_image[j] = order_image[j];
    }
    
            counting = 0;
            char image_codeword[64];
    
            for(int i=0;i<256;i++) {
                for(int j=0;j<64;j++) {
                    image_codetable[i][j] = 'N';
                }
            }
            
            Huffmantree(&tree_image, freq_image, data_image, count_image);
            Huffmancode(tree_image, image_codetable, image_codeword, root_image);
            /* Read the Huffman coded data bit by bit */
            for(int i=0;i<image_size;i++) {
                int j = 0;
                while(image_codetable[element_image[i]][j] != 'N') {
                    if(image_codetable[element_image[i]][j] == '0') {
                        bits[counting] = 0;
                        counting++;
                    }
                    else if(image_codetable[element_image[i]][j] == '1') {
                        bits[counting] = 1;
                        counting++;
                    }
                    j++;
                }
            }
                counting--;
                
  
    
    /**********************************************/
    /*************** Image QM coding **************/
    /**********************************************/
    context = 0;
    
    BIT_FILE *image_qm_dat;
    image_qm_dat = OpenOutputBitFile(image_QMcoded_file);
    
    for (int t=0;t<n;t++) {
        buffer[t] = 0;
    }
    
    QM(image_qm_dat->file);
    reset();
    Initialize();
    for(int i=0;i<counting;i++) {
        
        encode(bits[i], context);
        for(int p=0;p<n-1;p++) {
            buffer[p] = buffer[p+1];
        }
        
        buffer[n-1] = bits[i];
        context = 0;
        
        for(int q=0;q<n;q++) {
            context += (int)exp2(q)*buffer[q];
        }
        
    }
    Flush();
    
    compressed_ratio = ((double)(image_size*8 - Counting()) / (double)(image_size*8))*(double)100;
    printf("\n\n");
    printf("The results of image.dat : \n");
    printf("The QM coded bit number using Huffman coding with context rule n = %d is %d\n",n,Counting());
    printf("The QM coded comressed ratio using Huffman coding with context rule n = %d is %f%%\n",n,compressed_ratio);
    printf("\n\n");
    /******************************************************************************/
    /************************** Ending Image Data *********************************/
    /******************************************************************************/
    
    
    
    return 0;
}



