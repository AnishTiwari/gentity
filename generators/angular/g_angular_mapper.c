
/* 
 * Given a input data type from dattypeparser.c associate it with
 * a angular model type

 * supported datatypes of typescript

 
 *Boolean
 *Number
 *String
 *Array
 *Tuple
 *Enum
 *Unknown
 *Any
 *Void
 *Null and Undefined
 *Never
 *Object 
 * 
 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char* g_typescript_mapper(char* datatype_name){


  if (strcmp(datatype_name, "Integer") == 0){
    return "Number";
  }
  else if (strcmp(datatype_name, "String") == 0){
    return "String";
  }
  
  else if (strcmp(datatype_name, "Text") == 0){
    return "String";
  }

  else if (strcmp(datatype_name, "Bool") == 0){
    return "Boolean";
  }

  else if (strcmp(datatype_name, "DateTime") == 0){
   return "DateTime";
  }
  
  /* exit if no mapper found */
  printf("\n\n DataType Mapper %s: No such Datatype type mapper found in angular _mapper\n", datatype_name);
  exit(1);


}


