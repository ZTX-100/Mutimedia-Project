//
//  main.c
//  Post_Processing_JPEG_Encoder_Images(deblocking)
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

char clock1_deblocking_raw_file[] = "/Users/zhtianxiao/Desktop/clock1_deblocking.raw";
char clock2_deblocking_raw_file[] = "/Users/zhtianxiao/Desktop/clock2_deblocking.raw";
char clock3_deblocking_raw_file[] = "/Users/zhtianxiao/Desktop/clock3_deblocking.raw";
char clock4_deblocking_raw_file[] = "/Users/zhtianxiao/Desktop/clock4_deblocking.raw";
char clock5_deblocking_raw_file[] = "/Users/zhtianxiao/Desktop/clock5_deblocking.raw";

char pepper1_raw_file[] = "/Users/zhtianxiao/Desktop/raw_image/pepper1.raw";
char pepper2_raw_file[] = "/Users/zhtianxiao/Desktop/raw_image/pepper2.raw";
char pepper3_raw_file[] = "/Users/zhtianxiao/Desktop/raw_image/pepper3.raw";
char pepper4_raw_file[] = "/Users/zhtianxiao/Desktop/raw_image/pepper4.raw";
char pepper5_raw_file[] = "/Users/zhtianxiao/Desktop/raw_image/pepper5.raw";
char pepper_raw_file[] = "/Users/zhtianxiao/Desktop/raw_image/pepper.raw";

char pepper1_deblocking_raw_file[] = "/Users/zhtianxiao/Desktop/pepper1_deblocking.raw";
char pepper2_deblocking_raw_file[] = "/Users/zhtianxiao/Desktop/pepper2_deblocking.raw";
char pepper3_deblocking_raw_file[] = "/Users/zhtianxiao/Desktop/pepper3_deblocking.raw";
char pepper4_deblocking_raw_file[] = "/Users/zhtianxiao/Desktop/pepper4_deblocking.raw";
char pepper5_deblocking_raw_file[] = "/Users/zhtianxiao/Desktop/pepper5_deblocking.raw";


int Q_50[8][8] = {
    16,  11,  10,  16,  24,  40,  51,  61,
    12,  12,  14,  19,  26,  58,  60,  55,
    14,  13,  16,  24,  40,  57,  69,  56,
    14,  17,  22,  29,  51,  87,  80,  62,
    18,  22,  37,  56,  68,  109, 103, 77,
    24,  35,  55,  64,  81,  104, 113, 92,
    49,  64,  78,  87,  103, 121, 120, 101,
    72,  92,  95,  98,  112, 100, 103, 99
};


/* The function is to find the maximum pixel values */
int max(int array[],int num) {
    
    int max_value = 0;
    for(int i=0;i<num;i++) {
        if(array[i] > max_value) {
            max_value = array[i];
        }
    }
    return max_value;
}



/* The function is to find the minimum value of an array */
int min(int array[],int num) {
    
    int min_value = array[0];
    for(int i=0;i<num;i++) {
        if(array[i] < min_value) {
            min_value = array[i];
        }
    }
    return min_value;
}



/* The function is to the minimum value of an array, but the data type is double */
double min_float(double array[],int num) {
    
    double min_value = array[0];
    for(int i=0;i<num;i++) {
        if(array[i] < min_value) {
            min_value = array[i];
        }
    }
    return min_value;
}



/* We clip x to the range (0,q) or (q,0).
 If x has the same sign with q but it does not in the range of (0,q) or (q,0),
 we devided x by 2 until x in the range of (0,q) or (q,0).
 If x has different sign with q, first we make x = -x and then do the same thing above */
int clip(double x, int q) {
    int d;
    double t = x;
    
    if(q < 0 && t < 0) {
        while(t < q) {
            t = 0.5*t;
        }
    }
    
    if(q > 0 && t < 0) {
        t = -x;
        while(t > q) {
            t = 0.5*t;
        }
    }
    
    if(q < 0 && t >= 0) {
        t = -x;
        while(t < q) {
            t = 0.5*t;
        }
    }
    
    if(q > 0 && t >= 0) {
        while(t > q) {
            t = 0.5*t;
        }
    }
    
    if(q == 0) {
        t = 0;
    }
    
    d = (int)t;
    return d;
}


/* The function is used to deblock the "blocking effect" at the
 boundary of each block */
