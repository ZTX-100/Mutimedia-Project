//
//  main.c
//  JPEG_Compression_Quality_Factor
//
//  Created by Tianxiao Zhang on 10/2/17.
//  Copyright © 2017 Tianxiao Zhang. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include "bitio.h"
#include "errhand.h"

#define SIZE 65536



char clock_1_file[] = "/Users/zhtianxiao/Desktop/clock/clock_1.jpg";
char clock_10_file[] = "/Users/zhtianxiao/Desktop/clock/clock_10.jpg";
char clock_20_file[] = "/Users/zhtianxiao/Desktop/clock/clock_20.jpg";
char clock_40_file[] = "/Users/zhtianxiao/Desktop/clock/clock_40.jpg";
char clock_60_file[] = "/Users/zhtianxiao/Desktop/clock/clock_60.jpg";
char clock_100_file[] = "/Users/zhtianxiao/Desktop/clock/clock_100.jpg";
char clock_bmp_file[] = "/Users/zhtianxiao/Desktop/clock/clock.bmp";

char clock_1_raw_file[] = "/Users/zhtianxiao/Desktop/clock/clock_1.raw";
char clock_10_raw_file[] = "/Users/zhtianxiao/Desktop/clock/clock_10.raw";
char clock_20_raw_file[] = "/Users/zhtianxiao/Desktop/clock/clock_20.raw";
char clock_40_raw_file[] = "/Users/zhtianxiao/Desktop/clock/clock_40.raw";
char clock_60_raw_file[] = "/Users/zhtianxiao/Desktop/clock/clock_60.raw";
char clock_100_raw_file[] = "/Users/zhtianxiao/Desktop/clock/clock_100.raw";
char clock_bmp_raw_file[] = "/Users/zhtianxiao/Desktop/clock/clock.raw";


/* The function is used to calculate the maximum of pixel values */
int max_value(int array[], int size) {
    
    int max = 0;
    for(int t=0;t<size;t++) {
        if(array[t] > max) {
            max = array[t];
        }
    }
    return max;
}



