//
//  main.c
//  Lloyd_Max_scalar_quantizer(4_bits_case)
//
//  Created by Tianxiao Zhang on 9/28/17.
//  Copyright © 2017 Tianxiao Zhang. All rights reserved.
//

#include <stdio.h>
#include <time.h>


void x() {
    static double me_time;
    static int count = 0;
    count++;
    int a = 0;
    clock_t tic = clock();
    for(int i=0;i<100000;i++) {
        for(int j=0;j<10000;j++) {
            a = i + j;
        }
    }
    clock_t toc = clock();
    me_time += (double)(toc-tic)/CLOCKS_PER_SEC;
    printf("%f seconds\n",me_time);
}


int main(int argc, const char * argv[]) {
 
    for(int k=0;k<10;k++) {

        x();
    }
    
    return 0;
}
