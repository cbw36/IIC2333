#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<stdarg.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <string.h>
#include "arraylist.h"


const int OFFSET_BITS = 8;
const int ADDRESS_BITS = 20;
const int EXTRA_BITS = 3;
const int FRAME_SIZE = 256; //bytes
const int OFFSET = 8;





int main(int argc, char * argv[])
{
    char* n_raw = argv[1];
    int n = atoi(n_raw );
    ArrayList* opt_bits = arraylist_init();
    int total = 0;

    for (int i = 1; i< ADDRESS_BITS - n + 2;i++)
    {
        if (n > 1){

            for (int j = 1; j < ADDRESS_BITS - n + 3 - i; j++){

                if (n > 2){

                        if (n > 3){
                              for (int k = 1; k < ADDRESS_BITS - n + 3 - i - j; k++){
                                if (n > 4){
                                  for (int l = 1; l < ADDRESS_BITS - n + 3 - i - j - k; l ++) {
                                    int m = 20 - j-i-k-l;

                                        if (j+i+k+l+m == 20 &&(total == 0 || total > pow(2, i)*j+(pow(2, j)*k)+(pow(2, k)*l)+(pow(2, l)*m)+(pow(2, m)*11)) ){
                                            if (total != 0) {
                                            arraylist_delete(opt_bits,4);
                                            arraylist_delete(opt_bits,3);
                                            arraylist_delete(opt_bits,2);
                                            arraylist_delete(opt_bits,1);
                                            arraylist_delete(opt_bits,0);
                                            }
                                            total = pow(2, i)*j+(pow(2, j)*k)+(pow(2, k)*l)+(pow(2, l)*m)+(pow(2, m)*11);
                                            arraylist_append(opt_bits, i);
                                            arraylist_append(opt_bits, j);
                                            arraylist_append(opt_bits, k);
                                            arraylist_append(opt_bits, l);
                                            arraylist_append(opt_bits, m);

                                        }

                                }}
                                else{
                                    int l = 20-j-i-k;
                                    if (j+i+k+l == 20 &&(total == 0 || total > pow(2, i)*j+(pow(2, j)*k)+(pow(2, k)*l)+(pow(2, l)*11)) ){

                                        if (total != 0) {
                                        arraylist_delete(opt_bits,3);
                                        arraylist_delete(opt_bits,2);
                                        arraylist_delete(opt_bits,1);
                                        arraylist_delete(opt_bits,0);
                                        }
                                        total = pow(2, i)*j+(pow(2, j)*k)+(pow(2, k)*l)+(pow(2, l)*11);
                                        arraylist_append(opt_bits, i);
                                        arraylist_append(opt_bits, j);
                                        arraylist_append(opt_bits, k);
                                        arraylist_append(opt_bits, l);
                                    }
                                    if  ((j+i+k+l) == 20){
                                        break;
                                    }
                                }
                        }}
                        else{
                          int k = 20 -j-i;
                            if (j+i+k == 20 &&(total == 0 || total > pow(2, i)*j+(pow(2, j)*k)+(pow(2, k)*11)) ){

                                if (total != 0) {
                                  arraylist_delete(opt_bits,2);
                                  arraylist_delete(opt_bits,1);
                                  arraylist_delete(opt_bits,0);
                                }
                                total = pow(2, i)*j+(pow(2, j)*k)+(pow(2, k)*11);
                                arraylist_append(opt_bits, i);
                                arraylist_append(opt_bits, j);
                                arraylist_append(opt_bits, k);
                            }
                    }
                }
                else{
                  //printf("%i %f\n",i,pow(2, i));

                    if (j+i== 20 && (total == 0 || total > ((pow(2, i)*j)+(pow(2, j)*11))) ){
                      //printf("i=%i j= %i total= %f\n",i,j, (pow(2, i)*j)+(pow(2, j)*11));
                      if (total != 0) {
                      arraylist_delete(opt_bits,1);
                      arraylist_delete(opt_bits,0);
                      }
                        total = (pow(2, i)*j)+(pow(2, j)*11);
                        //printf("%i\n", i);
                        //printf("%i\n", j);
                        arraylist_append(opt_bits, i);
                        arraylist_append(opt_bits, j);
                    }
                    if  ((j+i) == 20){
                        break;
                    }
                }
            }
        }
        else{
            arraylist_append(opt_bits, 20);
            break;
        }
}
    for (int i = 0; i<n; i++)
    {
        printf("Bits for level %i = %i \n", i, opt_bits->list[i]);
    }
}
