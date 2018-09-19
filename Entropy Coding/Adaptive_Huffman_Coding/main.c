//
//  main.c
//  Adaptive_Huffman_Coding
//
//  Created by Tianxiao Zhang on 9/17/17.
//  Copyright © 2017 Tianxiao Zhang. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "bitio.h"
#include "errhand.h"
#define MAX_ORDER 512
#define MAX_LENGTH 65536

char audio_file[] = "/Users/zhtianxiao/Desktop/audio.dat";
char binary_file[] = "/Users/zhtianxiao/Desktop/binary.dat";
char image_file[] = "/Users/zhtianxiao/Desktop/image.dat";
char text_file[] = "/Users/zhtianxiao/Desktop/text.dat";

// The new 4 files I write on;
char coded_audio_file[] = "/Users/zhtianxiao/Desktop/coded_audio.dat";
char coded_binary_file[] = "/Users/zhtianxiao/Desktop/coded_binary.dat";
char coded_image_file[] = "/Users/zhtianxiao/Desktop/coded_image.dat";
char coded_text_file[] = "/Users/zhtianxiao/Desktop/coded_text.dat";


typedef struct node {
    int value;
    int weight;
    int order;
    
    struct node *left;
    struct node *right;
    struct node *parent;
    
}Node;


void create_new_node(Node **leaf, int value) { // Create new node;
    
    Node *temp = (Node *)malloc(sizeof(Node));
    
    temp->value = value;
    temp->weight = 1;
    
    temp->left = NULL;
    temp->right = NULL;
    temp->parent = NULL;
    
    *leaf = temp;
}


void create_new_nyt(Node **leaf) {          // Create new nyt node;
    
    Node *temp = (Node *)malloc(sizeof(Node));
    
    temp->value = -2;
    temp->weight = 0;
    
    temp->parent = NULL;
    temp->left = NULL;
    temp->right = NULL;
    
    *leaf = temp;
}

void add_node(Node **tree, Node *left, Node *right) {
    
    Node *temp = (Node *)malloc(sizeof(Node));
    
    temp->weight = left->weight + right->weight;
    temp->left = left;
    temp->right = right;
    temp->left->parent = temp;
    temp->right->parent = temp;
    temp->value = -1;
    temp->parent = NULL;
    *tree = temp;
}


// search the same weight in higher order;
void search_weight(Node **tree, int weight, int *order, int parent_order, Node **position, char *l_r) {
    
    if((*tree)->weight == weight && (*tree)->order > *order && (*tree)->order != parent_order) {
        
        *position = (*tree)->parent;
        *order = (*tree)->order;
        
        // To figure out the element is on the left for on the right
        if((*tree)->parent->right->order == (*tree)->order) {
            strcpy(l_r, "r");
        }
        else {
            strcpy(l_r, "l");
        }
    }
    
    if((*tree)-> left != NULL) {
        search_weight(&(*tree)->left, weight, &*order, parent_order, &*position, l_r);
    }
    
    if((*tree)-> right != NULL) {
        search_weight(&(*tree)->right, weight, &*order, parent_order, &*position, l_r);
    }
    
}


// Switch two nodes;
void swap_node(Node *tree, char *l_r, Node *sibling, char *l_r_sibling) {
    
    if(strcmp(l_r,"l") == 0 && strcmp(l_r_sibling,"l") == 0) {
        
        Node *temp = tree->left;
        tree->left = sibling->left;
        sibling->left = temp;
        
        tree->left->parent = tree;
        sibling->left->parent = sibling;
    }
    
    else if(strcmp(l_r,"l") == 0 && strcmp(l_r_sibling,"r") == 0) {
        
        Node *temp = tree->left;
        tree->left = sibling->right;
        sibling->right = temp;
        
        tree->left->parent = tree;
        sibling->right->parent = sibling;
    }
    
    else if(strcmp(l_r,"r") == 0 && strcmp(l_r_sibling,"l") == 0) {
        
        Node *temp = tree->right;
        tree->right = sibling->left;
        sibling->left = temp;
        
        tree->right->parent = tree;
        sibling->left->parent = sibling;
    }
    
    else if(strcmp(l_r,"r") == 0 && strcmp(l_r_sibling,"r") == 0) {
        
        Node *temp = tree->right;
        tree->right = sibling->right;
        sibling->right = temp;
        
        tree->right->parent = tree;
        sibling->right->parent = sibling;
    }
    
}


// Search the value in the tree equals to new value;
void search_value(Node **tree, int value, Node **position) {
    
    if((*tree)->left != NULL) {
        
        search_value(&((*tree)->left), value, &*position);
    }
    
    if((*tree)->value == value && (*tree)->left == NULL && (*tree)->right == NULL && (*tree)->weight != 0) {
        
        *position = *tree;
    }
    
    if((*tree)->right != NULL) {
        
        search_value(&((*tree)->right), value, &*position);
    }
    
}