int main(int argc, const char * argv[]) {
    /******************************************************/
    /************** Read the original image ***************/
    /******************************************************/
    int clock[SIZE*2];
    int clock_size = 0;
    BIT_FILE *clock_bmp;
    clock_bmp = OpenInputBitFile(clock_bmp_file);
    
    while(!feof(clock_bmp->file)) {
        clock[clock_size++] = (int)InputBits(clock_bmp, 8);
    }
    clock_size--;
    
    
    int clock_bmp_raw[SIZE];
    BIT_FILE *clock_raw;
    clock_raw = OpenInputBitFile(clock_bmp_raw_file);
    
    for(int i=0;i<SIZE;i++) {
        clock_bmp_raw[i] = (int)InputBits(clock_raw, 8);
    }
    
    
    printf("\n\n");
    printf("The original image size is %d Bytes\n",clock_size);
    /******************************************************/
    
    
    
    double compressed_ratio;
    /******************************************************/
    /******** Read JPEG with quality factor = 100 *********/
    /******************************************************/
    int clock_100[SIZE];
    int clock_100_size = 0;
    BIT_FILE *clock_100_jpg;
    clock_100_jpg = OpenInputBitFile(clock_100_file);
    
    while(!feof(clock_100_jpg->file)) {
        clock_100[clock_100_size++] = (int)InputBits(clock_100_jpg, 8);
    }
    clock_100_size--;
    
    compressed_ratio = (double)(clock_size - clock_100_size) / (double)clock_size * (double)100;
    
    
    
    double mse = 0;
    double peak_value = 255;
    double psnr = 0;
    int clock_100_raw[SIZE];
    
    BIT_FILE *clock_100_jpg_raw;
    clock_100_jpg_raw = OpenInputBitFile(clock_100_raw_file);
    
    for(int i=0;i<SIZE;i++) {
        clock_100_raw[i] = (int)InputBits(clock_100_jpg_raw, 8);
        mse += pow(clock_bmp_raw[i]-clock_100_raw[i],2)/(double)SIZE;
    }
    
    psnr = 10*log10(pow(peak_value,2)/mse);
    
    printf("\n\n");
    printf("The JPEG image size with quality factor 100 is %d Bytes\n",clock_100_size);
    printf("The compressed ratio for JPEG image with quality factor 100 is %f%%\n", compressed_ratio);
    printf("The PSNR of JPEG image for quality factor 100 is %f\n",psnr);
    /******************************************************/
    
    
    /******************************************************/
    /********* Read JPEG with quality factor = 60 *********/
    /******************************************************/
    int clock_60[SIZE];
    int clock_60_size = 0;
    BIT_FILE *clock_60_jpg;
    clock_60_jpg = OpenInputBitFile(clock_60_file);
    
    while(!feof(clock_60_jpg->file)) {
        clock_60[clock_60_size++] = (int)InputBits(clock_60_jpg, 8);
    }
    clock_60_size--;
    
    compressed_ratio = (double)(clock_size - clock_60_size) / (double)clock_size * (double)100;
    
    
    mse = 0;
    int clock_60_raw[SIZE];
    
    BIT_FILE *clock_60_jpg_raw;
    clock_60_jpg_raw = OpenInputBitFile(clock_60_raw_file);
    
    for(int i=0;i<SIZE;i++) {
        clock_60_raw[i] = (int)InputBits(clock_60_jpg_raw, 8);
        mse += pow(clock_bmp_raw[i]-clock_60_raw[i],2)/(double)SIZE;
    }
    
    psnr = 10*log10(pow(peak_value,2)/mse);
    
    printf("\n\n");
    printf("The JPEG image size with quality factor 60 is %d Bytes\n",clock_60_size);
    printf("The compressed ratio for JPEG image with quality factor 60 is %f%%\n", compressed_ratio);
    printf("The PSNR of JPEG image for quality factor 60 is %f\n",psnr);
    /******************************************************/
    
    
    /******************************************************/
    /********* Read JPEG with quality factor = 40 *********/
    /******************************************************/
    int clock_40[SIZE];
    int clock_40_size = 0;
    BIT_FILE *clock_40_jpg;
    clock_40_jpg = OpenInputBitFile(clock_40_file);
    
    while(!feof(clock_40_jpg->file)) {
        clock_40[clock_40_size++] = (int)InputBits(clock_40_jpg, 8);
    }
    clock_40_size--;
    
    compressed_ratio = (double)(clock_size - clock_40_size) / (double)clock_size * (double)100;
    
    
    mse = 0;
    int clock_40_raw[SIZE];
    
    BIT_FILE *clock_40_jpg_raw;
    clock_40_jpg_raw = OpenInputBitFile(clock_40_raw_file);
    
    for(int i=0;i<SIZE;i++) {
        clock_40_raw[i] = (int)InputBits(clock_40_jpg_raw, 8);
        mse += pow(clock_bmp_raw[i]-clock_40_raw[i],2)/(double)SIZE;
    }
    
    psnr = 10*log10(pow(peak_value,2)/mse);
    
    printf("\n\n");
    printf("The JPEG image size with quality factor 40 is %d Bytes\n",clock_40_size);
    printf("The compressed ratio for JPEG image with quality factor 40 is %f%%\n", compressed_ratio);
    printf("The PSNR of JPEG image for quality factor 40 is %f\n",psnr);
    /******************************************************/
    
    
    /******************************************************/
    /********* Read JPEG with quality factor = 20 *********/
    /******************************************************/
    int clock_20[SIZE];
    int clock_20_size = 0;
    BIT_FILE *clock_20_jpg;
    clock_20_jpg = OpenInputBitFile(clock_20_file);
    
    while(!feof(clock_20_jpg->file)) {
        clock_20[clock_20_size++] = (int)InputBits(clock_20_jpg, 8);
    }
    clock_20_size--;
    
    compressed_ratio = (double)(clock_size - clock_20_size) / (double)clock_size * (double)100;
    
    
    
    mse = 0;
    int clock_20_raw[SIZE];
    
    BIT_FILE *clock_20_jpg_raw;
    clock_20_jpg_raw = OpenInputBitFile(clock_20_raw_file);
    
    for(int i=0;i<SIZE;i++) {
        clock_20_raw[i] = (int)InputBits(clock_20_jpg_raw, 8);
        mse += pow(clock_bmp_raw[i]-clock_20_raw[i],2)/(double)SIZE;
    }
    
    psnr = 10*log10(pow(peak_value,2)/mse);
    
    printf("\n\n");
    printf("The JPEG image size with quality factor 20 is %d Bytes\n",clock_20_size);
    printf("The compressed ratio for JPEG image with quality factor 20 is %f%%\n", compressed_ratio);
    printf("The PSNR of JPEG image for quality factor 20 is %f\n",psnr);
    /******************************************************/
    
    
    /******************************************************/
    /********* Read JPEG with quality factor = 10 *********/
    /******************************************************/
    int clock_10[SIZE];
    int clock_10_size = 0;
    BIT_FILE *clock_10_jpg;
    clock_10_jpg = OpenInputBitFile(clock_10_file);
    
    while(!feof(clock_10_jpg->file)) {
        clock_10[clock_10_size++] = (int)InputBits(clock_10_jpg, 8);
    }
    clock_10_size--;
    
    compressed_ratio = (double)(clock_size - clock_10_size) / (double)clock_size * (double)100;
    
    
    
    mse = 0;
    int clock_10_raw[SIZE];
    
    BIT_FILE *clock_10_jpg_raw;
    clock_10_jpg_raw = OpenInputBitFile(clock_10_raw_file);
    
    for(int i=0;i<SIZE;i++) {
        clock_10_raw[i] = (int)InputBits(clock_10_jpg_raw, 8);
        mse += pow(clock_bmp_raw[i]-clock_10_raw[i],2)/(double)SIZE;
    }
    
    psnr = 10*log10(pow(peak_value,2)/mse);
    
    printf("\n\n");
    printf("The JPEG image size with quality factor 10 is %d Bytes\n",clock_10_size);
    printf("The compressed ratio for JPEG image with quality factor 10 is %f%%\n", compressed_ratio);
    printf("The PSNR of JPEG image for quality factor 10 is %f\n",psnr);
    /******************************************************/
    
    
    /******************************************************/
    /********* Read JPEG with quality factor = 1 **********/
    /******************************************************/
    int clock_1[SIZE];
    int clock_1_size = 0;
    BIT_FILE *clock_1_jpg;
    clock_1_jpg = OpenInputBitFile(clock_1_file);
    
    while(!feof(clock_1_jpg->file)) {
        clock_1[clock_1_size++] = (int)InputBits(clock_1_jpg, 8);
    }
    clock_1_size--;
    
    compressed_ratio = (double)(clock_size - clock_1_size) / (double)clock_size * (double)100;
    
    
    
    mse = 0;
    int clock_1_raw[SIZE];
    
    BIT_FILE *clock_1_jpg_raw;
    clock_1_jpg_raw = OpenInputBitFile(clock_1_raw_file);
    
    for(int i=0;i<SIZE;i++) {
        clock_1_raw[i] = (int)InputBits(clock_1_jpg_raw, 8);
        mse += pow(clock_bmp_raw[i]-clock_1_raw[i],2)/(double)SIZE;
    }
    
    psnr = 10*log10(pow(peak_value,2)/mse);
    
    printf("\n\n");
    printf("The JPEG image size with quality factor 1 is %d Bytes\n",clock_1_size);
    printf("The compressed ratio for JPEG image with quality factor 1 is %f%%\n", compressed_ratio);
    printf("The PSNR of JPEG image for quality factor 1 is %f\n",psnr);
    /******************************************************/
    printf("\n\n");
    
    
    return 0;
}
