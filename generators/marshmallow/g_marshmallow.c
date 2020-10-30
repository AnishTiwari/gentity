/* generator for Marshmallow */

#ifndef G_MARSHMALLOW_H

#define G_MARSHMALLOW_H

#define MARSHMALLOW_TYPES_FILE_NAME  "./outputs/marshmallow_types.py"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../../structure.h"
#include "../../utility.h"
#include "../../datatypeparser.h"

#include "../../enumtypeparser.h"
#include "./g_marshmallow_data_mapper.c"

FILE *fp1_;

int marshmallow_init_file(){

  fp1_ = fopen(MARSHMALLOW_TYPES_FILE_NAME, "w+");

  if(fp1_)
    {
      fputs(_HEADER, fp1_);
      fclose(fp1_);
      return 1;
    }
  else{
    perror("fopen");
    return 0;
  }
}

/* actual writer method */
void g_marshmallow(en_c_t container, dt_t dt, ec_t ec){

  printf("%d is the total attributes found %s!!!\n",container->en_idx,dt->name);


  /* DEBUG PURPOSES */
  /* for(int i=0;i < container->en_idx ;i++){ */
  /*   printf(" %d ) %s is related to \t %d", i, container->entity[i].name, container->entity[0].relation[0] ); */
  /*   printf("Its Attribute are :::\n\n"); */
  /*   for(int j=0; j < container->entity[i].attributes->idx ; j++){ */
  /*     printf("%s \t %s \n",container->entity[i].attributes->attribute[j].attr_name, container->entity[i].attributes->attribute[j].attr_description); */

  /*   } */
  /* } */

  for(int c_i=container->en_idx-1; c_i>=0 ; c_i--){
    
    en_t my_entity = &container->entity[c_i];
    /* opening file in append mode */

    fp1_ = fopen(MARSHMALLOW_TYPES_FILE_NAME, "a+");

    char* temp_name = malloc(sizeof(char)* (strlen(my_entity->name) + 1 )) ;
    strcpy(temp_name, my_entity->name);
    to_upper(&temp_name[0]);
    
    fprintf(fp1_, "class %sSchema(Schema):\n", temp_name);

    /* creating dump only primary key id field only if persistent is true */
    if(my_entity->persistent == 1){
      fprintf(fp1_, "\tid = fields.Int(dump_only=True)\n");
    }
    for(int i=0;i < my_entity->attributes->idx;i++){
      dt_t temp_dt =  find_key(my_entity->attributes->attribute[i].type, my_dt);

      /* check if the attr is of type -ENUM */

      if(strstr(my_entity->attributes->attribute[i].attr_name, "Enum") != NULL){
	fprintf(fp1_, "\t%s = fields.Str(validate=validate.OneOf([", my_entity->attributes->attribute[i].attr_name);
	  
	ec_t key_enum_container = find_enum( my_entity->attributes->attribute[i].attr_name, ec);

	et_t key_enum = key_enum_container->enums;
	while(key_enum != NULL){
	  fprintf(fp1_,"'%s',", key_enum->value);
	  key_enum = key_enum->next;
	}
	fprintf(fp1_, "]))\n");
	  
      }
      else{
	char* got_basetype =  g_marshmallow_mapper(temp_dt->basetype);
	if(my_entity->attributes->attribute[i].is_nullable == 0){

	  if(dt->length ){
	    fprintf(fp1_, "\t%s = fields.%s(required=True, validate=validate.Length(max=%d))\n",my_entity->attributes->attribute[i].attr_name,got_basetype,dt->length);
 
	  }
	  else{
	    fprintf(fp1_, "\t%s = fields.%s(required=True)\n",my_entity->attributes->attribute[i].attr_name,got_basetype);
	  }
	}
      
	else{
	
	  /* check if its contains rule */
	  if(dt->rule != NULL){
	    fprintf(fp1_, "\t%s = fields.%s(validate=validate.Length(max=%d))\n",my_entity->attributes->attribute[i].attr_name,got_basetype,dt->length);
 
	  }
	  else{
	    fprintf(fp1_, "\t%s = fields.%s()\n",my_entity->attributes->attribute[i].attr_name,got_basetype);
	  }
	}
      
      }
    }
    
    /* **********  nested schemas ****************/

    /*  1: if the current entity is parent to some other entity*/
    for(int child=0; child <= my_entity->size; child++){
      if(strcmp(container->entity[my_entity->relation[child]].parent_relation, "OneToOne") == 0)
	{

	  char* temp_name1 = malloc(sizeof(char) * (strlen(container->entity[my_entity->relation[child]].name))) ;
	  strcpy(temp_name1,container->entity[my_entity->relation[child]].name);
	  to_upper(&temp_name1[0]);

	  /* exclude necessary necessary to avoid self loop */
	  fprintf(fp1_, "\t%s = fields.Nested('%sSchema', exclude=('%s',))\n",container->entity[my_entity->relation[child]].name, temp_name1, my_entity->name);

	  free(temp_name1);
	}

      else if(strcmp(container->entity[my_entity->relation[child]].parent_relation, "OneToMany") == 0)
	{
	  /* exclude curr entity  to avoid self loop */

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
    
	  fprintf(fp1_, "\t%s = fields.Nested('%sSchema',many=True, exclude=('%s',))\n",plural_child, temp_name2, plural_self);
	  
	  free(plural_child);
	}
	
    }

    /* 2: if the current entity is child to someother entity */

    if(my_entity->parent != NULL){

      if(strcmp(my_entity->parent_relation ,"ManyToMany") != 0)
	{
	  
	  char* temp_name1 = malloc(sizeof(char) * (strlen(my_entity->parent))) ;
	  strcpy(temp_name1, my_entity->parent);
	  to_upper(&temp_name1[0]);
    
	  fprintf(fp1_,"\t%s = fields.Nested('%sSchema', exclude=('%s', ))",my_entity->parent,temp_name1,my_entity->name);

	  free(temp_name1);
	}

      else {
	
	char* plural_child;
	plural_child = malloc(sizeof(my_entity->parent));
	strcpy(plural_child, my_entity->parent);
	pluralise_word(&plural_child[0]);
	  
	char* temp_name1 = malloc(sizeof(char) * (strlen(my_entity->parent))) ;
	strcpy(temp_name1, my_entity->parent);
	to_upper(&temp_name1[0]);


	
	  char* self = my_entity->name;
	   char* plural_self;
	  plural_self= malloc(sizeof(char) * strlen(self));
	  strcpy(plural_self, self);
	  pluralise_word(&plural_self[0]);
	  
	
	fprintf(fp1_,"\t%s = fields.Nested('%sSchema', many=True, exclude=('%s', ))",plural_child, temp_name1, plural_self);

	free(temp_name1);
      }
      
    }
    
    fprintf(fp1_, "\n\n\n");
    fclose(fp1_); 
  
  }
 
}
#endif
