
/* this file contains the structure of entity */
#ifndef STRUCTURE_H

#define STRUCTURE_H

typedef struct entity_container en_c;
typedef en_c* en_c_t;

typedef struct entity en;
typedef en* en_t;

typedef struct attributes attr;
typedef attr* attr_t;

typedef struct attributes_container attr_c;
typedef attr_c* ac_t;

struct attributes_container{
  int idx;
  attr_t attribute;
};
struct attributes{
  char* attr_name;
  char* attr_description;
  char* type;
};

struct entity{
  char* name;
  char* description;
  char* parent;
  char* parent_relation;
  int  persistent;
  ac_t attributes;
};

struct entity_container{
  int en_idx;
  en_t entity;
};

#endif