// For adaptive Huffman tree, we do not need to print codetable
// So I write each codeword in a new file
// root is the length of codeword for each element
void codeword(Node **tree, int value, int code[], int root, BIT_FILE *coded_dat) {
    
    if((*tree)->value != -1) {
        if((*tree)->value == value) {
            for(int i=0;i<root;i++) {
                OutputBit(coded_dat, code[i]);
            }
            
            printf("\n");
        }
    }
    else{
        code[root] = 0;
        codeword(&((*tree)->left), value, code, root+1, coded_dat);
        code[root] = 1;
        codeword(&((*tree)->right), value, code, root+1, coded_dat);
    }
    
}


// Sort the order from root to leaves, from right to left
// The order of higher level is always larger than that of lower level
// On the same level, the order of right element is always larger than that of left element
void inorder(Node *tree)
{
    int max_order = MAX_ORDER;
    Node *p;
    Node *qu[MAX_ORDER];
    int front = -1;
    int rear = -1;
    rear++;
    qu[rear] = tree;
    while(front != rear)
    {
        front = (front+1) % MAX_ORDER;
        p= qu[front];
        p->order = max_order--;
        
        if(p->right != NULL)
        {
            rear=(rear+1) % MAX_ORDER;
            qu[rear] = p->right;
        }
        
        if(p->left != NULL)
        {
            rear= (rear+1) % 512;
            qu[rear] = p->left;
        }
        
    }
}


// Update the tree for each element is read in
void update_tree(Node **tree, int value, int buff[], Node **nyt) {
    Node *temp = NULL;
    // If the tree has the new element
    if(buff[value] >= 1) {
        search_value(&*tree, value, &temp);
        
        Node *inner_temp = NULL;
        char l_r[2];
        
        // To figure out the element is on the right or on the left
        if (temp->parent->right->order == temp->order) {
            
            strcpy(l_r,"r");
        } else {
            
            strcpy(l_r,"l");
        }
        
        char l_r_sibling[2];
        int order = temp->order;
        search_weight(&*tree, temp->weight, &order, temp->parent->order, &inner_temp, l_r_sibling);
        if (inner_temp != NULL) {
            swap_node(temp->parent, l_r, inner_temp, l_r_sibling);
        }
    }
    
    // If the tree does not have the new element
    // We need to generate new NYT
    if(buff[value] == 0) {
        Node *new_nyt;
        create_new_nyt(&new_nyt);
        Node *new_node;
        create_new_node(&new_node, value);
        
        Node *old_nyt = NULL;
        add_node(&old_nyt, new_nyt, new_node);
        
        if (*tree == NULL) {
            *tree = old_nyt;
            *nyt = (*tree)->left;
        }
        
        else {
            old_nyt->parent = (*nyt)->parent;
            (*nyt)->parent->left = old_nyt;
            *nyt = old_nyt->left;
        }
        
        temp = old_nyt;
    }
    
    inorder(*tree);
    
    // Increment weight
    if(temp->left != NULL && temp->right != NULL) {
        
        temp->weight = temp->left->weight + temp->right->weight;
    }
    
    else {
        temp->weight++;
    }
    
    
    
    while(temp->parent != NULL) {
        // Go to parent node
        temp = temp->parent;
        
        // If not the root
        if (temp->parent != NULL)
        {
            Node *inner_temp = NULL;
            
            char l_r[2];
            if (temp->parent->right->order == temp->order) {
                strcpy(l_r, "r");
                
            } else {
                strcpy(l_r,"l");
                
            }
            
            char l_r_sibling[2];
            int order = temp->order;
            search_weight(&*tree, temp->weight, &order, temp->parent->order, &inner_temp, l_r_sibling);
            if (inner_temp != NULL) {
                swap_node(temp->parent, l_r, inner_temp, l_r_sibling);
            }
            
        }
        // Increment weight
        if(temp->left != NULL && temp->right != NULL) {
            
            temp->weight = temp->left->weight + temp->right->weight;
        }
        
        else {
            temp->weight++;
        }
        
        *tree = temp;
    }
    
    inorder(*tree);
}


