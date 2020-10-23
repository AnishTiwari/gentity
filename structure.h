
/* this file contains the structure of entity */
#ifndef STRUCTURE_H

#include "./lib/uthash.h"
#define STRUCTURE_H

typedef struct entity_container en_c;
typedef en_c* en_c_t;

typedef struct entity en;
typedef en* en_t;

typedef struct attributes attr;
typedef attr* attr_t;

typedef struct attributes_container attr_c;
typedef attr_c* ac_t;

/* typedef struct relation r; */
/* typedef r* r_t; */

/* struct relation{ */
/*   int child; */
/* }; */

struct attributes_container{ 
  attr_t attribute;
  int idx;
};

struct attributes{
  char* attr_name;
  char* attr_description;
  char* type;
  int is_nullable;
  char* validation;
};

struct entity{
  int* relation; 		/* stores the ptr of any child relation found for the current entity */
  ac_t attributes;
  char* name;
  char* description;
  char* parent;
  char* parent_relation;
  int  persistent;
  int size;
 
};

struct entity_container{
  en_t entity;
  int en_idx;

};


/* datatype hash table struct */

typedef struct datatype dt;
typedef dt* dt_t;

struct datatype{
  UT_hash_handle hh; 
  char* name;
  char* basetype;
  int  length;
  char* rule;
};


typedef struct enumcontainer ec;
typedef ec* ec_t;

typedef struct enumtype et;
typedef et* et_t;

/* enumtype struct (linked list)  */
struct enumtype{
  char* value;
  et_t next;
};

struct enumcontainer{
  UT_hash_handle hh; 
  char* enum_name;
  et_t enums ;
 
};
#endif
