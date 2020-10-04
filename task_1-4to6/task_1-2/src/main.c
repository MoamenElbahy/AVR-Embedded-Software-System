#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include <avr/io.h>
//#include <util/delay.h>

// You have to declare functions before you can use them.
// The definition of them may take place at another place.

/**add two variables, uses call-by-value (copy of variables is passed)
 */
uint8_t add(uint8_t a, uint8_t b) {
    a += b;
    return a;
}

/**swap two variables, uses call-by-pointer (pointer to variables is passed)
 */
void swap(uint8_t *a, uint8_t *b) {
    uint8_t c = *a;
    *a = *b;
    *b = c;
}

void binary_rep(uint8_t a){

    printf("\nbinary representation of %d is : ",a);
    
    uint8_t binaryNum[8]={}; 
    uint8_t i = 0;
    while (a > 0) { 
  
        // storing remainder in binary array 
        binaryNum[i] = a % 2; 
        a = a / 2; 
        i++; 
    }
    for (int j = (sizeof(binaryNum)/sizeof(binaryNum[0]))-1; j >= 0; j--) {
        printf("%d",binaryNum[j]);
    }

}

void swapBits(uint8_t a) {

    binary_rep(a);
     uint8_t binary_num = (0xC0 & a)|((a<<3) & 0x38)|((a>>3) & 0x7); 
     binary_rep(binary_num);
}

void rotate(uint8_t a) {

    binary_rep(a);
    
    uint8_t binary_num;
    binary_num = (a>>7) | (0x2 & (a>>5)) | (0x4 & (a>>3)) | (0x8 & (a>>1)) | (0x10 & (a<<1)) | (0x20 & (a<<3)) | (0x40 & (a<<5)) | (a<<7);
  
   printf("\nthe equivalent reversed binary of %d is : %d",a, binary_num);
   binary_rep(binary_num);

}

typedef uint8_t (*aFunctionPointer_t)(uint8_t a, uint8_t b);



int main(void) {

  uint8_t a=15;
 /*  

  printf("\nyou entered : %d",a);
   //printf("rotate a = %d\n",rotate(a));
    rotate(a);
    
 //swapBits(a);


    DDRG |= 0x02; // setting a bit
    DDRG &= (~0x02)  // clear a bit

    without bit shofting : x |= 0x10;
    with bit shifting: x |= (0x1 << 4)

    DDRG ^= 0x02; // toggel a bit

    printf("!!!Hello World!!!\n");

    // define some variables
    uint8_t a = 3;
    uint8_t b = 5;
    uint8_t *c = &a;

    // call some functions
    printf("a=%d\nb=%d\n", a, b);
    printf("result of add: a+b=%d\n", add(a, b));

    swap(&a, &b);
    printf("result of swap: a=%d, b=%d\n", a, b);

    *c = add(a, b);
    printf("result of add: c=%d\n", *c);
    printf("result of add: a+b=%d?\n", add(a, b));

    printf("result of add: 200+100=%d?\n", add(200, 100));

    // sometimes you have to store the address of a function in a pointer
    aFunctionPointer_t p = &add;
    printf("call add via function pointer, result: %d\n", p(3,6));


    // bitwise operations

    // uses decimal numbers/ operation OR
    printf("3|5=%d\n", 3 | 5);

    // use hex and decimal numbers/ operation AND
    printf("0xf2&7=0x%02x\n", 0xf2 & 7);

    // use binary numbers/ operation XOR
    printf("0xb1011^0xb1100=%d \n", 0b1011 ^ 0b1100);

    // use decimal numbers/ operation NOT
    printf("~13=%d \n", (uint8_t) ~13);

    // left and right shift
    printf("1<<3=%d\n", 1 << 3);
    printf("0xf0>>4=0x%02x\n", 0xF0 >> 4);
    */
  
    /*
    uint8_t b = 0xC0 & a;
    uint8_t c = (a <<3);//0x38 & (3 <<a);
    uint8_t d = 0x7 & (a >>3); // 0x7 & (3 >>a);
    uint8_t f = d|(c|b);

   printf("a= %d\n",a);
   printf("b= %d\n",b);
   printf("c= %d\n",c);
   printf("d= %d\n",d);
   printf("f= %d\n",f);
   */

 
    aFunctionPointer_t p = &add;
    printf("call add via function pointer, result: %d\n", p(3,6));


    return EXIT_SUCCESS;
}