int main(int argc, const char *argv[]) {
    
    int root = 0;
       // BINARY DATA
    Node *tree_binary = NULL;
    Node *nyt_binary = NULL;
    int num_binary;
    int code_binary[256];
    int buff_binary[256] = {0};
    long binary_length = 0;
    double pr_binary = 0.0;;
    double binary_entropy = 0.0;
    double average_binary = 0.0;
    double compressed_ratio_binary = 0.0;

    
    BIT_FILE *binary_dat;
    binary_dat = OpenInputBitFile(binary_file);
    BIT_FILE *coded_binary_dat;
    coded_binary_dat = OpenOutputBitFile(coded_binary_file);
    
    while(!feof(binary_dat->file)) {
        num_binary = (int)InputBits(binary_dat, 8);
        update_tree(&tree_binary, num_binary, buff_binary, &nyt_binary);
        buff_binary[num_binary]++;
        binary_length++;
        codeword(&tree_binary, num_binary, code_binary, root, coded_binary_dat);
    }
    binary_length--;
    
    // For this part, I read the element from the new generated file
    // Because I output the codeword of adaptive Huffman coding into a new file
    // So this part I read the new file 1 byte each time to calculate the
    // compressed file length
    int num_coded_binary;
    int buff_coded_binary[256] = {0};
    long coded_binary_length = 0;
    BIT_FILE *read_coded_binary_dat;
    read_coded_binary_dat = OpenInputBitFile(coded_binary_file);
    
    while(!feof(read_coded_binary_dat->file)) {
        num_coded_binary = (int)InputBits(read_coded_binary_dat, 8);
        buff_coded_binary[num_coded_binary]++;
        coded_binary_length++;
    }
    coded_binary_length--;
    
    for(int k=0;k<256;k++) {
        pr_binary = (double)buff_binary[k] / (double)binary_length;
        if (pr_binary == 0)
            continue;
        binary_entropy += -(pr_binary*log2(pr_binary)); // Calculate entropy;
    }
    
    // Compute average codeword length
    average_binary = (double)(coded_binary_length*8)/(double)binary_length;
    // Compute compressed ratio
    compressed_ratio_binary = (double)(binary_length-coded_binary_length)/(double)binary_length*(double)100;
    


    

    
    
    // TEXT DATA
    
    Node *tree_text = NULL;
    Node *nyt_text = NULL;
    int num_text;
    int code_text[256];
    int buff_text[256] = {0};
    long text_length = 0;
    double pr_text = 0.0;;
    double text_entropy = 0.0;
    double average_text = 0.0;
    double compressed_ratio_text = 0.0;
    
    BIT_FILE *text_dat;
    text_dat = OpenInputBitFile(text_file);
    BIT_FILE *coded_text_dat;
    coded_text_dat = OpenOutputBitFile(coded_text_file);
    
    while(!feof(text_dat->file)) {
        num_text = (int)InputBits(text_dat, 8);
        update_tree(&tree_text, num_text, buff_text, &nyt_text);
        buff_text[num_text]++;
        text_length++;
        codeword(&tree_text, num_text, code_text, root, coded_text_dat);
    }
    text_length--;
    
    
    int num_coded_text;
    int buff_coded_text[256] = {0};
    long coded_text_length = 0;
    BIT_FILE *read_coded_text_dat;
    read_coded_text_dat = OpenInputBitFile(coded_text_file);
    while(!feof(read_coded_text_dat->file)) {
        num_coded_text = (int)InputBits(read_coded_text_dat, 8);
        buff_coded_text[num_coded_text]++;
        coded_text_length++;
    }
    coded_text_length--;
    
    for(int k=0;k<256;k++) {
        pr_text = (double)buff_text[k] / (double)text_length;
        if (pr_text == 0)
            continue;
        text_entropy += -(pr_text*log2(pr_text)); // Calculate entropy;
    }
    
    average_text = (double)(coded_text_length*8)/(double)text_length;
    compressed_ratio_text = (double)(text_length-coded_text_length)/(double)text_length*(double)100;
    
    
    // AUDIO DATA
    
    
    Node *tree_audio = NULL;
    Node *nyt_audio = NULL;
    int num_audio;
    int code_audio[256];
    int buff_audio[256] = {0};
    long audio_length = 0;
    double pr_audio = 0.0;;
    double audio_entropy = 0.0;
    double average_audio = 0.0;
    double compressed_ratio_audio = 0.0;
    
    BIT_FILE *audio_dat;
    audio_dat = OpenInputBitFile(audio_file);
    BIT_FILE *coded_audio_dat;
    coded_audio_dat = OpenOutputBitFile(coded_audio_file);
    
    while(!feof(audio_dat->file)) {
        num_audio = (int)InputBits(audio_dat, 8);
        update_tree(&tree_audio, num_audio, buff_audio, &nyt_audio);
        buff_audio[num_audio]++;
        audio_length++;
        codeword(&tree_audio, num_audio, code_audio, root, coded_audio_dat);
    }
    audio_length--;
    
    
    int num_coded_audio;
    int buff_coded_audio[256] = {0};
    long coded_audio_length = 0;
    BIT_FILE *read_coded_audio_dat;
    read_coded_audio_dat = OpenInputBitFile(coded_audio_file);
    while(!feof(read_coded_audio_dat->file)) {
        num_coded_audio = (int)InputBits(read_coded_audio_dat, 8);
        buff_coded_audio[num_coded_audio]++;
        coded_audio_length++;
    }
    coded_audio_length--;
    
    for(int k=0;k<256;k++) {
        pr_audio = (double)buff_audio[k] / (double)audio_length;
        if (pr_audio == 0)
            continue;
        audio_entropy += -(pr_audio*log2(pr_audio)); // Calculate entropy;
    }
    
    average_audio = (double)(coded_audio_length*8)/(double)audio_length;
    compressed_ratio_audio = (double)(audio_length-coded_audio_length)/(double)audio_length*(double)100;
    

    


    
    
    // IMAGE DATA
    

    Node *tree_image = NULL;
    Node *nyt_image = NULL;
    int num_image;
    int code_image[256];
    int buff_image[256] = {0};
    long image_length = 0;
    double pr_image = 0.0;;
    double image_entropy = 0.0;
    double average_image = 0.0;
    double compressed_ratio_image = 0.0;
    
    BIT_FILE *image_dat;
    image_dat = OpenInputBitFile(image_file);
    BIT_FILE *coded_image_dat;
    coded_image_dat = OpenOutputBitFile(coded_image_file);
    
    while(!feof(image_dat->file)) {
        num_image = (int)InputBits(image_dat, 8);
        update_tree(&tree_image, num_image, buff_image, &nyt_image);
        buff_image[num_image]++;
        image_length++;
        codeword(&tree_image, num_image, code_image, root, coded_image_dat);
    }
    image_length--;
    
    
    int num_coded_image;
    int buff_coded_image[256] = {0};
    long coded_image_length = 0;
    BIT_FILE *read_coded_image_dat;
    read_coded_image_dat = OpenInputBitFile(coded_image_file);
    while(!feof(read_coded_image_dat->file)) {
        num_coded_image = (int)InputBits(read_coded_image_dat, 8);
        buff_coded_image[num_coded_image]++;
        coded_image_length++;
    }
    coded_image_length--;
    
    for(int k=0;k<256;k++) {
        pr_image = (double)buff_image[k] / (double)image_length;
        if (pr_image == 0)
            continue;
        image_entropy += -(pr_image*log2(pr_image)); // Calculate entropy;
    }
    
    average_image = (double)(coded_image_length*8)/(double)image_length;
    compressed_ratio_image = (double)(image_length-coded_image_length)/(double)image_length*(double)100;
    
    

    // BINARY DATA
    
printf("\n\n");
printf("Adaptive Huffman coding for binary.dat : \n");
printf("The size of the compressed file binary.dat is %lu\n",coded_binary_length);
printf("The compressed ratio of the input file binary.dat is %f%%\n",compressed_ratio_binary);
printf("The size of the input file binary.dat is %ld Bytes\n",binary_length);
printf("The entropy of the input file binary.dat is %f\n",binary_entropy);
printf("Adaptive Huffman coding average codeword length is %f\n",average_binary);
printf("\n\n");

    
    // TEXT DATA
    
printf("\n\n");
printf("Adaptive Huffman coding for text.dat : \n");
printf("The size of the compressed file text.dat is %lu\n",coded_text_length);
printf("The compressed ratio of the input file text.dat is %f%%\n",compressed_ratio_text);
printf("The size of the input file text.dat is %ld Bytes\n",text_length);
printf("The entropy of the input file text.dat is %f\n",text_entropy);
printf("Adaptive Huffman coding average codeword length is %f\n",average_text);
printf("\n\n");

    // AUDIO DATA
    
    
printf("\n\n");
printf("Adaptive Huffman coding for audio.dat : \n");
printf("The size of the compressed file audio.dat is %lu\n",coded_audio_length);
printf("The compressed ratio of the input file audio.dat is %f%%\n",compressed_ratio_audio);
printf("The size of the input file audio.dat is %ld Bytes\n",audio_length);
printf("The entropy of the input file audio.dat is %f\n",audio_entropy);
printf("Adaptive Huffman coding average codeword length is %f\n",average_audio);
printf("\n\n");

    
    
    // IMAGE DATA

printf("\n\n");
printf("Adaptive Huffman coding for image.dat : \n");
printf("The size of the compressed file image.dat is %lu\n",coded_image_length);
printf("The compressed ratio of the input file image.dat is %f%%\n",compressed_ratio_image);
printf("The size of the input file image.dat is %ld Bytes\n",image_length);
printf("The entropy of the input file image.dat is %f\n",image_entropy);
printf("Adaptive Huffman coding average codeword length is %f\n",average_image);
printf("\n\n");



    
    return 0;
}

