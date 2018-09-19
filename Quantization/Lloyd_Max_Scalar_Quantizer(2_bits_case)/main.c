//
//  main.c
//  Lloyd_Max_Scalar_Quantizer(2_bits_case)
//
//  Created by Tianxiao Zhang on 9/28/17.
//  Copyright © 2017 Tianxiao Zhang. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bitio.h"
#include "errhand.h"

#define SIZE 65536

char chem_256_file[] = "/Users/zhtianxiao/Desktop/chem.256";
char house_256_file[] = "/Users/zhtianxiao/Desktop/house.256";
char moon_256_file[] = "/Users/zhtianxiao/Desktop/moon.256";

char couple_256_file[] = "/Users/zhtianxiao/Desktop/couple.256";
char elaine_256_file[] = "/Users/zhtianxiao/Desktop/elaine.256";
char f16_256_file[] = "/Users/zhtianxiao/Desktop/f16.256";

char chem_txt_file[] = "/Users/zhtianxiao/Desktop/quantized_chem_2bits.txt";
char house_txt_file[] = "/Users/zhtianxiao/Desktop/quantized_house_2bits.txt";
char moon_txt_file[] = "/Users/zhtianxiao/Desktop/quantized_moon_2bits.txt";

char couple_txt_file[] = "/Users/zhtianxiao/Desktop/quantized_couple_2bits.txt";
char elaine_txt_file[] = "/Users/zhtianxiao/Desktop/quantized_elaine_2bits.txt";
char f16_txt_file[] = "/Users/zhtianxiao/Desktop/quantized_f16_2bits.txt";



