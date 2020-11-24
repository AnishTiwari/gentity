#include<stdio.h>

#include "parser.h"


int main(){

 if(parse()){
    printf("Successfully Parsed!!");
  }

  else{
    perror("ERROR:");
  }

  return 0;
}
