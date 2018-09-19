//
//  main.c
//  Post_Processing_JPEG_Encoder_Images(reapplying_pepper1)
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

char pepper_raw_file[] = "/Users/zhtianxiao/Desktop/Reapplying/raw_image/pepper5.raw";

char pepper_raw_shift_file[64][100];

int main(int argc, const char * argv[]) {
    
    int pepper_n[size][size][3];
    BIT_FILE *pepper_raw;
    pepper_raw = OpenInputBitFile(pepper_raw_file);
    
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            for(int k=0;k<3;k++) {
            pepper_n[i][j][k] = (int)InputBits(pepper_raw, 8);
            }
        }
    }
    
    /* Using loop to print all 64 file name  */
    for(int t=0;t<64;t++) {
        sprintf(pepper_raw_shift_file[t],"/Users/zhtianxiao/Desktop/Reapplying/pepper5/pepper5_raw/pepper5_%d.raw",t+1);
    }
    
    
    BIT_FILE *pepper_raw_shift;
    
    
    int pepper_shift[size][size][3];
    
    int count = 0;
    
    /* Shift the image */
    for(int i=-3;i<=4;i++) {
        
        for(int j=-3;j<=4;j++) {
            
            for(int m=0;m<size;m++) {
                for(int n=0;n<size;n++) {
                    for(int p=0;p<3;p++) {
                    pepper_shift[m][n][p] = pepper_n[m][n][p];
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
                            pepper_shift[m][n+j][p] = pepper_n[m+i][n][p];
                            }
                        }
                    }
                    
                }
                
                /* If shift to upper left */
                if(j < 0) {
                    
                    for(int m=0;m<size-i;m++) {
                        for(int n=0;n<size+j;n++) {
                            for(int p=0;p<3;p++) {
                            pepper_shift[m][n][p] = pepper_n[m+i][n-j][p];
                            }
                        }
                    }
                    
                }
                
                
                
            }
            
            /* If shift to lower */
            if(i < 0) {
                
                /* if shift to lower right */
                if(j >= 0) {
                    
                    for(int m=0;m<size+i;m++) {
                        for(int n=0;n<size-j;n++) {
                            for(int p=0;p<3;p++) {
                            pepper_shift[m-i][n+j][p] = pepper_n[m][n][p];
                            }
                        }
                    }
                    
                }
                
                /* If shift to lower left */
                if(j < 0) {
                    
                    for(int m=0;m<size+i;m++) {
                        for(int n=0;n<size+j;n++) {
                            for(int p=0;p<3;p++) {
                            pepper_shift[m-i][n][p] = pepper_n[m][n-j][p];
                            }
                        }
                    }
                    
                }
                
                
            }
            
            /* Output shifted image */
            pepper_raw_shift =  NULL;
            pepper_raw_shift = OpenOutputBitFile(pepper_raw_shift_file[count++]);
            for(int m=0;m<size;m++) {
                for(int n=0;n<size;n++) {
                    for(int p=0;p<3;p++) {
                    OutputBits(pepper_raw_shift, (unsigned long)pepper_shift[m][n][p], 8);
                    }
                }
            }
            
            
        }
        
    }
    
    
    
    
    return 0;
}
