//
//  main.c
//  Quantization(histogram)
//
//  Created by Tianxiao Zhang on 9/27/17.
//  Copyright © 2017 Tianxiao Zhang. All rights reserved.
//

#include <stdio.h>
#include "bitio.h"
#include "errhand.h"

#define SIZE 65536

char chem_256_file[] = "/Users/zhtianxiao/Desktop/chem.256";
char house_256_file[] = "/Users/zhtianxiao/Desktop/house.256";
char moon_256_file[] = "/Users/zhtianxiao/Desktop/moon.256";

char couple_256_file[] = "/Users/zhtianxiao/Desktop/couple.256";
char elaine_256_file[] = "/Users/zhtianxiao/Desktop/elaine.256";
char f16_256_file[] = "/Users/zhtianxiao/Desktop/f16.256";

char chem_txt_file[] = "/Users/zhtianxiao/Desktop/chem.txt";
char house_txt_file[] = "/Users/zhtianxiao/Desktop/house.txt";
char moon_txt_file[] = "/Users/zhtianxiao/Desktop/moon.txt";

char couple_txt_file[] = "/Users/zhtianxiao/Desktop/couple.txt";
char elaine_txt_file[] = "/Users/zhtianxiao/Desktop/elaine.txt";
char f16_txt_file[] = "/Users/zhtianxiao/Desktop/f16.txt";


int main(int argc, const char * argv[]) {
    
    
    /************************** Start chem.256 **************************/
    // Start generating .txt file after reading it
    int chem[SIZE];
    
    BIT_FILE *chem_256;
    chem_256 = OpenInputBitFile(chem_256_file);
    
    for(int i=0;i<SIZE;i++) {
        chem[i] = (int)InputBits(chem_256, 8);
    }

    BIT_FILE *chem_txt;
    chem_txt = OpenOutputBitFile(chem_txt_file);
    
    for(int j=0;j<SIZE;j++) {
        OutputBits(chem_txt, (unsigned long)chem[j], 8);
    }
    /************************** End chem.256 *****************************/
    
    
    
    /************************** Start house.256 **************************/
    // Start generating .txt file after reading it
    int house[SIZE];
    
    BIT_FILE *house_256;
    house_256 = OpenInputBitFile(house_256_file);
    
    for(int i=0;i<SIZE;i++) {
        house[i] = (int)InputBits(house_256, 8);
    }
   
    
    BIT_FILE *house_txt;
    house_txt = OpenOutputBitFile(house_txt_file);
    
    for(int j=0;j<SIZE;j++) {
        OutputBits(house_txt, (unsigned long)house[j], 8);
    }
    /************************** End house.256 ****************************/
    
    
    
    /************************** Start moon.256 ***************************/
    // Start generating .txt file after reading it
    int moon[SIZE];
    
    BIT_FILE *moon_256;
    moon_256 = OpenInputBitFile(moon_256_file);
    
    for(int i=0;i<SIZE;i++) {
        moon[i] = (int)InputBits(moon_256, 8);
    }

    
    BIT_FILE *moon_txt;
    moon_txt = OpenOutputBitFile(moon_txt_file);
    
    for(int j=0;j<SIZE;j++) {
        OutputBits(moon_txt, (unsigned long)moon[j], 8);
    }
    /*************************** End moon.256 *****************************/
    
    
    
    /************************** Start couple.256 **************************/
    // Start generating .txt file after reading it
    int couple[SIZE];
    
    BIT_FILE *couple_256;
    couple_256 = OpenInputBitFile(couple_256_file);
    
    for(int i=0;i<SIZE;i++) {
        couple[i] = (int)InputBits(couple_256, 8);
    }
    
    
    BIT_FILE *couple_txt;
    couple_txt = OpenOutputBitFile(couple_txt_file);
    
    for(int j=0;j<SIZE;j++) {
        OutputBits(couple_txt, (unsigned long)couple[j], 8);
    }
    /************************** End couple.256 ****************************/
    
    
    
    /************************* Start elaine.256 ***************************/
    // Start generating .txt file after reading it
    int elaine[SIZE];
    
    BIT_FILE *elaine_256;
    elaine_256 = OpenInputBitFile(elaine_256_file);
    
    for(int i=0;i<SIZE;i++) {
        elaine[i] = (int)InputBits(elaine_256, 8);
    }
    
    
    BIT_FILE *elaine_txt;
    elaine_txt = OpenOutputBitFile(elaine_txt_file);
    
    for(int j=0;j<SIZE;j++) {
        OutputBits(elaine_txt, (unsigned long)elaine[j], 8);
    }
    /************************** End elaine.256 ****************************/
    
    
    
    /************************** Start f16.256 *****************************/
    // Start generating .txt file after reading it
    int f16[SIZE];
    
    BIT_FILE *f16_256;
    f16_256 = OpenInputBitFile(f16_256_file);
    
    for(int i=0;i<SIZE;i++) {
        f16[i] = (int)InputBits(f16_256, 8);
    }
    
    
    BIT_FILE *f16_txt;
    f16_txt = OpenOutputBitFile(f16_txt_file);
    
    for(int j=0;j<SIZE;j++) {
        OutputBits(f16_txt, (unsigned long)f16[j], 8);
    }
    /************************** End f16.256 *******************************/
    
    
    return 0;
}
