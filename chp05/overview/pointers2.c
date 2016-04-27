#include<stdio.h>

int main(){
   int y = 1000, z;
   int *p = &y;
   printf("The pointer p has the value %d and address: %p\n", *p, p);
   z = *p++ + 5;
   printf("The pointer p has the value %d and address: %p\n", *p, p);
   printf("The variable z has the value %d\n", z);
   return 0;
}
