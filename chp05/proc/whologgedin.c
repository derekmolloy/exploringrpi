/* Written by Derek Molloy for the book "Exploring RPi */ 

#include<unistd.h>
#include<stdio.h>
int main(){
   printf("The user logged in is %s\n", getlogin());
   return 0;
}
