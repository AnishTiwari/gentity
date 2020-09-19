#ifndef DATATYPEPARSER_H

#define DATATYPEPARSER_H

#include "./lib/uthash.h"

#include <string.h>
#include "./structure.h"
#include "./utility.h"
#include <stdlib.h>
#include<stdio.h>

#define DATATYPE_FILE "./resources/datatypes.xml"

dt_t my_dt, s ;
_d_t parsed;

void add_user(dt_t dt);

dt_t find_key(char* key, dt_t dt);

void print_users();


int datatypeparse();

#endif
