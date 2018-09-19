//
//  main.c
//  Post_Processing_JPEG_Encoder_Images(intuitive_trial)
//
//  Created by Tianxiao Zhang on 10/4/17.
//  Copyright © 2017 Tianxiao Zhang. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bitio.h"
#include "errhand.h"

#define size 256
#define SIZE 65536

char clock1_raw_file[] = "/Users/zhtianxiao/Desktop/raw_image/clock1.raw";
char clock2_raw_file[] = "/Users/zhtianxiao/Desktop/raw_image/clock2.raw";
char clock3_raw_file[] = "/Users/zhtianxiao/Desktop/raw_image/clock3.raw";
char clock4_raw_file[] = "/Users/zhtianxiao/Desktop/raw_image/clock4.raw";
char clock5_raw_file[] = "/Users/zhtianxiao/Desktop/raw_image/clock5.raw";
char clock_raw_file[] = "/Users/zhtianxiao/Desktop/raw_image/clock.raw";

char pepper1_raw_file[] = "/Users/zhtianxiao/Desktop/raw_image/pepper1.raw";
char pepper2_raw_file[] = "/Users/zhtianxiao/Desktop/raw_image/pepper2.raw";
char pepper3_raw_file[] = "/Users/zhtianxiao/Desktop/raw_image/pepper3.raw";
char pepper4_raw_file[] = "/Users/zhtianxiao/Desktop/raw_image/pepper4.raw";
char pepper5_raw_file[] = "/Users/zhtianxiao/Desktop/raw_image/pepper5.raw";
char pepper_raw_file[] = "/Users/zhtianxiao/Desktop/raw_image/pepper.raw";

char clock1_mean_raw_file[] = "/Users/zhtianxiao/Desktop/clock1_mean.raw";
char clock2_mean_raw_file[] = "/Users/zhtianxiao/Desktop/clock2_mean.raw";
char clock3_mean_raw_file[] = "/Users/zhtianxiao/Desktop/clock3_mean.raw";
char clock4_mean_raw_file[] = "/Users/zhtianxiao/Desktop/clock4_mean.raw";
char clock5_mean_raw_file[] = "/Users/zhtianxiao/Desktop/clock5_mean.raw";

char pepper1_mean_raw_file[] = "/Users/zhtianxiao/Desktop/pepper1_mean.raw";
char pepper2_mean_raw_file[] = "/Users/zhtianxiao/Desktop/pepper2_mean.raw";
char pepper3_mean_raw_file[] = "/Users/zhtianxiao/Desktop/pepper3_mean.raw";
char pepper4_mean_raw_file[] = "/Users/zhtianxiao/Desktop/pepper4_mean.raw";
char pepper5_mean_raw_file[] = "/Users/zhtianxiao/Desktop/pepper5_mean.raw";

/* The function calculates the mean of the 9 pixels around the block boundary pixels
 and set the mean value to the block boundary pixels to smooth the block boundary */
