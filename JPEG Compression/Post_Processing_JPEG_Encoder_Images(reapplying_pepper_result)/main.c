//
//  main.c
//  Post_Processing_JPEG_Encoder_Images(reapplying_pepper1_result)
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

char pepper_raw_file[] = "/Users/zhtianxiao/Desktop/Reapplying/raw_image/pepper.raw";
char pepper_jpg_raw_output_file[] = "/Users/zhtianxiao/Desktop/pepper5_reapplying.raw";

char pepper_jpg_raw_file[64][100];

/* The function is to shift images */
void image_shift(int image[size][size][3], int shifted_image[size][size][3], int i, int j) {
    
    for(int m=0;m<size;m++) {
        for(int n=0;n<size;n++) {
            for(int p=0;p<3;p++) {
            shifted_image[m][n][p] = image[m][n][p];
            }
        }
    }
    
    /* If shift to upper */
    if(i >= 0) {
        
        /* If shift to upper right */
        if(j >= 0) {
            
            for(int m=0;m<size-i;m++) {
                for(int n=0;n<size-j;n++) {
                    for(int p=0;p<3;p++) {
                    shifted_image[m][n+j][p] = image[m+i][n][p];
                    }
                }
            }
            
        }
        
        /* If shift to upper left */
        if(j < 0) {
            
            for(int m=0;m<size-i;m++) {
                for(int n=0;n<size+j;n++) {
                    for(int p=0;p<3;p++) {
                    shifted_image[m][n][p] = image[m+i][n-j][p];
                    }
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
                    for(int p=0;p<3;p++) {
                    shifted_image[m-i][n+j][p] = image[m][n][p];
                    }
                }
            }
            
        }
        
        /* If shift to lower left */
        if(j < 0) {
            
            for(int m=0;m<size+i;m++) {
                for(int n=0;n<size+j;n++) {
                    for(int p=0;p<3;p++) {
                    shifted_image[m-i][n][p] = image[m][n-j][p];
                    }
                }
            }
            
        }
        
        
    }
    
    
    
}



int main(int argc, const char * argv[]) {
    
    int pepper_reapplying[size][size][3];
    int pepper_n[size][size][3];
    int pepper_shifted[size][size][3];
    int pepper[size][size][3];
    int count = 0;
    
    double mse_pepper = 0;
    double psnr_pepper = 0;
    double peak_pepper = 255;
    
    BIT_FILE *pepper_raw;
    pepper_raw = OpenInputBitFile(pepper_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
            pepper_reapplying[i][j][k] = 0;
            pepper[i][j][k] = (int)InputBits(pepper_raw, 8);
            }
        }
    }
    
    /* Using loop to print all 64 file names */
    for(int t=0;t<64;t++) {
        sprintf(pepper_jpg_raw_file[t],"/Users/zhtianxiao/Desktop/Reapplying/pepper5/pepper5_jpg_raw/pepper5_%d.raw",t+1);
    }
    
    
    
    BIT_FILE *pepper_jpg_raw;
    
    /* Shift images back */
    for(int i=3;i>=-4;i--) {
        for(int j=3;j>=-4;j--) {
            
            pepper_jpg_raw = NULL;
            pepper_jpg_raw = OpenInputBitFile(pepper_jpg_raw_file[count++]);
            
            for(int m=0;m<size;m++) {
                for(int n=0;n<size;n++) {
                    for(int p=0;p<3;p++) {
                    pepper_n[m][n][p] = (int)InputBits(pepper_jpg_raw, 8);
                    }
                }
            }
            
            image_shift(pepper_n, pepper_shifted, i, j);
            
            for(int m=0;m<size;m++) {
                for(int n=0;n<size;n++) {
                    for(int p=0;p<3;p++) {
                    pepper_reapplying[m][n][p] += pepper_shifted[m][n][p];
                    }
                }
            }
            
            
            
        }
    }
    
    /* Average the images and get the result */
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
            pepper_reapplying[i][j][k] /= 64;
            mse_pepper += pow((double)(pepper[i][j][k]-pepper_reapplying[i][j][k]),2)/(double)(SIZE*3);
            }
        }
    }
    
    psnr_pepper = 10*log10(pow(peak_pepper,2)/mse_pepper);
    
    printf("\n\n");
    printf("The PSNR of pepper5 using reapplying method is %f",psnr_pepper);
    printf("\n\n");
    
    
    /* Output the result image */
    BIT_FILE *pepper_jpg_raw_output;
    pepper_jpg_raw_output = OpenOutputBitFile(pepper_jpg_raw_output_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
            OutputBits(pepper_jpg_raw_output, (unsigned long)pepper_reapplying[i][j][k], 8);
            }
        }
    }
    
    
    return 0;
}


