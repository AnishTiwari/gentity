/* generator for sql_alchemy */

#ifndef G_SQL_ALCHEMY_H

#define G_SQL_ALCHEMY_H

#define SQL_ALCHEMY_FILE_NAME  "./outputs/sql_alchemy_models.py"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../structure.h"
#include "../utility.h"

FILE *fp1;
/* int g_line = 0; */
/* typedef struct store s; */
/* typedef s* s_t; */

/* typedef struct store_container sc; */
/* typedef sc* sc_t; */

/* struct store { */
/*   char* entity_name; */
/*   long  int sql_alchemy_end; */
/* }; */

/* struct store_container { */
/*   int idx; */
/*   s_t entity; */
/* }; */

/* sc_t ent; */


int sql_alchemy_init_file(){

  fp1 = fopen( SQL_ALCHEMY_FILE_NAME, "w+");

  if(fp1)
    {
      fputs(_HEADER, fp1);
      fclose(fp1);
      return 1;
    }
  else{
    perror("fopen");
    return 0;
  }
}

/* actual writer method */
void g_sql_alchemy(en_c_t container){
  printf("%d is the total attributes found!!!\n",container->en_idx);
  for(int i=0;i < container->en_idx ;i++){
    printf(" %d ) %s is related to \t %d", i, container->entity[i].name, container->entity[0].relation[0] ); 
    printf("Its Attribute are :::\n\n");
    for(int j=0; j < container->entity[i].attributes->idx ; j++){
      printf("%s \t %s \n",container->entity[i].attributes->attribute[j].attr_name, container->entity[i].attributes->attribute[j].attr_description);

    }
  }
  for(int c_i=0; c_i< container->en_idx; c_i++){
    en_t my_entity = &container->entity[c_i];
    /* opening file in append mode */
    fp1 = fopen( SQL_ALCHEMY_FILE_NAME,"a+");
    /* FILE* tmp; */
    /* tmp = freopen(0, "r+b", fp1); */
    /* if (tmp) fp1 = tmp; */
  
    printf("%s\n",_HEADER);


    /* ent->entity[ent->idx].sql_alchemy_end += strlen(_HEADER); */
    /* g_line += 2; */
    if(my_entity->description != NULL){
      fprintf(fp1,"# %s\n",my_entity->description);
    }
    char* temp_name = malloc(sizeof(char)* (strlen(my_entity->name))) ;
    strcpy(temp_name,my_entity->name);
    to_upper(&temp_name[0]);
    fprintf(fp1, "class %s(db.Model):\n",temp_name);
  
    fprintf(fp1, "\t__tablename__ = %s", my_entity->name);
  
    for(int i=0;i < my_entity->attributes->idx;i++){
      if(my_entity->attributes->attribute[i].attr_description != NULL){
	fprintf(fp1, "\n\t#%s",my_entity->attributes->attribute[i].attr_description);
  
      }
      fprintf(fp1, "\n\t%s = db.Column(db.Integer) ",my_entity->attributes->attribute[i].attr_name);
    
    }
  
    /* add relationship when parent, parent_relation are set in entitytypes.xml */
    if(my_entity->parent != NULL){

      /* ONE TO ONE RELATION */
      if(strcmp(my_entity->parent_relation ,"OneToOne") == 0)
	{

	  /* bringing file pointer to appropritae position before writing  */
	  fprintf(fp1,"\n\t%s_id = db.Column(db.Integer,db.ForeignKey('%s.id'), unique=True)",my_entity->parent,my_entity->parent);
	
	}

      /* ONE TO MANY */
      else if(strcmp(my_entity->parent_relation, "OneToMany") == 0){

      
      }
      /* MANY TO MANY */
      else{

      }

      
    }

    if(my_entity->size >= 0 ){


      for(int rel_idx =0; rel_idx <= my_entity->size; rel_idx ++){
	/* relation is one to one with child */
	if(strcmp(container->entity[my_entity->relation[rel_idx]].parent_relation, "OneToOne") == 0)
	  {
	    char* temp_relation;
	    temp_relation = malloc(sizeof(strlen(container->entity[my_entity->relation[rel_idx]].name)));
	    strcpy(temp_relation,container->entity[my_entity->relation[rel_idx]].name);
	    to_upper(&temp_relation[0]);
	    fprintf(fp1,"\n\t%s = db.relationship(\"%s\", backref=\"%s\" ,use_list=\"false\")",container->entity[my_entity->relation[rel_idx]].name, temp_relation,my_entity->name );

	  }
	/* relation is one to many with child */

	else if(strcmp(container->entity[my_entity->relation[rel_idx]].parent_relation, "OneToMany") == 0)
	  {
	    char* temp_relation;
	    temp_relation = malloc(sizeof(strlen(container->entity[my_entity->relation[rel_idx]].name)));
	    strcpy(temp_relation,container->entity[my_entity->relation[rel_idx]].name);
	    to_upper(&container->entity[my_entity->relation[rel_idx]].name[0]);
	    fprintf(fp1,"\n\t%s = db.relationship(\"%s\", backref=\"%s\" ,uselist=\"false\")",container->entity[my_entity->relation[rel_idx]].name, temp_relation,my_entity->name );

	  }
	/* relation is Many to many with child */
	else {

	}
	
      }
      
    }

    fputs("\n\n\n",fp1);
  
    fclose(fp1);
  } /* end of for */
}

#endif
