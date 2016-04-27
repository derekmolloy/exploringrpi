#include<unistd.h>
#include<stdio.h>
int main(){
   int x=0;
   do{
      printf("Hello Raspberry Pi!\n");
      sleep(5);
   }while(x++<50);
   return 0;
}