void deblocking(int image[size][size]) {
    
    int v[10];
    int v_new[10];
    int p[17];
    int b[9] = {1,1,2,2,4,2,2,1,1};
    int max_v;
    int min_v;
    int d;
    int QP = 30;
    int T1 = 15;
    int T2 = 4;
    int F_v = 0;
    int index = 0;
    double a_3[3];
    double a_3_1;
    double k1 = 0.6533;
    double k3 = 0.2706;
    
    for(int i=1;i<32;i++) {
        
        for(int j=0;j<size;j++) {
            
            F_v = 0;
            index = j % 8;
            QP = Q_50[0][index];
            
            for(int m=0;m<10;m++) {
                v[m] = image[i*8-6+m][j];
            }
            
            max_v = max(v, 8);
            min_v = min(v, 8);
            
            for(int k=0;k<3;k++) {
                a_3[k] = k3*v[2*k+1]-k1*v[2*k+2]+k1*v[2*k+3]-k3*v[2*k+4];
            }
            
            for(int n=0;n<9;n++) {
                if(abs(v[n]-v[n+1]) <= T1) {
                    F_v++;
                }
            }
            
            if(F_v >= T2) {
                if((max_v-min_v)<2*QP) {
                    if(abs(v[1]-v[0]) < QP) {
                        for(int t=1;t<5;t++) {
                            p[t] = v[0];
                        }
                    }
                    else {
                        for(int t=1;t<5;t++) {
                            p[t] = v[1];
                        }
                    }
                    
                    if(abs(v[9]-v[8]) < QP) {
                        for(int t=13;t<17;t++) {
                            p[t] = v[9];
                        }
                    }
                    else {
                        for(int t=13;t<17;t++) {
                            p[t] = v[8];
                        }
                    }
                    
                    for(int t=1;t<9;t++) {
                        p[t+4] = v[t];
                    }
                    
                    int buff = 0;
                    for(int m=1;m<9;m++) {
                        for(int n=0;n<9;n++) {
                            buff += b[n]*p[m+n];
                        }
                        v[m] = buff/16;
                        buff = 0;
                    }
                }
                
                else {
                    for(int t=0;t<10;t++) {
                        v_new[t] = v[t];
                    }
                }
                
            }
            
            else {
                if(a_3[1] < QP && a_3[1] > -QP) {
                    double buffer[3];
                    for(int t=0;t<3;t++) {
                        if(a_3[t] < 0) {
                            buffer[t] = -a_3[t];
                        }
                        else
                            buffer[t] = a_3[t];
                    }
                    
                    if(buffer[1] != 0) {
                        a_3_1 = a_3[1]*min_float(buffer, 3)/buffer[1];
                    }
                    else
                        a_3_1 = 0;
                    
                    d = clip(k1*(a_3_1-a_3[1]), (v[4]-v[5])/2);
                    v_new[4] = v[4] - d;
                    v_new[5] = v[5] + d;
                    
                    
                }
                
                else {
                    for(int t=0;t<10;t++) {
                        v_new[t] = v[t];
                    }
                }
                
            }
            
            for(int m=0;m<10;m++) {
                image[i*8-6+m][j] = v[m];
            }
            
            
        }
        
    }
    
    
    
    for(int i=1;i<32;i++) {
        
        for(int j=0;j<size;j++) {
            
            index = j % 8;
            F_v = 0;
            QP = Q_50[index][0];
            
            for(int m=0;m<10;m++) {
                v[m] = image[j][i*8-6+m];
            }
            
            max_v = max(v, 8);
            min_v = min(v, 8);
            
            for(int k=0;k<3;k++) {
                a_3[k] = k3*v[2*k+1]-k1*v[2*k+2]+k1*v[2*k+3]-k3*v[2*k+4];
            }
            
            for(int n=0;n<9;n++) {
                if(abs(v[n]-v[n+1]) <= T1) {
                    F_v++;
                }
            }
            
            if(F_v >= T2) {
                if((max_v-min_v)<2*QP) {
                    if(abs(v[1]-v[0]) < QP) {
                        for(int t=1;t<5;t++) {
                            p[t] = v[0];
                        }
                    }
                    else {
                        for(int t=1;t<5;t++) {
                            p[t] = v[1];
                        }
                    }
                    
                    if(abs(v[9]-v[8]) < QP) {
                        for(int t=13;t<17;t++) {
                            p[t] = v[9];
                        }
                    }
                    else {
                        for(int t=13;t<17;t++) {
                            p[t] = v[8];
                        }
                    }
                    
                    for(int t=1;t<9;t++) {
                        p[t+4] = v[t];
                    }
                    
                    int buff = 0;
                    for(int m=1;m<9;m++) {
                        for(int n=0;n<9;n++) {
                            buff += b[n]*p[m+n];
                        }
                        v[m] = buff/16;
                        buff = 0;
                    }
                }
                
                else {
                    for(int t=0;t<10;t++) {
                        v_new[t] = v[t];
                    }
                }
                
            }
            
            else {
                if(a_3[1] < QP && a_3[1] > -QP) {
                    double buffer[3];
                    for(int t=0;t<3;t++) {
                        if(a_3[t] < 0) {
                            buffer[t] = -a_3[t];
                        }
                        else
                            buffer[t] = a_3[t];
                    }
                    
                    if(buffer[1] != 0) {
                        a_3_1 = a_3[1]*min_float(buffer, 3)/buffer[1];
                    }
                    else
                        a_3_1 = 0;
                    
                    d = clip(k1*(a_3_1-a_3[1]), (v[4]-v[5])/2);
                    v_new[4] = v[4] - d;
                    v_new[5] = v[5] + d;
                    
                    
                }
                
                else {
                    for(int t=0;t<10;t++) {
                        v_new[t] = v[t];
                    }
                }
                
            }
            
            for(int m=0;m<10;m++) {
                image[j][i*8-6+m] = v[m];
            }
            
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
    
    deblocking(clock_n);
    
    BIT_FILE *clock1_deblocking;
    clock1_deblocking = OpenOutputBitFile(clock1_deblocking_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            OutputBits(clock1_deblocking, (unsigned long)clock_n[i][j], 8);
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
    
    deblocking(clock_n);
    
    BIT_FILE *clock2_deblocking;
    clock2_deblocking = OpenOutputBitFile(clock2_deblocking_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            OutputBits(clock2_deblocking, (unsigned long)clock_n[i][j], 8);
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
    
    deblocking(clock_n);
    
    BIT_FILE *clock3_deblocking;
    clock3_deblocking = OpenOutputBitFile(clock3_deblocking_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            OutputBits(clock3_deblocking, (unsigned long)clock_n[i][j], 8);
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
    
    deblocking(clock_n);
    
    BIT_FILE *clock4_deblocking;
    clock4_deblocking = OpenOutputBitFile(clock4_deblocking_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            OutputBits(clock4_deblocking, (unsigned long)clock_n[i][j], 8);
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
    
    deblocking(clock_n);
    
    BIT_FILE *clock5_deblocking;
    clock5_deblocking = OpenOutputBitFile(clock5_deblocking_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            OutputBits(clock5_deblocking, (unsigned long)clock_n[i][j], 8);
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
    
    deblocking(pepper1);
    deblocking(pepper2);
    deblocking(pepper3);
    
    for(int p=0;p<size;p++) {
        for(int q=0;q<size;q++) {
            pepper_n[p][q][0] = pepper1[p][q];
            pepper_n[p][q][1] = pepper2[p][q];
            pepper_n[p][q][2] = pepper3[p][q];
        }
    }
    
    
    BIT_FILE *pepper1_deblocking;
    pepper1_deblocking = OpenOutputBitFile(pepper1_deblocking_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
                OutputBits(pepper1_deblocking, (unsigned long)pepper_n[i][j][k], 8);
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
    
    deblocking(pepper1);
    deblocking(pepper2);
    deblocking(pepper3);
    
    for(int p=0;p<size;p++) {
        for(int q=0;q<size;q++) {
            pepper_n[p][q][0] = pepper1[p][q];
            pepper_n[p][q][1] = pepper2[p][q];
            pepper_n[p][q][2] = pepper3[p][q];
        }
    }
    
    
    BIT_FILE *pepper2_deblocking;
    pepper2_deblocking = OpenOutputBitFile(pepper2_deblocking_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
                OutputBits(pepper2_deblocking, (unsigned long)pepper_n[i][j][k], 8);
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
    
    deblocking(pepper1);
    deblocking(pepper2);
    deblocking(pepper3);
    
    for(int p=0;p<size;p++) {
        for(int q=0;q<size;q++) {
            pepper_n[p][q][0] = pepper1[p][q];
            pepper_n[p][q][1] = pepper2[p][q];
            pepper_n[p][q][2] = pepper3[p][q];
        }
    }
    
    
    BIT_FILE *pepper3_deblocking;
    pepper3_deblocking = OpenOutputBitFile(pepper3_deblocking_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
                OutputBits(pepper3_deblocking, (unsigned long)pepper_n[i][j][k], 8);
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
    
    deblocking(pepper1);
    deblocking(pepper2);
    deblocking(pepper3);
    
    for(int p=0;p<size;p++) {
        for(int q=0;q<size;q++) {
            pepper_n[p][q][0] = pepper1[p][q];
            pepper_n[p][q][1] = pepper2[p][q];
            pepper_n[p][q][2] = pepper3[p][q];
        }
    }
    
    
    BIT_FILE *pepper4_deblocking;
    pepper4_deblocking = OpenOutputBitFile(pepper4_deblocking_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
                OutputBits(pepper4_deblocking, (unsigned long)pepper_n[i][j][k], 8);
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
    
    deblocking(pepper1);
    deblocking(pepper2);
    deblocking(pepper3);
    
    for(int p=0;p<size;p++) {
        for(int q=0;q<size;q++) {
            pepper_n[p][q][0] = pepper1[p][q];
            pepper_n[p][q][1] = pepper2[p][q];
            pepper_n[p][q][2] = pepper3[p][q];
        }
    }
    
    
    BIT_FILE *pepper5_deblocking;
    pepper5_deblocking = OpenOutputBitFile(pepper5_deblocking_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
                OutputBits(pepper5_deblocking, (unsigned long)pepper_n[i][j][k], 8);
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



