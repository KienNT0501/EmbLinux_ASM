#include "../inc/strutils.h"
void main(){
    char* str_test1 = " Kien Dep Trai ";
    char* str_test2 = "37";
    printf("Results after linking libary!\n");
    printf("1. String Reverse of _ Kien Dep Trai _ : %s\n", str_reverse(str_test1) );
    printf("2. String Strim of_ Kien Dep Trai _:%s\n", str_trim(str_test1) );
    printf("3. String to Int of 37 - 1 : %d\n", str_to_int(str_test2)-1 );
}