void mean(int image[size][size]) {
    
    
    int v1[size];
    int v2[size];
    
    for(int p=0;p<size;p++) {
        v1[p] = 0;
        v2[p] = 0;
    }
    
    for(int i=1;i<32;i++) {
        
        for(int j=0;j<size;j++) {
            
            if(j == 0) {
                v1[j] = (image[i*8-2][j]+image[i*8-2][j+1]+image[i*8-1][j]+image[i*8-1][j+1]+image[i*8][j]+image[i*8][j+1])/6;
                v2[j] = (image[i*8-1][j]+image[i*8-1][j+1]+image[i*8][j]+image[i*8][j+1]+image[i*8+1][j]+image[i*8+1][j+1])/6;
            }
            
            if(j == size-1) {
                v1[j] = (image[i*8-2][j-1]+image[i*8-2][j]+image[i*8-1][j-1]+image[i*8-1][j]+image[i*8][j-1]+image[i*8][j])/6;
                v2[j] = (image[i*8-1][j-1]+image[i*8-1][j]+image[i*8][j-1]+image[i*8][j]+image[i*8+1][j-1]+image[i*8+1][j])/6;
            }
            
            else {
                for(int m=-1;m<2;m++) {
                    for(int n=-1;n<2;n++) {
                        v1[j] += image[i*8-1+m][j+n];
                        v2[j] += image[i*8+m][j+n];
                    }
                }
                v1[j] = v1[j]/9;
                v2[j] = v2[j]/9;
            }
            
        }
        
        for(int q=0;q<size;q++) {
            image[i*8-1][q] = v1[q];
            image[i*8][q] = v2[q];
            v1[q] = 0;
            v2[q] = 0;
        }
        
    }
    
    
    for(int p=0;p<size;p++) {
        v1[p] = 0;
        v2[p] = 0;
    }
    
    for(int i=1;i<32;i++) {
        
        for(int j=0;j<size;j++) {
            
            if(j == 0) {
                v1[j] = (image[j][i*8-2]+image[j+1][i*8-2]+image[j][i*8-1]+image[j+1][i*8-1]+image[j][i*8]+image[j+1][i*8])/6;
                v2[j] = (image[j][i*8-1]+image[j+1][i*8-1]+image[j][i*8]+image[j+1][i*8]+image[j][i*8+1]+image[j+1][i*8+1])/6;
            }
            
            if(j == size-1) {
                v1[j] = (image[j-1][i*8-2]+image[j][i*8-2]+image[j-1][i*8-1]+image[j][i*8-1]+image[j-1][i*8]+image[j][i*8])/6;
                v2[j] = (image[j-1][i*8-1]+image[j][i*8-1]+image[j-1][i*8]+image[j][i*8]+image[j-1][i*8+1]+image[j][i*8+1])/6;
            }
            
            else {
                for(int m=-1;m<2;m++) {
                    for(int n=-1;n<2;n++) {
                        v1[j] += image[j+n][i*8-1+m];
                        v2[j] += image[j+n][i*8+m];
                    }
                }
                v1[j] = v1[j]/9;
                v2[j] = v2[j]/9;
            }
            
        }
        
        for(int q=0;q<size;q++) {
            image[q][i*8-1] = v1[q];
            image[q][i*8] = v2[q];
            v1[q] = 0;
            v2[q] = 0;
        }
        
    }
    
    
}




