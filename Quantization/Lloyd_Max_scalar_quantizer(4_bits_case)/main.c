//
//  main.c
//  Lloyd_Max_scalar_quantizer(4_bits_case)
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

char chem_txt_file[] = "/Users/zhtianxiao/Desktop/quantized_chem_4bits.txt";
char house_txt_file[] = "/Users/zhtianxiao/Desktop/quantized_house_4bits.txt";
char moon_txt_file[] = "/Users/zhtianxiao/Desktop/quantized_moon_4bits.txt";

char couple_txt_file[] = "/Users/zhtianxiao/Desktop/quantized_couple_4bits.txt";
char elaine_txt_file[] = "/Users/zhtianxiao/Desktop/quantized_elaine_4bits.txt";
char f16_txt_file[] = "/Users/zhtianxiao/Desktop/quantized_f16_4bits.txt";

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
    
    /************************** Start reading chem.256 **************************/
    int chem[SIZE];
    
    BIT_FILE *chem_256;
    chem_256 = OpenInputBitFile(chem_256_file);
    
    for(int i=0;i<SIZE;i++) {
        chem[i] = (int)InputBits(chem_256, 8);
        combined[i] = chem[i];
    }
    /************************** End reading chem.256 ****************************/
    
    
    /************************* Start reading house.256 **************************/
    int house[SIZE];
    
    BIT_FILE *house_256;
    house_256 = OpenInputBitFile(house_256_file);
    
    for(int i=0;i<SIZE;i++) {
        house[i] = (int)InputBits(house_256, 8);
        combined[SIZE+i] = house[i];
    }
    
    /************************* End reading house.256 ****************************/
    
    
    /************************** Start reading moon.256 **************************/
    int moon[SIZE];
    
    BIT_FILE *moon_256;
    moon_256 = OpenInputBitFile(moon_256_file);
    
    for(int i=0;i<SIZE;i++) {
        moon[i] = (int)InputBits(moon_256, 8);
        combined[SIZE*2+i] = moon[i];
    }
    /************************** End reading moon.256 ****************************/
    
    
    
    /****************************************************************************/
    /************************* Start quantizer design ***************************/
    /****************************************************************************/
    double quantizer_4[16];
    double old_MSE_4 = 0;
    double new_MSE_4 = 0;
    double average_4[16];
    double mse_4[16];
    double MSE_4;
    double bound_4[17];
    double PSNR_4;
    int buffer_4[16][SIZE];
    int count_4[16] = {0};
    int reconstructed_value_4[16];
    int peak_signal;
    int step = SIZE*3 / 16;
    int count[256] = {0};

    bound_4[0] = 0;
    bound_4[16] = 255;
    
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
            bound_4[y++] = i;
        }
        if(y == 16) {
            break;
        }
    }
    
    
    for(int i=0;i<16;i++) {
        quantizer_4[i] = (bound_4[i]+bound_4[i+1]) / 2;
    }
    
    printf("\n\n");
    printf("The initial bound for each inverval is : \n");
    for(int t=0;t<17;t++) {
        printf("%f ",bound_4[t]);
    }
    printf("\n\n");
    printf("The initial reconstructed value for each inverval is : \n");
    for(int t=0;t<16;t++) {
        printf("%f ",quantizer_4[t]);
    }
    printf("\n\n");
    
    peak_signal = max_value(combined, SIZE*3);
    
    int index = 0;
    while(1) {
        
        index++;
        
        for(int t=0;t<16;t++) {
            count_4[t] = 0;
        }
        
        for(int i=0;i<SIZE*3;i++) {
            
            for(int m=0;m<16;m++) {
                
                if(combined[i] >= bound_4[m] && combined[i] < bound_4[m+1]) {
                    buffer_4[m][count_4[m]++] = combined[i];
                }
                else
                    continue;
                
            }
            
            
        }
        
        MSE_4 = 0;
        for(int t=0;t<16;t++) {
            average_4[t] = 0;
            mse_4[t] = 0;
            
        }
        
        for(int i=0;i<16;i++) {
            
            for(int j=0;j<count_4[i];j++) {
                mse_4[i] += pow((quantizer_4[i]-(double)buffer_4[i][j]),2) / (double)(count_4[i]);
                average_4[i] += buffer_4[i][j] / (double)(count_4[i]);
                
            }
            MSE_4 += mse_4[i]/16;
            
        }
        
        PSNR_4 = 10*log10(pow((double)peak_signal,2)/MSE_4);
        
        if(index == 1) {
            printf("\nThe PSNR after %d iteration is %f\n",index,PSNR_4);
        }
        
        if(index > 1) {
            printf("The PSNR after %d iterations is %f\n",index,PSNR_4);
        }
        
        
        for(int t=0;t<4;t++) {
            quantizer_4[t] = average_4[t];
        }
        
        for(int t=1;t<16;t++) {
            bound_4[t] = (quantizer_4[t-1]+quantizer_4[t]) / 2;
        }
        
        
        if(index == 1) {
            old_MSE_4 = MSE_4;
        }
        
        if(index == 2) {
            new_MSE_4 = MSE_4;
            
            if((old_MSE_4-new_MSE_4)/new_MSE_4 < 0.0001 && (old_MSE_4-new_MSE_4)/new_MSE_4 > -0.0001) {
                break;
            }
            
        }
        
        if(index >= 3) {
            
            old_MSE_4 = new_MSE_4;
            new_MSE_4 = MSE_4;
            
            if((old_MSE_4-new_MSE_4)/new_MSE_4 < 0.0001 && (old_MSE_4-new_MSE_4)/new_MSE_4 > -0.0001) {
                break;
            }
            
        }
        
    }
    
    printf("\n\n");
    printf("The iteration times are %d",index);
    
    printf("\n\n");
    printf("The reconstructed value for each interval is : \n");
    for(int i=0;i<16;i++) {
        printf("  %f ",quantizer_4[i]);
    }
    printf("\n\n");
    printf("The bound value for each interval is : \n");
    for(int j=0;j<17;j++) {
        printf("%f  ",bound_4[j]);
    }
    printf("\n\n");
    printf("\n\n");
    /******************************************************************************/
    /**************************** End quantizer design ****************************/
    /******************************************************************************/
    
    
    
    /************************** Start reading couple.256 **************************/
    int couple[SIZE];
    
    BIT_FILE *couple_256;
    couple_256 = OpenInputBitFile(couple_256_file);
    
    for(int i=0;i<SIZE;i++) {
        couple[i] = (int)InputBits(couple_256, 8);
    }
    /************************* End reading couple.256 *****************************/
    
    
    /************************** Start reading elaine.256 **************************/
    int elaine[SIZE];
    
    BIT_FILE *elaine_256;
    elaine_256 = OpenInputBitFile(elaine_256_file);
    
    for(int i=0;i<SIZE;i++) {
        elaine[i] = (int)InputBits(elaine_256, 8);
    }
    /************************* End reading elaine.256 *****************************/
    
    
    /************************** Start reading f16.256 *****************************/
    int f16[SIZE];
    
    BIT_FILE *f16_256;
    f16_256 = OpenInputBitFile(f16_256_file);
    
    for(int i=0;i<SIZE;i++) {
        f16[i] = (int)InputBits(f16_256, 8);
    }
    /************************** End reading f16.256 *******************************/
    
    
    
    // Round the constructed value for each interval to an integer
    for(int i=0;i<16;i++) {
        reconstructed_value_4[i] = (int)round(quantizer_4[i]);
    }
    
    
    /*******************************************************************************/
    /********************** Start quantizing the image data ************************/
    /*******************************************************************************/
    
    
    /************************ Start quantizing chem.256 ****************************/
    int quantized_chem[SIZE];
    double MSE_chem_4 = 0;
    double PSNR_chem_4;
    
    BIT_FILE *chem_txt;
    chem_txt = OpenOutputBitFile(chem_txt_file);
    
    for(int i=0;i<SIZE;i++) {
        
        for(int j=0;j<16;j++) {
            if(chem[i] >= bound_4[j] && chem[i] < bound_4[j+1]) {
                MSE_chem_4 += pow((quantizer_4[j]-chem[i]), 2) / (double)SIZE;
                quantized_chem[i] = reconstructed_value_4[j];
                OutputBits(chem_txt, (unsigned long)quantized_chem[i], 8);
            }
            else
                continue;
        }
    }
    
    PSNR_chem_4 = 10*log10(pow((double)max_value(chem, SIZE),2) / MSE_chem_4);
    printf("\nThe PSNR of chem.256 in 4 bits is %f\n",PSNR_chem_4);
    /************************* End quantizing chem.256 ******************************/
    
    
    /************************ Start quantizing house.256 ****************************/
    int quantized_house[SIZE];
    double MSE_house_4 = 0;
    double PSNR_house_4;
    
    BIT_FILE *house_txt;
    house_txt = OpenOutputBitFile(house_txt_file);
    
    for(int i=0;i<SIZE;i++) {
        
        for(int j=0;j<16;j++) {
            if(house[i] >= bound_4[j] && house[i] < bound_4[j+1]) {
                MSE_house_4 += pow((quantizer_4[j]-house[i]), 2) / (double)SIZE;
                quantized_house[i] = reconstructed_value_4[j];
                OutputBits(house_txt, (unsigned long)quantized_house[i], 8);
            }
            else
                continue;
        }
    }
    
    PSNR_house_4 = 10*log10(pow((double)max_value(house, SIZE),2) / MSE_house_4);
    printf("\nThe PSNR of house.256 in 4 bits is %f\n",PSNR_house_4);
    /************************ End quantizing house.256 *****************************/
    
    
    /************************ Start quantizing moon.256 ****************************/
    int quantized_moon[SIZE];
    double MSE_moon_4 = 0;
    double PSNR_moon_4;
    
    BIT_FILE *moon_txt;
    moon_txt = OpenOutputBitFile(moon_txt_file);
    
    for(int i=0;i<SIZE;i++) {
        
        for(int j=0;j<16;j++) {
            if(moon[i] >= bound_4[j] && moon[i] < bound_4[j+1]) {
                MSE_moon_4 += pow((quantizer_4[j]-moon[i]), 2) / (double)SIZE;
                quantized_moon[i] = reconstructed_value_4[j];
                OutputBits(moon_txt, (unsigned long)quantized_moon[i], 8);
            }
            else
                continue;
        }
    }
    
    PSNR_moon_4 = 10*log10(pow((double)max_value(moon, SIZE),2) / MSE_moon_4);
    printf("\nThe PSNR of moon.256 in 4 bits is %f\n",PSNR_moon_4);
    /************************* End quantizing moon.256 ******************************/
    
    
    /*********************** Start quantizing couple.256 ****************************/
    int quantized_couple[SIZE];
    double MSE_couple_4 = 0;
    double PSNR_couple_4;
    
    BIT_FILE *couple_txt;
    couple_txt = OpenOutputBitFile(couple_txt_file);
    
    for(int i=0;i<SIZE;i++) {
        
        for(int j=0;j<16;j++) {
            if(couple[i] >= bound_4[j] && couple[i] < bound_4[j+1]) {
                MSE_couple_4 += pow((quantizer_4[j]-couple[i]), 2) / (double)SIZE;
                quantized_couple[i] = reconstructed_value_4[j];
                OutputBits(couple_txt, (unsigned long)quantized_couple[i], 8);
            }
            else
                continue;
        }
    }
    
    PSNR_couple_4 = 10*log10(pow((double)max_value(couple, SIZE),2) / MSE_couple_4);
    printf("\nThe PSNR of couple.256 in 4 bits is %f\n",PSNR_couple_4);
    /************************ End quantizing couple.256 *****************************/
    
    
    /*********************** Start quantizing elaine.256 ****************************/
    int quantized_elaine[SIZE];
    double MSE_elaine_4 = 0;
    double PSNR_elaine_4;
    
    BIT_FILE *elaine_txt;
    elaine_txt = OpenOutputBitFile(elaine_txt_file);
    
    for(int i=0;i<SIZE;i++) {
        
        for(int j=0;j<16;j++) {
            if(elaine[i] >= bound_4[j] && elaine[i] < bound_4[j+1]) {
                MSE_elaine_4 += pow((quantizer_4[j]-elaine[i]), 2) / (double)SIZE;
                quantized_elaine[i] = reconstructed_value_4[j];
                OutputBits(elaine_txt, (unsigned long)quantized_elaine[i], 8);
            }
            else
                continue;
        }
    }
    
    PSNR_elaine_4 = 10*log10(pow((double)max_value(elaine, SIZE),2) / MSE_elaine_4);
    printf("\nThe PSNR of elaine.256 in 4 bits is %f\n",PSNR_elaine_4);
    /************************ End quantizing elaine.256 ****************************/
    
    
    /************************* Start quantizing f16.256 ****************************/
    int quantized_f16[SIZE];
    double MSE_f16_4 = 0;
    double PSNR_f16_4;
    
    BIT_FILE *f16_txt;
    f16_txt = OpenOutputBitFile(f16_txt_file);
    
    for(int i=0;i<SIZE;i++) {
        
        for(int j=0;j<16;j++) {
            if(f16[i] >= bound_4[j] && f16[i] < bound_4[j+1]) {
                MSE_f16_4 += pow((quantizer_4[j]-f16[i]), 2) / (double)SIZE;
                quantized_f16[i] = reconstructed_value_4[j];
                OutputBits(f16_txt, (unsigned long)quantized_f16[i], 8);
            }
            else
                continue;
        }
    }
    
    PSNR_f16_4 = 10*log10(pow((double)max_value(f16, SIZE),2) / MSE_f16_4);
    printf("\nThe PSNR of f16.256 in 4 bits is %f\n",PSNR_f16_4);
    printf("\n\n");
    /*************************** End quantizing f16.256 **************************/
    
    
    /*****************************************************************************/
    /*********************** End quantizing the image data ***********************/
    /*****************************************************************************/
    
    
    /*****************************************************************************/
    /***************** Start calculating theotetical entropy code ****************/
    /*****************************************************************************/
    
    
    /*********************************/
    /*********** chem.256 ************/
    /*********************************/
    int freq_chem[256] = {0};
    double entropy_chem = 0;
    double pr_chem;
    
    for(int i=0;i<SIZE;i++) {
        freq_chem[chem[i]]++;
    }
    
    for(int j=0;j<256;j++) {
        if(freq_chem[j] == 0) {
            continue;
        }
        else {
            pr_chem = (double)freq_chem[j] / (double)SIZE;
            entropy_chem += -(pr_chem*log2(pr_chem));
            
        }
    }
    
    printf("The entropy of chem.256 is %f\n",entropy_chem);
    
    
    /*********************************/
    /*********** house.256 ***********/
    /*********************************/
    int freq_house[256] = {0};
    double entropy_house = 0;
    double pr_house;
    
    for(int i=0;i<SIZE;i++) {
        freq_house[house[i]]++;
    }
    
    for(int j=0;j<256;j++) {
        if(freq_house[j] == 0) {
            continue;
        }
        else {
            pr_house = (double)freq_house[j] / (double)SIZE;
            entropy_house += -(pr_house*log2(pr_house));
            
        }
    }
    
    printf("The entropy of house.256 is %f\n",entropy_house);
    
    
    /*********************************/
    /*********** moon.256 ************/
    /*********************************/
    int freq_moon[256] = {0};
    double entropy_moon = 0;
    double pr_moon;
    
    for(int i=0;i<SIZE;i++) {
        freq_moon[moon[i]]++;
    }
    
    for(int j=0;j<256;j++) {
        if(freq_moon[j] == 0) {
            continue;
        }
        else {
            pr_moon = (double)freq_moon[j] / (double)SIZE;
            entropy_moon += -(pr_moon*log2(pr_moon));
            
        }
    }
    
    printf("The entropy of moon.256 is %f\n",entropy_moon);
    
    
    /*********************************/
    /********** couple.256 ***********/
    /*********************************/
    int freq_couple[256] = {0};
    double entropy_couple = 0;
    double pr_couple;
    
    for(int i=0;i<SIZE;i++) {
        freq_couple[couple[i]]++;
    }
    
    for(int j=0;j<256;j++) {
        if(freq_couple[j] == 0) {
            continue;
        }
        else {
            pr_couple = (double)freq_couple[j] / (double)SIZE;
            entropy_couple += -(pr_couple*log2(pr_couple));
            
        }
    }
    
    printf("The entropy of couple.256 is %f\n",entropy_couple);
    
    
    /*********************************/
    /********** elaine.256 ***********/
    /*********************************/
    int freq_elaine[256] = {0};
    double entropy_elaine = 0;
    double pr_elaine;
    
    for(int i=0;i<SIZE;i++) {
        freq_elaine[elaine[i]]++;
    }
    
    for(int j=0;j<256;j++) {
        if(freq_elaine[j] == 0) {
            continue;
        }
        else {
            pr_elaine = (double)freq_elaine[j] / (double)SIZE;
            entropy_elaine += -(pr_elaine*log2(pr_elaine));
            
        }
    }
    
    printf("The entropy of elaine.256 is %f\n",entropy_elaine);
    
    
    /*********************************/
    /************ f16.256 ************/
    /*********************************/
    int freq_f16[256] = {0};
    double entropy_f16 = 0;
    double pr_f16;
    
    for(int i=0;i<SIZE;i++) {
        freq_f16[f16[i]]++;
    }
    
    for(int j=0;j<256;j++) {
        if(freq_f16[j] == 0) {
            continue;
        }
        else {
            pr_f16 = (double)freq_f16[j] / (double)SIZE;
            entropy_f16 += -(pr_f16*log2(pr_f16));
            
        }
    }
    
    printf("The entropy of f16.256 is %f\n",entropy_f16);
    printf("\n\n");
    
    /**********************************************************************************/
    /******************* End calculating theotetical entropy code *********************/
    /**********************************************************************************/
    return 0;
}


