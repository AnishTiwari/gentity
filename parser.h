#include<stdio.h>
#include<stdlib.h>

#include "./generators/g_sql_alchemy.h"

#define FILE_NAME "./resources/entitytypes.xml"

int parse(){

  FILE *fp;
  fp = fopen(FILE_NAME, "r");

  if( !fp ){
    perror("No such file or content found !!\n");
    return 0;
  }
  int i=0;
  int j = 0;
 char curr_char;
 char curr_word[128];  
 while((curr_char = fgetc(fp)) != EOF ){

   
   if(curr_char != ' '){
     curr_word[i++] = curr_char;
   }
    /*  end of word reached */

   else{
     

     curr_word[i] = '\0';
    printf("word parsd is %s\n",curr_word);
    i=0;

    /* put your generators here */

    /* ::: generator for sql_alchemy :::: */

    g_sql_alchemy(curr_word);
    
   }
   
  }

  
  return 1;
}











