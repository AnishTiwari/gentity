/* generator for sql_alchemy */

#ifndef G_SQL_ALCHEMY_H

#define G_SQL_ALCHEMY_H

#define SQL_ALCHEMY_FILE_NAME  "./outputs/sql_alchemy_models.py"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../structure.h"
#include "../utility.h"
#include "../datatypeparser.h"
#include "./g_data_mapper.c"
#include "../enumtypeparser.h"


FILE *fp1;

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
void g_sql_alchemy(en_c_t container, dt_t dt){

  printf("%d is the total attributes found!!!\n",container->en_idx);


  /* DEBUG PURPOSES */
  /* for(int i=0;i < container->en_idx ;i++){ */
  /*   printf(" %d ) %s is related to \t %d", i, container->entity[i].name, container->entity[0].relation[0] );  */
  /*   printf("Its Attribute are :::\n\n"); */
  /*   for(int j=0; j < container->entity[i].attributes->idx ; j++){ */
  /*     printf("%s \t %s \n",container->entity[i].attributes->attribute[j].attr_name, container->entity[i].attributes->attribute[j].attr_description); */

  /*   } */
  /* } */

  for(int c_i=0; c_i< container->en_idx; c_i++){
    en_t my_entity = &container->entity[c_i];
    /* opening file in append mode */
    fp1 = fopen( SQL_ALCHEMY_FILE_NAME,"a+");
  
    int special = 0;
    printf("%d is the persistent value for the entity",my_entity->persistent);
    if(my_entity->persistent == 1){ 

      if(my_entity->size >= 0 ){

	for(int rel_idx =0; rel_idx <= my_entity->size; rel_idx ++){

	  /* Association table creation */
	  if(strcmp(container->entity[my_entity->relation[rel_idx]].parent_relation, "ManyToMany") == 0)
	    {
	
	      char* assoc_entity;
	      assoc_entity = malloc(sizeof(char) * (strlen(container->entity[my_entity->relation[rel_idx]].name)+strlen(my_entity->name))+1);
	      char* caps_1 ;
	      caps_1 = malloc(sizeof(char) * (strlen(container->entity[my_entity->relation[rel_idx]].name)) +1);
	      strcpy( caps_1 ,container->entity[my_entity->relation[rel_idx]].name);
	      /* to_upper(&caps_1[0]); */

	      printf("%s:\n\n\n",caps_1);
	    
	      strcat(assoc_entity,caps_1);

	      strcat(assoc_entity, "_");
	     
	      caps_1 = malloc(sizeof(char) * (strlen(my_entity->name)));
	      strcpy( caps_1 ,my_entity->name);
	   
	      /* to_upper(&caps_1[0]); */
	      strcat(assoc_entity,caps_1);
 
	      fprintf(fp1, "\n%s = db.Table(\"%s\",\n\tdb.Column(\"%s_id\", db.Integer, db.ForeignKey(\"%s.id\")),\n\tdb.Column(\"%s_id\", db.Integer, db.ForeignKey(\"%s.id\")))", assoc_entity,assoc_entity,my_entity->name, my_entity->name, container->entity[my_entity->relation[rel_idx]].name,container->entity[my_entity->relation[rel_idx]].name );

	      free(assoc_entity);
	      free(caps_1);
	      special = 1;
	      fputs("\n\n",fp1);
	    }
	  
	}
	
      }	  
      if(my_entity->description != NULL){
	fprintf(fp1,"\n# %s\n",my_entity->description);
      }
      char* temp_name = malloc(sizeof(char)* (strlen(my_entity->name))) ;
      strcpy(temp_name,my_entity->name);
      to_upper(&temp_name[0]);
      fprintf(fp1, "class %s(db.Model):\n",temp_name);
  
      fprintf(fp1, "\t__tablename__ = '%s'", my_entity->name);

      /* Auto generating Id primary key value */

      fprintf(fp1, "\n\tid = db.Column(db.Integer, primary_key=True)");

      
      /* assigning the attribute values to class model attribute */
      for(int i=0;i < my_entity->attributes->idx;i++){

	/* check if the attr is of type -ENUM */

	if(strstr(my_entity->attributes->attribute[i].attr_name, "Enum") != NULL){
	  fprintf(fp1, "\n\t%s = db.Column(db.Enum(%s))", my_entity->attributes->attribute[i].attr_name, my_entity->attributes->attribute[i].attr_name);

	  
	}
	else{
	  dt_t temp_dt =  find_key(my_entity->attributes->attribute[i].type, my_dt);

	  if(my_entity->attributes->attribute[i].attr_description != NULL){
	    fprintf(fp1, "\n\t#%s",my_entity->attributes->attribute[i].attr_description);

	  }

	  /* calling the g_mapper fn to map the correct datatype name
	   * :: if basetype got from g_mapper is string then put the
	   *     string length eg: string(20) if length=20
	   * :: else no need to do anything just write the basetype
	   */

	  char* got_basetype =  g_mapper(temp_dt->basetype);
	  if(my_entity->attributes->attribute[i].is_nullable == 0)
	    {
	      if(strcmp("String", got_basetype) == 0){
	    
		fprintf(fp1, "\n\t%s = db.Column(db.%s(%d), nullable=False)", my_entity->attributes->attribute[i].attr_name,got_basetype, find_key(my_dt->name,  dt)->length);
	      }
	      else
		{
		  fprintf(fp1, "\n\t%s = db.Column(db.%s, nullable=False)", my_entity->attributes->attribute[i].attr_name,got_basetype);
		}

	    }
	  else{

	    if(strcmp("String", got_basetype) == 0){
	      fprintf(fp1, "\n\t%s = db.Column(db.%s(%d))", my_entity->attributes->attribute[i].attr_name,got_basetype,  find_key(my_dt->name,  dt)->length);
	    }
	    else
	      {
		fprintf(fp1, "\n\t%s = db.Column(db.%s)", my_entity->attributes->attribute[i].attr_name,got_basetype);
	      }
	
	  }
	}
      }
  
      /* add relationship when parent, parent_relation are set in entitytypes.xml */
      if(my_entity->parent != NULL){

	/* ONE TO ONE RELATION */
	if(strcmp(my_entity->parent_relation ,"OneToOne") == 0)
	  {
	    fprintf(fp1,"\n\t%s_id = db.Column(db.Integer,db.ForeignKey('%s.id'), unique=True)",my_entity->parent,my_entity->parent);
	
	  }

	/* ONE TO MANY */
	else if(strcmp(my_entity->parent_relation, "OneToMany") == 0){

	  fprintf(fp1,"\n\t%s_id = db.Column(db.Integer,db.ForeignKey('%s.id'))",my_entity->parent,my_entity->parent);
	}
	/* MANY TO MANY */
	else{

	}
      }
      if(special){

	if(my_entity->size >= 0 ){

	  /* WRITES TO THE CURRENT CHILD IN CASE OF RELATION */
	  for(int rel_idx =0; rel_idx <= my_entity->size; rel_idx ++){
	    /* relation is one to one with child */
	    if(strcmp(container->entity[my_entity->relation[rel_idx]].parent_relation, "OneToOne") == 0)
	      {
		char* temp_relation;
		temp_relation = malloc(sizeof(strlen(container->entity[my_entity->relation[rel_idx]].name)));
		strcpy(temp_relation,container->entity[my_entity->relation[rel_idx]].name);
		to_upper(&temp_relation[0]);
		fprintf(fp1,"\n\t%s = db.relationship(\"%s\", backref=\"%s\" ,uselist=False)",container->entity[my_entity->relation[rel_idx]].name, temp_relation,my_entity->name );

	    
		free(temp_relation);
	      }
	    /* relation is one to many with child */

	    else if(strcmp(container->entity[my_entity->relation[rel_idx]].parent_relation, "OneToMany") == 0)
	      {
		char* temp_relation;
		temp_relation = malloc(sizeof(strlen(container->entity[my_entity->relation[rel_idx]].name)));
		strcpy(temp_relation, container->entity[my_entity->relation[rel_idx]].name);
		to_upper(&temp_relation[0]);

		char* plural_child;
		plural_child = malloc(sizeof(container->entity[my_entity->relation[rel_idx]].name));
		strcpy(plural_child, container->entity[my_entity->relation[rel_idx]].name);
		pluralise_word(&plural_child[0]);
		fprintf(fp1,"\n\t%s = db.relationship(\"%s\", backref=\"%s\")",plural_child, temp_relation,my_entity->name );
		free(plural_child);

		free(temp_relation);
	      }
	    /* relation is Many to many with child */
	    else {
	      char* assoc_entity;
	      assoc_entity = malloc(sizeof(char) * (strlen(container->entity[my_entity->relation[rel_idx]].name)+strlen(my_entity->name))+2);

	      char* caps_1 ;
	      caps_1 = malloc(sizeof(char) * (strlen(container->entity[my_entity->relation[rel_idx]].name)));

	      strcpy(caps_1, container->entity[my_entity->relation[rel_idx]].name);
	    
	      strcpy(assoc_entity, caps_1);

	      to_upper(&caps_1[0]);
	      strcat(assoc_entity, "_");
	      char* caps_2;
	      caps_2 = malloc(sizeof(char) * (strlen(my_entity->name)));

	      strcpy(caps_2, my_entity->name);
	      strcat(assoc_entity,caps_2);

	      printf("%s\n\n",assoc_entity);
	    
	      char* plural_child;
	      plural_child=malloc(sizeof(char) * strlen(container->entity[my_entity->relation[rel_idx]].name));
	      strcpy(plural_child , container->entity[my_entity->relation[rel_idx]].name);
	      pluralise_word(&plural_child[0]);


	      char* plural_backref_parent;
	      plural_backref_parent = malloc(sizeof(char) * strlen(my_entity->name));
	      strcpy(plural_backref_parent , my_entity->name);
	      pluralise_word(&plural_backref_parent[0]);

 
	      
	      fprintf(fp1, "\n\t%s = db.relationship(\"%s\", secondary=%s, backref=db.backrefs(\"%s\") )",plural_child, caps_1, assoc_entity, plural_backref_parent);

	      free(assoc_entity);
	      free(caps_1);
	      free(caps_2);
	      free(plural_backref_parent);
	      free(plural_child);
	    }
	  }
	
	}
      
      }

      fputs("\n\n\n",fp1);  
      fclose(fp1);
    }
  } /* end of for */
}

#endif