int main(int argc, const char * argv[]) {
    
    /*************************************************************************/
    /****************************** Begin clock ******************************/
    /*************************************************************************/
    double mse_clock;
    double psnr_clock;
    double peak_clock = 255;
    int clock[size][size];
    int clock_n[size][size];
    BIT_FILE *clock_raw;
    clock_raw = OpenInputBitFile(clock_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            clock[i][j] = (int)InputBits(clock_raw, 8);
        }
    }
    
    /****************************************/
    /**************** clock1 ****************/
    /****************************************/
    BIT_FILE *clock1_raw;
    clock1_raw = OpenInputBitFile(clock1_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            clock_n[i][j] = (int)InputBits(clock1_raw, 8);
        }
    }
    
    mean(clock_n);
    
    BIT_FILE *clock1_mean;
    clock1_mean = OpenOutputBitFile(clock1_mean_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            OutputBits(clock1_mean, (unsigned long)clock_n[i][j], 8);
        }
    }
 
    /* PSNR calculation */
    mse_clock = 0;
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            mse_clock += pow((double)(clock[i][j]-clock_n[i][j]),2) / (double)SIZE;
        }
    }
    
    psnr_clock = 10*log10(pow(peak_clock,2)/mse_clock);
    printf("\n\n");
    printf("The PSNR for compressed image clock1.raw is %f\n\n",psnr_clock);
    /****************************************/
    
    
    /****************************************/
    /**************** clock2 ****************/
    /****************************************/
    BIT_FILE *clock2_raw;
    clock2_raw = OpenInputBitFile(clock2_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            clock_n[i][j] = (int)InputBits(clock2_raw, 8);
        }
    }
    
    mean(clock_n);
    
    BIT_FILE *clock2_mean;
    clock2_mean = OpenOutputBitFile(clock2_mean_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            OutputBits(clock2_mean, (unsigned long)clock_n[i][j], 8);
        }
    }
    
    /* PSNR calculation */
    mse_clock = 0;
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            mse_clock += pow((double)(clock[i][j]-clock_n[i][j]),2) / (double)SIZE;
        }
    }
    
    psnr_clock = 10*log10(pow(peak_clock,2)/mse_clock);
    printf("\n\n");
    printf("The PSNR for compressed image clock2.raw is %f\n\n",psnr_clock);
    /****************************************/
    
    
    /****************************************/
    /**************** clock3 ****************/
    /****************************************/
    BIT_FILE *clock3_raw;
    clock3_raw = OpenInputBitFile(clock3_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            clock_n[i][j] = (int)InputBits(clock3_raw, 8);
        }
    }
    
    mean(clock_n);
    
    BIT_FILE *clock3_mean;
    clock3_mean = OpenOutputBitFile(clock3_mean_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            OutputBits(clock3_mean, (unsigned long)clock_n[i][j], 8);
        }
    }
    
    /* PSNR calculation */
    mse_clock = 0;
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            mse_clock += pow((double)(clock[i][j]-clock_n[i][j]),2) / (double)SIZE;
        }
    }
    
    psnr_clock = 10*log10(pow(peak_clock,2)/mse_clock);
    printf("\n\n");
    printf("The PSNR for compressed image clock3.raw is %f\n\n",psnr_clock);
    /****************************************/
    
    
    /****************************************/
    /**************** clock4 ****************/
    /****************************************/
    BIT_FILE *clock4_raw;
    clock4_raw = OpenInputBitFile(clock4_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            clock_n[i][j] = (int)InputBits(clock4_raw, 8);
        }
    }
    
    mean(clock_n);
    
    BIT_FILE *clock4_mean;
    clock4_mean = OpenOutputBitFile(clock4_mean_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            OutputBits(clock4_mean, (unsigned long)clock_n[i][j], 8);
        }
    }
    
    /* PSNR calculation */
    mse_clock = 0;
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            mse_clock += pow((double)(clock[i][j]-clock_n[i][j]),2) / (double)SIZE;
        }
    }
    
    psnr_clock = 10*log10(pow(peak_clock,2)/mse_clock);
    printf("\n\n");
    printf("The PSNR for compressed image clock4.raw is %f\n\n",psnr_clock);
    /****************************************/
    
    
    /****************************************/
    /**************** clock5 ****************/
    /****************************************/
    BIT_FILE *clock5_raw;
    clock5_raw = OpenInputBitFile(clock5_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            clock_n[i][j] = (int)InputBits(clock5_raw, 8);
        }
    }
    
    mean(clock_n);
    
    BIT_FILE *clock5_mean;
    clock5_mean = OpenOutputBitFile(clock5_mean_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            OutputBits(clock5_mean, (unsigned long)clock_n[i][j], 8);
        }
    }
    
    /* PSNR calculation */
    mse_clock = 0;
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            mse_clock += pow((double)(clock[i][j]-clock_n[i][j]),2) / (double)SIZE;
        }
    }
    
    psnr_clock = 10*log10(pow(peak_clock,2)/mse_clock);
    printf("\n\n");
    printf("The PSNR for compressed image clock5.raw is %f\n\n",psnr_clock);
    /****************************************/
    
    /*************************************************************************/
    /****************************** End clock ********************************/
    /*************************************************************************/
    
    
    
    /*************************************************************************/
    /***************************** Begin pepper ******************************/
    /*************************************************************************/
    double mse_pepper;
    double psnr_pepper;
    double peak_pepper = 255;
    int pepper1[size][size];
    int pepper2[size][size];
    int pepper3[size][size];
    int pepper[size][size][3];
    int pepper_n[size][size][3];
    BIT_FILE *pepper_raw;
    pepper_raw = OpenInputBitFile(pepper_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
            pepper[i][j][k] = (int)InputBits(pepper_raw, 8);
            }
        }
    }
    
    /****************************************/
    /**************** pepper1 ***************/
    /****************************************/
    BIT_FILE *pepper1_raw;
    pepper1_raw = OpenInputBitFile(pepper1_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
            pepper_n[i][j][k] = (int)InputBits(pepper1_raw, 8);
            }
        }
    }
    
    for(int p=0;p<size;p++) {
        for(int q=0;q<size;q++) {
            pepper1[p][q] = pepper_n[p][q][0];
            pepper2[p][q] = pepper_n[p][q][1];
            pepper3[p][q] = pepper_n[p][q][2];
        }
    }

    mean(pepper1);
    mean(pepper2);
    mean(pepper3);
    
    for(int p=0;p<size;p++) {
        for(int q=0;q<size;q++) {
            pepper_n[p][q][0] = pepper1[p][q];
            pepper_n[p][q][1] = pepper2[p][q];
            pepper_n[p][q][2] = pepper3[p][q];
        }
    }
    
    
    BIT_FILE *pepper1_mean;
    pepper1_mean = OpenOutputBitFile(pepper1_mean_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
            OutputBits(pepper1_mean, (unsigned long)pepper_n[i][j][k], 8);
            }
        }
    }
    
    /* PSNR calculation */
    mse_pepper = 0;
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
            mse_pepper += pow((double)(pepper[i][j][k]-pepper_n[i][j][k]),2) / (double)(SIZE*3);
            }
        }
    }
    
    psnr_pepper = 10*log10(pow(peak_pepper,2)/mse_pepper);
    printf("\n\n");
    printf("The PSNR for compressed image pepper1.raw is %f\n\n",psnr_pepper);
    /*****************************************/
    
    
    /****************************************/
    /**************** pepper2 ***************/
    /****************************************/
    BIT_FILE *pepper2_raw;
    pepper2_raw = OpenInputBitFile(pepper2_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
                pepper_n[i][j][k] = (int)InputBits(pepper2_raw, 8);
            }
        }
    }
    
    for(int p=0;p<size;p++) {
        for(int q=0;q<size;q++) {
            pepper1[p][q] = pepper_n[p][q][0];
            pepper2[p][q] = pepper_n[p][q][1];
            pepper3[p][q] = pepper_n[p][q][2];
        }
    }
    
    mean(pepper1);
    mean(pepper2);
    mean(pepper3);
    
    for(int p=0;p<size;p++) {
        for(int q=0;q<size;q++) {
            pepper_n[p][q][0] = pepper1[p][q];
            pepper_n[p][q][1] = pepper2[p][q];
            pepper_n[p][q][2] = pepper3[p][q];
        }
    }
    
    
    BIT_FILE *pepper2_mean;
    pepper2_mean = OpenOutputBitFile(pepper2_mean_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
                OutputBits(pepper2_mean, (unsigned long)pepper_n[i][j][k], 8);
            }
        }
    }
    
    /* PSNR calculation */
    mse_pepper = 0;
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
                mse_pepper += pow((double)(pepper[i][j][k]-pepper_n[i][j][k]),2) / (double)(SIZE*3);
            }
        }
    }
    
    psnr_pepper = 10*log10(pow(peak_pepper,2)/mse_pepper);
    printf("\n\n");
    printf("The PSNR for compressed image pepper2.raw is %f\n\n",psnr_pepper);
    /*****************************************/
    
    
    /****************************************/
    /**************** pepper3 ***************/
    /****************************************/
    BIT_FILE *pepper3_raw;
    pepper3_raw = OpenInputBitFile(pepper3_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
                pepper_n[i][j][k] = (int)InputBits(pepper3_raw, 8);
            }
        }
    }
    
    for(int p=0;p<size;p++) {
        for(int q=0;q<size;q++) {
            pepper1[p][q] = pepper_n[p][q][0];
            pepper2[p][q] = pepper_n[p][q][1];
            pepper3[p][q] = pepper_n[p][q][2];
        }
    }
    
    mean(pepper1);
    mean(pepper2);
    mean(pepper3);
    
    for(int p=0;p<size;p++) {
        for(int q=0;q<size;q++) {
            pepper_n[p][q][0] = pepper1[p][q];
            pepper_n[p][q][1] = pepper2[p][q];
            pepper_n[p][q][2] = pepper3[p][q];
        }
    }
    
    
    BIT_FILE *pepper3_mean;
    pepper3_mean = OpenOutputBitFile(pepper3_mean_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
                OutputBits(pepper3_mean, (unsigned long)pepper_n[i][j][k], 8);
            }
        }
    }
    
    /* PSNR calculation */
    mse_pepper = 0;
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
                mse_pepper += pow((double)(pepper[i][j][k]-pepper_n[i][j][k]),2) / (double)(SIZE*3);
            }
        }
    }
    
    psnr_pepper = 10*log10(pow(peak_pepper,2)/mse_pepper);
    printf("\n\n");
    printf("The PSNR for compressed image pepper3.raw is %f\n\n",psnr_pepper);
    /*****************************************/
    
    
    /****************************************/
    /**************** pepper4 ***************/
    /****************************************/
    BIT_FILE *pepper4_raw;
    pepper4_raw = OpenInputBitFile(pepper4_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
                pepper_n[i][j][k] = (int)InputBits(pepper4_raw, 8);
            }
        }
    }
    
    for(int p=0;p<size;p++) {
        for(int q=0;q<size;q++) {
            pepper1[p][q] = pepper_n[p][q][0];
            pepper2[p][q] = pepper_n[p][q][1];
            pepper3[p][q] = pepper_n[p][q][2];
        }
    }
    
    mean(pepper1);
    mean(pepper2);
    mean(pepper3);
    
    for(int p=0;p<size;p++) {
        for(int q=0;q<size;q++) {
            pepper_n[p][q][0] = pepper1[p][q];
            pepper_n[p][q][1] = pepper2[p][q];
            pepper_n[p][q][2] = pepper3[p][q];
        }
    }
    
    
    BIT_FILE *pepper4_mean;
    pepper4_mean = OpenOutputBitFile(pepper4_mean_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
                OutputBits(pepper4_mean, (unsigned long)pepper_n[i][j][k], 8);
            }
        }
    }
    
    /* PSNR calculation */
    mse_pepper = 0;
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
                mse_pepper += pow((double)(pepper[i][j][k]-pepper_n[i][j][k]),2) / (double)(SIZE*3);
            }
        }
    }
    
    psnr_pepper = 10*log10(pow(peak_pepper,2)/mse_pepper);
    printf("\n\n");
    printf("The PSNR for compressed image pepper4.raw is %f\n\n",psnr_pepper);
    /*****************************************/
    
    
    /****************************************/
    /**************** pepper5 ***************/
    /****************************************/
    BIT_FILE *pepper5_raw;
    pepper5_raw = OpenInputBitFile(pepper5_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
                pepper_n[i][j][k] = (int)InputBits(pepper5_raw, 8);
            }
        }
    }
    
    for(int p=0;p<size;p++) {
        for(int q=0;q<size;q++) {
            pepper1[p][q] = pepper_n[p][q][0];
            pepper2[p][q] = pepper_n[p][q][1];
            pepper3[p][q] = pepper_n[p][q][2];
        }
    }
    
    mean(pepper1);
    mean(pepper2);
    mean(pepper3);
    
    for(int p=0;p<size;p++) {
        for(int q=0;q<size;q++) {
            pepper_n[p][q][0] = pepper1[p][q];
            pepper_n[p][q][1] = pepper2[p][q];
            pepper_n[p][q][2] = pepper3[p][q];
        }
    }
    
    
    BIT_FILE *pepper5_mean;
    pepper5_mean = OpenOutputBitFile(pepper5_mean_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
                OutputBits(pepper5_mean, (unsigned long)pepper_n[i][j][k], 8);
            }
        }
    }
    
    /* PSNR calculation */
    mse_pepper = 0;
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
                mse_pepper += pow((double)(pepper[i][j][k]-pepper_n[i][j][k]),2) / (double)(SIZE*3);
            }
        }
    }
    
    psnr_pepper = 10*log10(pow(peak_pepper,2)/mse_pepper);
    printf("\n\n");
    printf("The PSNR for compressed image pepper5.raw is %f\n\n",psnr_pepper);
    /*****************************************/
    
    
    
    /*************************************************************************/
    /****************************** End pepper *******************************/
    /*************************************************************************/
    
    return 0;
}




