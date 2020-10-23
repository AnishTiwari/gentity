#ifndef ENUMTYPEPARSER_H

#define ENUMTYPEPARSER_H

#include "./lib/uthash.h"

#include <string.h>
#include "./structure.h"
#include "./utility.h"
#include <stdlib.h>
#include<stdio.h>

#define ENUMTYPE_FILE "./resources/enumtypes.xml"


ec_t my_ec, s1 ;
_d_t parsed;


et_t my_et ;

void add_new_user(ec_t ec);

ec_t find_enum(char* key, ec_t ec);

void print_enums();


int enumtypeparse();

#endif
