
/* 
 * Given a input data type from dattypeparser.c associate it with
 * a marshmallow data type

 * supported datatypes of marshmallow

 
 * __all__ = [
 *     "Field",
 *     "Raw",
 *     "Nested",
 *     "Mapping",
 *     "Dict",
 *     "List",
 *     "Tuple",
 *     "String",
 *     "UUID",
 *     "Number",
 *     "Integer",
 *     "Decimal",
 *     "Boolean",
 *     "Float",
 *     "DateTime",
 *     "NaiveDateTime",
 *     "AwareDateTime",
 *     "Time",
 *     "Date",
 *     "TimeDelta",
 *     "Url",
 *     "URL",
 *     "Email",
 *     "IP",
 *     "IPv4",
 *     "IPv6",
 *     "Method",
 *     "Function",
 *     "Str",
 *     "Bool",
 *     "Int",
 *     "Constant",
 *     "Pluck",
 * ]
 * 
 * 
 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char* g_marshmallow_mapper(char* datatype_name){


  if (strcmp(datatype_name, "Integer") == 0){
    return "Integer";
  }
  else if (strcmp(datatype_name, "String") == 0){
    return "Str";
  }
  
  else if (strcmp(datatype_name, "Text") == 0){
    return "String";
  }

  else if (strcmp(datatype_name, "Bool") == 0){
    return "Bool";
  }

  else if (strcmp(datatype_name, "DateTime") == 0){
   return "DateTime";
  }
  
  /* exit if no mapper found */
  printf("\n\n DataType Mapper %s: No such Datatype type mapper found in marshmallow _mapper\n", datatype_name);
  exit(1);


}


