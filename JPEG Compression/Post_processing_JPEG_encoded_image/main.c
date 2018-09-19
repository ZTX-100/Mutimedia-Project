//
//  main.c
//  Post_processing_JPEG_encoded_image
//
//  Created by Tianxiao Zhang on 10/3/17.
//  Copyright © 2017 Tianxiao Zhang. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include "bitio.h"
#include "errhand.h"

#define SIZE 65536

char clock_file[] = "/Users/zhtianxiao/Desktop/raw_image/clock.raw";
char clock1_file[] = "/Users/zhtianxiao/Desktop/raw_image/clock1.raw";
char clock2_file[] = "/Users/zhtianxiao/Desktop/raw_image/clock2.raw";
char clock3_file[] = "/Users/zhtianxiao/Desktop/raw_image/clock3.raw";
char clock4_file[] = "/Users/zhtianxiao/Desktop/raw_image/clock4.raw";
char clock5_file[] = "/Users/zhtianxiao/Desktop/raw_image/clock5.raw";

char pepper_file[] = "/Users/zhtianxiao/Desktop/raw_image/pepper.raw";
char pepper1_file[] = "/Users/zhtianxiao/Desktop/raw_image/pepper1.raw";
char pepper2_file[] = "/Users/zhtianxiao/Desktop/raw_image/pepper2.raw";
char pepper3_file[] = "/Users/zhtianxiao/Desktop/raw_image/pepper3.raw";
char pepper4_file[] = "/Users/zhtianxiao/Desktop/raw_image/pepper4.raw";
char pepper5_file[] = "/Users/zhtianxiao/Desktop/raw_image/pepper5.raw";


