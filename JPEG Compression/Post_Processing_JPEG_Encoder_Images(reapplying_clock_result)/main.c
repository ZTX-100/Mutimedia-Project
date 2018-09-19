//
//  main.c
//  Post_Processing_JPEG_Encoder_Images(reapplying_clock1_result)
//
//  Created by Tianxiao Zhang on 10/5/17.
//  Copyright © 2017 Tianxiao Zhang. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include "bitio.h"
#include "errhand.h"

#define size 256
#define SIZE 65536

char clock_raw_file[] = "/Users/zhtianxiao/Desktop/Reapplying/raw_image/clock.raw";
char clock_jpg_raw_output_file[] = "/Users/zhtianxiao/Desktop/clock5_reapplying.raw";

char clock_jpg_raw_file[64][100];

/* The funtion is to shift the image */
void image_shift(int image[size][size], int shifted_image[size][size], int i, int j) {
    
    for(int m=0;m<size;m++) {
        for(int n=0;n<size;n++) {
            shifted_image[m][n] = image[m][n];
        }
    }
    
    /* If shift to upper */
    if(i >= 0) {
        
        /* If shift to upper right */
        if(j >= 0) {
            
            for(int m=0;m<size-i;m++) {
                for(int n=0;n<size-j;n++) {
                    shifted_image[m][n+j] = image[m+i][n];
                }
            }
            
        }
        
        /* if shift to upper left */
        if(j < 0) {
            
            for(int m=0;m<size-i;m++) {
                for(int n=0;n<size+j;n++) {
                    shifted_image[m][n] = image[m+i][n-j];
                }
            }
            
        }
        
        
        
    }
    
    /* If shift to lower */
    if(i < 0) {
        
        /* If shift to lower right */
        if(j >= 0) {
            
            for(int m=0;m<size+i;m++) {
                for(int n=0;n<size-j;n++) {
                    shifted_image[m-i][n+j] = image[m][n];
                }
            }
            
        }
        
        /* If shift to lower left */
        if(j < 0) {
            
            for(int m=0;m<size+i;m++) {
                for(int n=0;n<size+j;n++) {
                    shifted_image[m-i][n] = image[m][n-j];
                }
            }
            
        }
        
        
    }
    
    
    
}



int main(int argc, const char * argv[]) {
    
    int clock_reapplying[size][size];
    int clock_n[size][size];
    int clock_shifted[size][size];
    int clock[size][size];
    int count = 0;
    
    double mse_clock = 0;
    double psnr_clock = 0;
    double peak_clock = 255;
    
    BIT_FILE *clock_raw;
    clock_raw = OpenInputBitFile(clock_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            clock_reapplying[i][j] = 0;
            clock[i][j] = (int)InputBits(clock_raw, 8);
        }
    }
    
    /* Using loop to print all 64 file name */
    for(int t=0;t<64;t++) {
        sprintf(clock_jpg_raw_file[t],"/Users/zhtianxiao/Desktop/Reapplying/clock5/clock5_jpg_raw/clock5_%d.raw",t+1);
    }
    
    BIT_FILE *clock_jpg_raw;
    
    /* Shift the image back */
    for(int i=3;i>=-4;i--) {
        for(int j=3;j>=-4;j--) {
            
            
            clock_jpg_raw = NULL;
            clock_jpg_raw = OpenInputBitFile(clock_jpg_raw_file[count++]);
            
            for(int m=0;m<size;m++) {
                for(int n=0;n<size;n++) {
                    clock_n[m][n] = (int)InputBits(clock_jpg_raw, 8);
                }
            }
            
            image_shift(clock_n, clock_shifted, i, j);
            
            for(int m=0;m<size;m++) {
                for(int n=0;n<size;n++) {
                    clock_reapplying[m][n] += clock_shifted[m][n];
                }
            }
            
            
            
        }
    }
    
    /* Average all 64 images and get the result */
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            clock_reapplying[i][j] /= 64;
            mse_clock += pow((double)(clock[i][j]-clock_reapplying[i][j]),2)/(double)SIZE;
        }
    }
    
    psnr_clock = 10*log10(pow(peak_clock,2)/mse_clock);
    
    printf("\n\n");
    printf("The PSNR of clock5 using reapplying method is %f",psnr_clock);
    printf("\n\n");
    
    
    /* Output the the result image */
    BIT_FILE *clock_jpg_raw_output;
    clock_jpg_raw_output = OpenOutputBitFile(clock_jpg_raw_output_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            OutputBits(clock_jpg_raw_output, (unsigned long)clock_reapplying[i][j], 8);
        }
    }
    
    
    return 0;
}

