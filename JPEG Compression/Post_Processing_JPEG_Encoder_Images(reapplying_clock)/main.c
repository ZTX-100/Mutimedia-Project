//
//  main.c
//  Post_Processing_JPEG_Encoder_Images(reapplying_clock1)
//
//  Created by Tianxiao Zhang on 10/5/17.
//  Copyright © 2017 Tianxiao Zhang. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bitio.h"
#include "errhand.h"

#define size 256

char clock_raw_file[] = "/Users/zhtianxiao/Desktop/Reapplying/raw_image/clock5.raw";

char clock_raw_shift_file[64][100];

int main(int argc, const char * argv[]) {
    
    int clock_n[size][size];
    BIT_FILE *clock_raw;
    clock_raw = OpenInputBitFile(clock_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            clock_n[i][j] = (int)InputBits(clock_raw, 8);
        }
    }
    
    /* Using loop to print all 64 file name */
    for(int t=0;t<64;t++) {
        sprintf(clock_raw_shift_file[t],"/Users/zhtianxiao/Desktop/Reapplying/clock5/clock5_raw/clock5_%d.raw",t+1);
    }
    
    BIT_FILE *clock_raw_shift;
    
    int clock_shift[size][size];
    
    int count = 0;
    
    /* using loop to get shifted file */
    for(int i=-3;i<=4;i++) {
        
        for(int j=-3;j<=4;j++) {
            
            
            for(int m=0;m<size;m++) {
                for(int n=0;n<size;n++) {
                    clock_shift[m][n] = clock_n[m][n];
                }
            }
            
            /* If shift to upper */
            if(i >= 0) {
                
                /* If shift to upper right */
                if(j >= 0) {
                    
                    for(int m=0;m<size-i;m++) {
                        for(int n=0;n<size-j;n++) {
                            clock_shift[m][n+j] = clock_n[m+i][n];
                        }
                    }
                    
                }
                
                /* If shift to upper left */
                if(j < 0) {
                    
                    for(int m=0;m<size-i;m++) {
                        for(int n=0;n<size+j;n++) {
                            clock_shift[m][n] = clock_n[m+i][n-j];
                        }
                    }
                    
                }
                
                
                
            }
            
            /* If shift to lower */
            if(i < 0) {
                
                /* If shift lower right */
                if(j >= 0) {
                    
                    for(int m=0;m<size+i;m++) {
                        for(int n=0;n<size-j;n++) {
                            clock_shift[m-i][n+j] = clock_n[m][n];
                        }
                    }
                    
                }
                
                /* If shift to lower left */
                if(j < 0) {
                    
                    for(int m=0;m<size+i;m++) {
                        for(int n=0;n<size+j;n++) {
                            clock_shift[m-i][n] = clock_n[m][n-j];
                        }
                    }
                    
                }
                
                
            }
            
            /* output each shifted image */
            clock_raw_shift =  NULL;
            clock_raw_shift = OpenOutputBitFile(clock_raw_shift_file[count++]);
            for(int m=0;m<size;m++) {
                for(int n=0;n<size;n++) {
                    OutputBits(clock_raw_shift, (unsigned long)clock_shift[m][n], 8);
                }
            }
            
            
        }
        
    }
    
    return 0;
}





