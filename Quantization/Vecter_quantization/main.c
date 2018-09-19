//
//  main.c
//  Vecter_quantization
//
//  Created by Tianxiao Zhang on 9/29/17.
//  Copyright © 2017 Tianxiao Zhang. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "bitio.h"
#include "errhand.h"


#define SIZE 65536
#define size 256

char chem_256_file[] = "/Users/zhtianxiao/Desktop/chem.256";
char house_256_file[] = "/Users/zhtianxiao/Desktop/house.256";
char moon_256_file[] = "/Users/zhtianxiao/Desktop/moon.256";

char couple_256_file[] = "/Users/zhtianxiao/Desktop/couple.256";
char elaine_256_file[] = "/Users/zhtianxiao/Desktop/elaine.256";
char f16_256_file[] = "/Users/zhtianxiao/Desktop/f16.256";

char block_trainning_file[] = "/Users/zhtianxiao/Desktop/block_trainning_8_8.txt";

char block_chem_256_file[] = "/Users/zhtianxiao/Desktop/block_chem_8_8.txt";
char block_house_256_file[] = "/Users/zhtianxiao/Desktop/block_house_8_8.txt";
char block_moon_256_file[] = "/Users/zhtianxiao/Desktop/block_moon_8_8.txt";


int n;
int block_length; // The length of block for each block row or colume
int block_row_number; // The block number for each image row or colume
int block_total_number; // The total number of blocks
int block_element_num; // The number of each bolck element

void initialize() {
    n = 3;
    block_length = (int)pow(2, n);
    block_row_number = size / block_length;
    block_total_number = SIZE / (int)pow(block_length,2);
    block_element_num = (int)pow(block_length,2);

}


void blocking(int array[size][size],int block_array[block_total_number][block_element_num]) {
    
    int p = 0;
    int q = 0;
    
    for(int i=0;i<block_row_number;i++) {
        for(int j=0;j<block_row_number;j++) {
            
            for(int x=0;x<block_length;x++) {
                for(int y=0;y<block_length;y++) {
                    block_array[p][q++] = array[i*block_length+x][j*block_length+y];
                }
            }
            p++;
            q = 0;
        }
    }

}


int main(int argc, const char * argv[]) {
    
    // Initialize the block parameters
    initialize();
    
    /************* Blocking chem.256 **************/
    int chem[size][size];
    int block_chem[block_total_number][block_element_num];
    
    BIT_FILE *chem_256;
    chem_256 = OpenInputBitFile(chem_256_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            chem[i][j] = (int)InputBits(chem_256, 8);
        }
    }
    
    blocking(chem, block_chem);
    /*********************** End *******************/
    
    
    /************* Blocking house.256 **************/
    int house[size][size];
    int block_house[block_total_number][block_element_num];
    
    BIT_FILE *house_256;
    house_256 = OpenInputBitFile(house_256_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            house[i][j] = (int)InputBits(house_256, 8);
        }
    }
    
    blocking(house, block_house);
    /********************* End ********************/
    
    
    /************* Blocking moon.256 **************/
    int moon[size][size];
    int block_moon[block_total_number][block_element_num];
    
    BIT_FILE *moon_256;
    moon_256 = OpenInputBitFile(moon_256_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            moon[i][j] = (int)InputBits(moon_256, 8);
        }
    }
    
    blocking(moon, block_moon);
    /******************** End *********************/
    
    
    /************* Blocking couple.256 ************/
    int couple[size][size];
    int block_couple[block_total_number][block_element_num];
    
    BIT_FILE *couple_256;
    couple_256 = OpenInputBitFile(couple_256_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            couple[i][j] = (int)InputBits(couple_256, 8);
        }
    }
    
    blocking(couple, block_couple);
    /********************** End *********************/
    
    
    /************* Blocking elaine.256 **************/
    int elaine[size][size];
    int block_elaine[block_total_number][block_element_num];
    
    BIT_FILE *elaine_256;
    elaine_256 = OpenInputBitFile(elaine_256_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            elaine[i][j] = (int)InputBits(elaine_256, 8);
        }
    }
    
    blocking(elaine, block_elaine);
    /********************* End ********************/
    
    
    /************** Blocking f16.256 **************/
    int f16[size][size];
    int block_f16[block_total_number][block_element_num];
    
    BIT_FILE *f16_256;
    f16_256 = OpenInputBitFile(f16_256_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            f16[i][j] = (int)InputBits(f16_256, 8);
        }
    }
    
    blocking(f16, block_f16);
    /********************* End ********************/
    
    
    
    /**********************************************/
    /***************** Test file ******************/
    /**********************************************/
    BIT_FILE *block_chem_txt;
    block_chem_txt = OpenOutputBitFile(block_chem_256_file);
    
    for(int i=0;i<block_total_number;i++) {
        for(int j=0;j<block_element_num;j++) {
            OutputBits(block_chem_txt, (unsigned long)block_chem[i][j], 8);
        }
    }
    
    BIT_FILE *block_house_txt;
    block_house_txt = OpenOutputBitFile(block_house_256_file);
    
    for(int i=0;i<block_total_number;i++) {
        for(int j=0;j<block_element_num;j++) {
            OutputBits(block_house_txt, (unsigned long)block_house[i][j], 8);
        }
    }
    
    BIT_FILE *block_moon_txt;
    block_moon_txt = OpenOutputBitFile(block_moon_256_file);
    
    for(int i=0;i<block_total_number;i++) {
        for(int j=0;j<block_element_num;j++) {
            OutputBits(block_moon_txt, (unsigned long)block_moon[i][j], 8);
        }
    }
    
    

    /***********************************************/
    /*************** Trainning file ****************/
    /***********************************************/
    BIT_FILE *block_trainning_txt;
    block_trainning_txt = OpenOutputBitFile(block_trainning_file);
    
    int vq_trainning[SIZE*3];
    int vq_count = 0;
    for(int i=0;i<block_total_number;i++) {
        for(int j=0;j<block_element_num;j++) {
            vq_trainning[vq_count] = block_couple[i][j];
            OutputBits(block_trainning_txt, (unsigned long)vq_trainning[vq_count], 8);
            vq_count++;
        }
    }
    
    for(int i=0;i<block_total_number;i++) {
        for(int j=0;j<block_element_num;j++) {
            vq_trainning[vq_count] = block_elaine[i][j];
            OutputBits(block_trainning_txt, (unsigned long)vq_trainning[vq_count], 8);
            vq_count++;
        }
    }
    
    for(int i=0;i<block_total_number;i++) {
        for(int j=0;j<block_element_num;j++) {
            vq_trainning[vq_count] = block_f16[i][j];
            OutputBits(block_trainning_txt, (unsigned long)vq_trainning[vq_count], 8);
            vq_count++;
        }
    }
    
    return 0;
}
