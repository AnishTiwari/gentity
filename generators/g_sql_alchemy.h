/* generator for sql_alchemy */

#ifndef G_SQL_ALCHEMY_H

#define G_SQL_ALCHEMY_H

#define FILE_NAME  "../outputs/sql_alchemy_models.py"

#include<stdio.h>
#include<stdlib.h>

FILE *fp1;

int init_file(){

  fp1 = fopen(FILE_NAME, "w");
  return 1;
}


void g_sql_alchemy(char * word){
  fp1 = fopen((FILE_NAME),"a+");

  printf("%s",word);
}

#endif