/* This function is used to get the maximum value of the image */
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
    
    int combined[SIZE*3];
    
    /************************** Start chem.256 **************************/
    // Start generating .txt file after reading it
    int chem[SIZE];
    
    BIT_FILE *chem_256;
    chem_256 = OpenInputBitFile(chem_256_file);
    
    for(int i=0;i<SIZE;i++) {
        chem[i] = (int)InputBits(chem_256, 8);
        combined[i] = chem[i];
    }
    /************************** End chem.256 *****************************/
    
    
    /************************** Start house.256 **************************/
    // Start generating .txt file after reading it
    int house[SIZE];
    
    BIT_FILE *house_256;
    house_256 = OpenInputBitFile(house_256_file);
    
    for(int i=0;i<SIZE;i++) {
        house[i] = (int)InputBits(house_256, 8);
        combined[SIZE+i] = house[i];
    }
    /************************** End house.256 ****************************/
    
    
    /************************** Start moon.256 ***************************/
    // Start generating .txt file after reading it
    int moon[SIZE];
    
    BIT_FILE *moon_256;
    moon_256 = OpenInputBitFile(moon_256_file);
    
    for(int i=0;i<SIZE;i++) {
        moon[i] = (int)InputBits(moon_256, 8);
        combined[SIZE*2+i] = moon[i];
    }
    /************************** End moon.256 ****************************/

    
    
    /********************************************************************/
    /********************* Start quantizer design ***********************/
    /********************************************************************/
    double quantizer_2[4];
    double old_MSE_2 = 0;
    double new_MSE_2 = 0;
    double average_2[4];
    double mse_2[4];
    double MSE_2;
    double bound_2[5];
    double PSNR_2;
    int buffer_2[4][SIZE*2];
    int count_2[4] = {0};
    int reconstructed_value_2[4];
    int peak_signal;
    int step = SIZE*3 / 4;
    int count[256] = {0};
    
    bound_2[0] = 0;
    bound_2[4] = 255;
    
    for(int i=0;i<SIZE*3;i++) {
        for(int j=0;j<255;j++) {
            if(combined[i] == j) {
                count[j]++;
            }
        }
    }
    
    int x = 0;
    int y = 1;
    for(int i=0;i<256;i++) {
        x += count[i];
        if(x >= y * step) {
            bound_2[y++] = i;
        }
        if(y == 4) {
            break;
        }
    }
    
    
    for(int i=0;i<4;i++) {
        quantizer_2[i] = (bound_2[i]+bound_2[i+1]) / 2;
    }
    
    printf("\n\n");
    printf("The initial bound for each inverval is : \n");
    for(int t=0;t<5;t++) {
        printf("%f ",bound_2[t]);
    }
    printf("\n\n");
    printf("The initial reconstructed value for each inverval is : \n");
    for(int t=0;t<4;t++) {
        printf("%f ",quantizer_2[t]);
    }
    printf("\n\n");
    
    
    
    peak_signal = max_value(combined, SIZE*3);
    
    int index = 0;
    while(1) {
        
        index++;
        
        for(int t=0;t<4;t++) {
            count_2[t] = 0;
        }
        
        for(int i=0;i<SIZE*3;i++) {
            
            for(int m=0;m<4;m++) {
                
                if(combined[i] >= bound_2[m] && combined[i] < bound_2[m+1]) {
                    buffer_2[m][count_2[m]++] = combined[i];
                }
                else
                    continue;
                
            }
            
            
        }

        MSE_2 = 0;
        for(int t=0;t<4;t++) {
            average_2[t] = 0;
            mse_2[t] = 0;
            
        }
        
        for(int i=0;i<4;i++) {
            
            for(int j=0;j<count_2[i];j++) {
                mse_2[i] += pow((quantizer_2[i]-(double)buffer_2[i][j]),2) / (double)(count_2[i]);
                average_2[i] += buffer_2[i][j] / (double)(count_2[i]);
                
            }
            MSE_2 += mse_2[i]/4;
            
        }
        
        PSNR_2 = 10*log10(pow((double)peak_signal,2)/MSE_2);
        
        if(index == 1) {
            printf("\nThe PSNR after %d iteration is %f\n",index,PSNR_2);
        }
        
        if(index > 1) {
            printf("The PSNR after %d iterations is %f\n",index,PSNR_2);
        }
        
        
        for(int t=0;t<4;t++) {
            quantizer_2[t] = average_2[t];
        }
        
        for(int t=1;t<4;t++) {
            bound_2[t] = (quantizer_2[t-1]+quantizer_2[t]) / 2;
        }
        
        
        if(index == 1) {
            old_MSE_2 = MSE_2;
        }
        
        if(index == 2) {
            new_MSE_2 = MSE_2;
            
            if((old_MSE_2-new_MSE_2)/new_MSE_2 < 0.0001 && (old_MSE_2-new_MSE_2)/new_MSE_2 > -0.0001) {
                break;
            }
            
        }
        
        if(index >= 3) {
            
            old_MSE_2 = new_MSE_2;
            new_MSE_2 = MSE_2;
            
            if((old_MSE_2-new_MSE_2)/new_MSE_2 < 0.0001 && (old_MSE_2-new_MSE_2)/new_MSE_2 > -0.0001) {
                break;
            }
            
        }
        
    }
    
    printf("\n\n");
    printf("The iteration times are %d",index);
    
    printf("\n\n");
    printf("The reconstructed value for each interval is : \n");
    for(int i=0;i<4;i++) {
        printf("  %f ",quantizer_2[i]);
    }
    printf("\n\n");
    printf("The bound value for each interval is : \n");
    for(int j=0;j<5;j++) {
        printf("%f  ",bound_2[j]);
    }
    printf("\n\n");
    /******************************************************************************/
    /*************************** End quantizer design *****************************/
    /******************************************************************************/
    
    
    
    /************************** Start reading couple.256 **************************/
    int couple[SIZE];
    
    BIT_FILE *couple_256;
    couple_256 = OpenInputBitFile(couple_256_file);
    
    for(int i=0;i<SIZE;i++) {
        couple[i] = (int)InputBits(couple_256, 8);
    }
    /************************** End reading couple.256 *****************************/
    
    
    /************************** Start reading elaine.256 ***************************/
    int elaine[SIZE];
    
    BIT_FILE *elaine_256;
    elaine_256 = OpenInputBitFile(elaine_256_file);
    
    for(int i=0;i<SIZE;i++) {
        elaine[i] = (int)InputBits(elaine_256, 8);
    }
    /************************** End reading elaine.256 *****************************/
    
    
    /*************************** Start reading f16.256 *****************************/
    int f16[SIZE];
    
    BIT_FILE *f16_256;
    f16_256 = OpenInputBitFile(f16_256_file);
    
    for(int i=0;i<SIZE;i++) {
        f16[i] = (int)InputBits(f16_256, 8);
    }
    /*************************** End reading f16.256 *******************************/
    
    
    // Round the constructed value for each interval to an integer
    for(int i=0;i<4;i++) {
        reconstructed_value_2[i] = (int)round(quantizer_2[i]);
    }
    
    /*******************************************************************************/
    /********************** Start quantizing the image data ************************/
    /*******************************************************************************/
    
    
    /************************ Start quantizing chem.256 ****************************/
    int quantized_chem[SIZE];
    double MSE_chem_2 = 0;
    double PSNR_chem_2;
    BIT_FILE *chem_txt;
    chem_txt = OpenOutputBitFile(chem_txt_file);
    
    for(int i=0;i<SIZE;i++) {
        
        for(int j=0;j<4;j++) {
            if(chem[i] >= bound_2[j] && chem[i] < bound_2[j+1]) {
                MSE_chem_2 += pow((quantizer_2[j]-chem[i]), 2) / (double)SIZE;
                quantized_chem[i] = reconstructed_value_2[j];
                OutputBits(chem_txt, (unsigned long)quantized_chem[i], 8);
            }
            else
                continue;
        }
    }
    
    PSNR_chem_2 = 10*log10(pow((double)max_value(chem, SIZE),2) / MSE_chem_2);
    printf("\nThe PSNR of chem.256 in 2 bits is %f\n",PSNR_chem_2);
    /************************* End quantizing chem.256 *****************************/
    
    
    /*********************** Start quantizing house.256 ****************************/
    int quantized_house[SIZE];
    double MSE_house_2 = 0;
    double PSNR_house_2;
    
    BIT_FILE *house_txt;
    house_txt = OpenOutputBitFile(house_txt_file);
    
    for(int i=0;i<SIZE;i++) {
        
        for(int j=0;j<4;j++) {
            if(house[i] >= bound_2[j] && house[i] < bound_2[j+1]) {
                 MSE_house_2 += pow((quantizer_2[j]-house[i]), 2) / (double)SIZE;
                quantized_house[i] = reconstructed_value_2[j];
                OutputBits(house_txt, (unsigned long)quantized_house[i], 8);
            }
            else
                continue;
        }
    }
    
    PSNR_house_2 = 10*log10(pow((double)max_value(house, SIZE),2) / MSE_house_2);
    printf("\nThe PSNR of house.256 in 2 bits is %f\n",PSNR_house_2);
    /*********************** End quantizing house.256 *****************************/
    
    
    /*********************** Start quantizing moon.256 ****************************/
    int quantized_moon[SIZE];
    double MSE_moon_2 = 0;
    double PSNR_moon_2;
    
    BIT_FILE *moon_txt;
    moon_txt = OpenOutputBitFile(moon_txt_file);
    
    for(int i=0;i<SIZE;i++) {
        
        for(int j=0;j<4;j++) {
            if(moon[i] >= bound_2[j] && moon[i] < bound_2[j+1]) {
                MSE_moon_2 += pow((quantizer_2[j]-moon[i]), 2) / (double)SIZE;
                quantized_moon[i] = reconstructed_value_2[j];
                OutputBits(moon_txt, (unsigned long)quantized_moon[i], 8);
            }
            else
                continue;
        }
    }
    
    PSNR_moon_2 = 10*log10(pow((double)max_value(moon, SIZE),2) / MSE_moon_2);
    printf("\nThe PSNR of moon.256 in 2 bits is %f\n",PSNR_moon_2);
    /************************* End quantizing moon.256 ******************************/
    
    
    /*********************** Start quantizing couple.256 ****************************/
    int quantized_couple[SIZE];
    double MSE_couple_2 = 0;
    double PSNR_couple_2;
    
    BIT_FILE *couple_txt;
    couple_txt = OpenOutputBitFile(couple_txt_file);
    
    for(int i=0;i<SIZE;i++) {
        
        for(int j=0;j<4;j++) {
            if(couple[i] >= bound_2[j] && couple[i] < bound_2[j+1]) {
                MSE_couple_2 += pow((quantizer_2[j]-couple[i]), 2) / (double)SIZE;
                quantized_couple[i] = reconstructed_value_2[j];
                OutputBits(couple_txt, (unsigned long)quantized_couple[i], 8);
            }
            else
                continue;
        }
    }
    
    PSNR_couple_2 = 10*log10(pow((double)max_value(couple, SIZE),2) / MSE_couple_2);
    printf("\nThe PSNR of couple.256 in 2 bits is %f\n",PSNR_couple_2);
    /************************ End quantizing couple.256 *****************************/
    
    
    /*********************** Start quantizing elaine.256 ****************************/
    int quantized_elaine[SIZE];
    double MSE_elaine_2 = 0;
    double PSNR_elaine_2;
    
    BIT_FILE *elaine_txt;
    elaine_txt = OpenOutputBitFile(elaine_txt_file);
    
    for(int i=0;i<SIZE;i++) {
        
        for(int j=0;j<4;j++) {
            if(elaine[i] >= bound_2[j] && elaine[i] < bound_2[j+1]) {
                MSE_elaine_2 += pow((quantizer_2[j]-elaine[i]), 2) / (double)SIZE;
                quantized_elaine[i] = reconstructed_value_2[j];
                OutputBits(elaine_txt, (unsigned long)quantized_elaine[i], 8);
            }
            else
                continue;
        }
    }
    
    PSNR_elaine_2 = 10*log10(pow((double)max_value(elaine, SIZE),2) / MSE_elaine_2);
    printf("\nThe PSNR of elaine.256 in 2 bits is %f\n",PSNR_elaine_2);
    /*********************** End quantizing elaine.256 ****************************/
    
    
    /************************ Start quantizing f16.256 ****************************/
    int quantized_f16[SIZE];
    double MSE_f16_2 = 0;
    double PSNR_f16_2;
    
    BIT_FILE *f16_txt;
    f16_txt = OpenOutputBitFile(f16_txt_file);
    
    for(int i=0;i<SIZE;i++) {
        
        for(int j=0;j<4;j++) {
            if(f16[i] >= bound_2[j] && f16[i] < bound_2[j+1]) {
                MSE_f16_2 += pow((quantizer_2[j]-f16[i]), 2) / (double)SIZE;
                quantized_f16[i] = reconstructed_value_2[j];
                OutputBits(f16_txt, (unsigned long)quantized_f16[i], 8);
            }
            else
                continue;
        }
    }
    
    PSNR_f16_2 = 10*log10(pow((double)max_value(f16, SIZE),2) / MSE_f16_2);
    printf("\nThe PSNR of f16.256 in 2 bits is %f\n",PSNR_f16_2);
    printf("\n\n");
    
    /************************** End quantizing f16.256 **************************/
    
    
    /****************************************************************************/
    /********************** End quantizing the image data ***********************/
    /****************************************************************************/
    return 0;
}

