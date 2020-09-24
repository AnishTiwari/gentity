#include<stdio.h>

#include "parser.h"

int main(){

  if(parse()){
    printf("Succesfully Parsed!!");
  }

  else{
    perror("ERROR:");
  }

  return 0;
}
