/* generator for Angular */

#ifndef G_ANGULAR_H

#define G_ANGULAR_H

#define ANGULAR_FILE_NAME  "./outputs/angular_model.ts"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../../structure.h"
#include "../../utility.h"
#include "../../datatypeparser.h"

#include "../../enumtypeparser.h"
#include "./g_angular_mapper.c"

FILE *fp1_;

int angular_init_file(){

  fp1_ = fopen(ANGULAR_FILE_NAME, "w+");
  
  if(fp1_)
    {
      fputs(TS_HEADER, fp1_);
      fclose(fp1_);
      return 1;
    }

  else{
    perror("fopen");
    return 0;
  }
  
}

/* actual writer method */
void g_angular(en_c_t container, dt_t dt, ec_t ec){

  printf("%d is the total attributes found %s!!!\n",container->en_idx,dt->name);

  for(int c_i=container->en_idx-1; c_i>=0 ; c_i--){
    
    en_t my_entity = &container->entity[c_i];

    /* opening file in append mode */
    fp1_ = fopen(ANGULAR_FILE_NAME, "a+");

    char* temp_name = malloc(sizeof(char)* (strlen(my_entity->name) + 1 )) ;
    strcpy(temp_name, my_entity->name);
    to_upper(&temp_name[0]);
    
    fprintf(fp1_, "export class %s{\n", temp_name);

    for(int i=0;i < my_entity->attributes->idx;i++){

      dt_t temp_dt =  find_key(my_entity->attributes->attribute[i].type, my_dt);

      /* check if the attr is of type -ENUM */
      if(strstr(my_entity->attributes->attribute[i].attr_name, "Enum") != NULL){

	fprintf(fp1_, "\t%s:%s", my_entity->attributes->attribute[i].attr_name, my_entity->attributes->attribute[i].attr_name);	  	  
      }

      /* if not enum type */
      else{
	
	char* got_basetype =  g_typescript_mapper(temp_dt->basetype);
	fprintf(fp1_, "\t%s: %s,\n",my_entity->attributes->attribute[i].attr_name,got_basetype);

      }
    }
    
    /* *************************  nested models *****************************/
    /*  1: if the current entity is parent to some other entity*/
    for(int child=0; child <= my_entity->size; child++){

      if(strcmp(container->entity[my_entity->relation[child]].parent_relation, "OneToOne") == 0)
	{
	  char* temp_name1 = malloc(sizeof(char) * (strlen(container->entity[my_entity->relation[child]].name))) ;
	  strcpy(temp_name1,container->entity[my_entity->relation[child]].name);
	  to_upper(&temp_name1[0]);

	  fprintf(fp1_, "\t%s: %s,\n", container->entity[my_entity->relation[child]].name, temp_name1);

	  free(temp_name1);
	}

      else {
	char* t = container->entity[my_entity->relation[child]].name;
	char* t1 = container->entity[my_entity->relation[child]].name;
	char* plural_child;
	plural_child = malloc(sizeof(char) * strlen(t));
	strcpy(plural_child, t);
	pluralise_word(&plural_child[0]);

	char* self = my_entity->name;
	char* plural_self;
	plural_self = malloc(sizeof(char) * strlen(self));
	strcpy(plural_self, self);
	pluralise_word(&plural_self[0]);
	char* temp_name2 = malloc(sizeof(char) * (strlen(t1))) ;
	strcpy(temp_name2, t1);
	to_upper(&temp_name2[0]);
    
	fprintf(fp1_, "\t%s: %s[],\n", plural_child, temp_name2);

	free(plural_child);

      }
 
    }
    /* refersto to code generation */
    for(int rel_idx =0; rel_idx <= my_entity->ref_size; rel_idx ++){

      char* t = container->entity[my_entity->references[rel_idx]].name;
      char* t1 = t;
      char* plural_child;
      plural_child = malloc(sizeof(char) * strlen(t));
      strcpy(plural_child, t);
      pluralise_word(&plural_child[0]);

      char* self = my_entity->name;
      char* plural_self;
      plural_self = malloc(sizeof(char) * strlen(self));
      strcpy(plural_self, self);
      pluralise_word(&plural_self[0]);
	  
      char* temp_name2 = malloc(sizeof(char) * (strlen(t1))) ;
      strcpy(temp_name2, t1);
      to_upper(&temp_name2[0]);
    
      fprintf(fp1_, "\t%s: %s[],\n", plural_child, temp_name2);
	  
      free(plural_child);
    }

    
    fprintf(fp1_, "}\n\n\n");
    fclose(fp1_); 
  }
}
#endif