int main(int argc, const char * argv[]) {
  
    /***************************************************************************/
    /****************************** Begin clock ********************************/
    /***************************************************************************/
    double peak_clock = 255;
    double mse_clock;
    double psnr_clock;
    int clock[SIZE];
    int clock_n[SIZE];
    
    BIT_FILE *clock_raw;
    clock_raw = OpenInputBitFile(clock_file);
    
    for(int i=0;i<SIZE;i++) {
        clock[i] = (int)InputBits(clock_raw, 8);
    }
    
    
    /************************************/
    /************** clock1 **************/
    /************************************/
    BIT_FILE *clock1_raw;
    clock1_raw = OpenInputBitFile(clock1_file);
    
    mse_clock = 0;
    for(int i=0;i<SIZE;i++) {
        clock_n[i] = (int)InputBits(clock1_raw, 8);
        mse_clock += pow((double)(clock[i]-clock_n[i]), 2)/(double)SIZE;
    }
    
    psnr_clock = 10*log10(pow(peak_clock, 2)/mse_clock);
    
    printf("\n\n");
    printf("The PSNR of clock1.raw is %f\n\n",psnr_clock);
    
    /************************************/
    
    /************************************/
    /************** clock2 **************/
    /************************************/
    BIT_FILE *clock2_raw;
    clock2_raw = OpenInputBitFile(clock2_file);
    
    mse_clock = 0;
    for(int i=0;i<SIZE;i++) {
        clock_n[i] = (int)InputBits(clock2_raw, 8);
        mse_clock += pow((double)(clock[i]-clock_n[i]), 2)/(double)SIZE;
    }
    
    psnr_clock = 10*log10(pow(peak_clock, 2)/mse_clock);
    
    printf("\n\n");
    printf("The PSNR of clock2.raw is %f\n\n",psnr_clock);
    
    /************************************/
    
    
    /************************************/
    /************** clock3 **************/
    /************************************/
    BIT_FILE *clock3_raw;
    clock3_raw = OpenInputBitFile(clock3_file);
    
    mse_clock = 0;
    for(int i=0;i<SIZE;i++) {
        clock_n[i] = (int)InputBits(clock3_raw, 8);
        mse_clock += pow((double)(clock[i]-clock_n[i]), 2)/(double)SIZE;
    }
    
    psnr_clock = 10*log10(pow(peak_clock, 2)/mse_clock);
    
    printf("\n\n");
    printf("The PSNR of clock3.raw is %f\n\n",psnr_clock);
    
    /************************************/
    
    
    /************************************/
    /************** clock4 **************/
    /************************************/
    BIT_FILE *clock4_raw;
    clock4_raw = OpenInputBitFile(clock4_file);
    
    mse_clock = 0;
    for(int i=0;i<SIZE;i++) {
        clock_n[i] = (int)InputBits(clock4_raw, 8);
        mse_clock += pow((double)(clock[i]-clock_n[i]), 2)/(double)SIZE;
    }
    
    psnr_clock = 10*log10(pow(peak_clock, 2)/mse_clock);
    
    printf("\n\n");
    printf("The PSNR of clock4.raw is %f\n\n",psnr_clock);
    
    /************************************/
    
    
    /************************************/
    /************** clock5 **************/
    /************************************/
    BIT_FILE *clock5_raw;
    clock5_raw = OpenInputBitFile(clock5_file);
    
    mse_clock = 0;
    for(int i=0;i<SIZE;i++) {
        clock_n[i] = (int)InputBits(clock5_raw, 8);
        mse_clock += pow((double)(clock[i]-clock_n[i]), 2)/(double)SIZE;
    }
    
    psnr_clock = 10*log10(pow(peak_clock, 2)/mse_clock);
    
    printf("\n\n");
    printf("The PSNR of clock5.raw is %f\n\n",psnr_clock);
    
    /************************************/
    
    
    /**************************************************************************/
    /******************************* End clock ********************************/
    /**************************************************************************/
    
    
    
    
    
    /**************************************************************************/
    /***************************** Begin pepper *******************************/
    /**************************************************************************/
    
    double peak_pepper = 255;
    double mse_pepper = 0;
    double psnr_pepper = 0;
    int pepper[SIZE][3];
    int pepper_n[SIZE][3];
    
    BIT_FILE *pepper_raw;
    pepper_raw = OpenInputBitFile(pepper_file);
    
    for(int i=0;i<SIZE;i++) {
        for(int j=0;j<3;j++) {
        pepper[i][j] = (int)InputBits(pepper_raw, 8);
        }
    }
    
    /*************************************/
    /************** pepper1 **************/
    /*************************************/
    BIT_FILE *pepper1_raw;
    pepper1_raw = OpenInputBitFile(pepper1_file);
    
    mse_pepper = 0;
    for(int i=0;i<SIZE;i++) {
        for(int j=0;j<3;j++) {
        pepper_n[i][j] = (int)InputBits(pepper1_raw, 8);
        mse_pepper += pow((double)(pepper[i][j]-pepper_n[i][j]), 2)/(double)(SIZE*3);
        }
    }
    
    psnr_pepper = 10*log10(pow(peak_pepper, 2)/mse_pepper);
    
    printf("\n\n");
    printf("The PSNR of pepper1.raw is %f\n\n",psnr_pepper);
    
    /*************************************/
    
   
    /*************************************/
    /************** pepper2 **************/
    /*************************************/
    BIT_FILE *pepper2_raw;
    pepper2_raw = OpenInputBitFile(pepper2_file);
    
    mse_pepper = 0;
    for(int i=0;i<SIZE;i++) {
        for(int j=0;j<3;j++) {
            pepper_n[i][j] = (int)InputBits(pepper2_raw, 8);
            mse_pepper += pow((double)(pepper[i][j]-pepper_n[i][j]), 2)/(double)(SIZE*3);
        }
    }
    
    psnr_pepper = 10*log10(pow(peak_pepper, 2)/mse_pepper);
    
    printf("\n\n");
    printf("The PSNR of pepper2.raw is %f\n\n",psnr_pepper);
    
    /*************************************/
    
    
    /*************************************/
    /************** pepper3 **************/
    /*************************************/
    BIT_FILE *pepper3_raw;
    pepper3_raw = OpenInputBitFile(pepper3_file);
    
    mse_pepper = 0;
    for(int i=0;i<SIZE;i++) {
        for(int j=0;j<3;j++) {
            pepper_n[i][j] = (int)InputBits(pepper3_raw, 8);
            mse_pepper += pow((double)(pepper[i][j]-pepper_n[i][j]), 2)/(double)(SIZE*3);
        }
    }
    
    psnr_pepper = 10*log10(pow(peak_pepper, 2)/mse_pepper);
    
    printf("\n\n");
    printf("The PSNR of pepper3.raw is %f\n\n",psnr_pepper);
    
    /*************************************/
    
    
    /*************************************/
    /************** pepper4 **************/
    /*************************************/
    BIT_FILE *pepper4_raw;
    pepper4_raw = OpenInputBitFile(pepper4_file);
    
    mse_pepper = 0;
    for(int i=0;i<SIZE;i++) {
        for(int j=0;j<3;j++) {
            pepper_n[i][j] = (int)InputBits(pepper4_raw, 8);
            mse_pepper += pow((double)(pepper[i][j]-pepper_n[i][j]), 2)/(double)(SIZE*3);
        }
    }
    
    psnr_pepper = 10*log10(pow(peak_pepper, 2)/mse_pepper);
    
    printf("\n\n");
    printf("The PSNR of pepper4.raw is %f\n\n",psnr_pepper);
    
    /*************************************/
    
    
    /*************************************/
    /************** pepper1 **************/
    /*************************************/
    BIT_FILE *pepper5_raw;
    pepper5_raw = OpenInputBitFile(pepper5_file);
    
    mse_pepper = 0;
    for(int i=0;i<SIZE;i++) {
        for(int j=0;j<3;j++) {
            pepper_n[i][j] = (int)InputBits(pepper5_raw, 8);
            mse_pepper += pow((double)(pepper[i][j]-pepper_n[i][j]), 2)/(double)(SIZE*3);
        }
    }
    
    psnr_pepper = 10*log10(pow(peak_pepper, 2)/mse_pepper);
    
    printf("\n\n");
    printf("The PSNR of pepper5.raw is %f\n\n",psnr_pepper);
    
    /*************************************/
    
    
    /***************************************************************************/
    /******************************* End pepper ********************************/
    /***************************************************************************/
    
    
    return 0;
}
