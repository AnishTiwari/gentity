/*
 *  Given a input base datatype find the assocaited datatype 
 *  name for the sql alchemy model class 
*/

/*
  the basics datatypes provided by flask - SqlAlchemy  	 
  Integer 
  String(size)
  Text
  DateTime
  Float
  Boolean
  PickleType
  LargeBinary
*/

#include <string.h>
#include<stdio.h>
#include<stdlib.h>

char* g_mapper(char* datatype_name){
  
  if (strcmp(datatype_name, "Integer") == 0){
    return "Integer";
  }
  else if (strcmp(datatype_name, "String") == 0){
    return "String";
  }
  
  else if (strcmp(datatype_name, "Text") == 0){
    return "Text";
  }

  else if (strcmp(datatype_name, "Bool") == 0){
    return "Boolean";
  }

  else if (strcmp(datatype_name, "DateTime") == 0){
    return "DateTime";
  }
  
  /* exit if no mapper found */
  printf("DataType Mapper %s: No such Datatype type mapper found in sql alchemy _mapper\n", datatype_name);
  exit(1);
}


 
