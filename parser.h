#ifndef PARSER_H

#define PARSER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "./generators/g_sql_alchemy.h"
#include "./generators/marshmallow/g_marshmallow.c"
#include "./generators/angular/g_angular.c"

#include "./structure.h"
#include "./utility.h"
#include "./datatypeparser.c"
#include "./enumtypeparser.c"

#define FILE_NAME "./resources/entitytypes.xml"
#define __CONCAT_INT(a,b) a##b

int parse(){

  FILE *fp;
  fp = fopen(FILE_NAME, "r");

  if( !fp ){
    perror("No such file or content found !!\n");
    return 0;
  }

  /* init all generators here */

  if(sql_alchemy_init_file() != 1){
    printf("Exiting now due to file SQL_ALCHEMY init error\n");
    exit(0);
  }

  
  if(marshmallow_init_file() != 1){
    printf("Exiting now due to file Marshmallow init error\n");
    exit(0);
  }

  if(angular_init_file() != 1){
    printf("Exiting now due to file angular init error\n");
    exit(0);
  }
  
  
  /* end of inits */
  
  _d_t parsed;
  en_t my_entity = NULL;
  en_c_t container = NULL;
  int i = 0; 
  char curr_char;
  char curr_word[128] ;
  /* read the .xml file and parse it word by word*/
  while((curr_char = fgetc(fp)) != EOF ){

    if(curr_char != ' ' ){
      curr_word[i++] = curr_char;
    }
    /*  end of word reached */
    else {
      curr_word[i] = '\0';
      if(curr_word[0] == '\0'){
	/* printf("CAME ACROSS SPACES TOKEN \n"); */
      }
      else
	{
	  i=0;
	  
	  if( (strstr(curr_word, "/Entity.Attributes") != NULL)){
	    
	  }

	  else if( (strstr(curr_word, "/Entity.References") != NULL)){

	  }

	  else if((strstr(curr_word, "/Entity" ) != NULL)){

	    container->entity[container->en_idx] = *my_entity;
	    container->en_idx++;
	    
	    container->entity = realloc(container->entity, sizeof(en) * (container->en_idx + 1));
	   
	  }

	  else if( strstr(curr_word, "/Container") != NULL ){

	    /* calling the datatype parser here and printing the parsed datatyes */
	    if(datatypeparse() == 1){
	      printf("DataType File Not found !!!! \n\n\n\n");
	      exit(0);
	    }

	    if(enumtypeparse() == 1){
	      printf("EnumType File Not found !!!! \n\n\n\n");
	      exit(0);
	    }

	    printf("\n********************************************\n");
	    
	    /* CALL THE GENERATORS HERE */
	    g_sql_alchemy(container, my_dt);
	    g_marshmallow(container, my_dt, my_ec);
	    g_angular(container, my_dt, my_ec);

	    /* enum for sql alchemy */
	    write_enum_class();
	    
	  }

	  else  if(strstr(curr_word, "Entity.Attributes") != NULL){
	    /* allocate memory to contain attribute */
	    my_entity->attributes->attribute = malloc(sizeof(attr) * 1);
	  }

	  else  if(strstr(curr_word, "Entity.References") != NULL){
	    my_entity->rsize=0;
	  }

	  else if(strstr(curr_word, "Entity" ) != NULL){

	    /* create a new struct obj entity */
	    if( (my_entity = malloc(sizeof(en))) == NULL){
	      perror("MALLOC:");
	    }
	    
	    if( (my_entity->attributes = malloc(sizeof(attr_c))) == NULL){
	      perror("MALLOC:");
	    }

	    my_entity->description = NULL;
	    my_entity->attributes->idx = 0;
	    my_entity->relation = (int *)malloc(sizeof(int) * 1);
	    my_entity->size = -1;
	    my_entity->references = (int *)malloc(sizeof(int) * 1);
	    my_entity->ref_size = -1;
	    my_entity->ref_p_size = -1;
	    my_entity->ref_parent = malloc(sizeof(en) * 1);

	  }

	  else if(strstr(curr_word, "Container") != NULL ){

	    if( (container = malloc(sizeof(en_c))) == NULL) { perror("MALLOC:");}
	    container->en_idx = 0;
	    container->entity = malloc(sizeof(en));

	  }

	  else{
	    /* call split only it contains key-value pairs */
	    parsed = split(curr_word);
	    
	    if(parsed != NULL && parsed->key != NULL && parsed->value !=NULL)
	      {
		char* sanitised_value;
		printf("Parsed KEY:  is %s\n",parsed->key);
		printf("Parsed VALUE:is %s\n",parsed->value);

		sanitised_value = malloc(sizeof(char) * strlen(parsed->value));
		int x=0, y=0, counter=0;
		while(parsed->value[y] != '\0'  && counter != 2){
		  if(parsed->value[y] != '"')
		    sanitised_value[x++] = parsed->value[y++];
		  else
		    {
		      counter++;
		      y++;
		    }
		}
		sanitised_value[x] ='\0';
		
		if(strcmp(parsed->key, "Name") == 0){
		  my_entity->name  =(char *) malloc(sizeof(strlen(sanitised_value)) + 1);
		  strcpy(my_entity->name , sanitised_value);

		  my_entity->persistent = 1;
		}

		else if(strcmp(parsed->key, "Description") == 0){
		  printf("****************************************************************************DESSS\n");
		  my_entity->description  = malloc(sizeof(strlen(sanitised_value)) +1 );
		  strcpy(my_entity->description , sanitised_value);
		}
	
		else if( strcmp(parsed->key, "Parent") == 0 ){
		  int entity_no;
		  my_entity->parent  = malloc(sizeof(strlen(sanitised_value)) +1 );
		  strcpy(my_entity->parent , sanitised_value);

		  /* find the parent entity in the container struct */
		  if( (entity_no = find_entity(container, my_entity->parent))  == -1) {
		    perror("NO SUCH ENTITY:");
		    exit(0);
		  }
		  else
		    {
		      container->entity[entity_no].size +=1;
		      if(container->entity[entity_no].size != 0)
		      
		        container->entity[entity_no].relation = realloc( container->entity[entity_no].relation, sizeof(container->entity[entity_no].size)+1  );
		      
		      container->entity[entity_no].relation[container->entity[entity_no].size] = container->en_idx;		      
		    }
		}

		else if( strcmp(parsed->key, "RefersTo") == 0 ){

		  my_entity->attributes->idx--;		
		  my_entity->rsize +=1;
		  int entity_no;

		  /* find the parent entity in the container struct */
		  if( (entity_no = find_entity(container, sanitised_value))  == -1) {
		    perror("NO SUCH ENTITY:");
		    exit(0);
		  }
		  
		  else
		    {
		      container->entity[entity_no].ref_size += 1;
		      my_entity->ref_p_size += 1;

		      if(container->entity[entity_no].ref_size != 0){

			container->entity[entity_no].references = realloc( container->entity[entity_no].references, sizeof(container->entity[entity_no].ref_size) + 1 );
			my_entity->ref_parent = realloc( my_entity->ref_parent, sizeof(my_entity->ref_p_size ) + 1 );
		      }

		      
		      container->entity[entity_no].references[container->entity[entity_no].ref_size] = container->en_idx;
		      
		      my_entity->ref_parent[my_entity->ref_p_size] = entity_no;
				     
		      printf("\n\n%s^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^%s\n", container->entity[entity_no].name, my_entity->name);
		 
		    }
		}
		
		else if(strcmp(parsed->key, "ParentRelation") == 0){
		  my_entity->parent_relation  = malloc(sizeof(strlen(sanitised_value)) + 1 );
		  strcpy(my_entity->parent_relation , sanitised_value);

		}

		else if(strcmp(parsed->key, "AttrName") == 0){
		  /* making the nullable flag false by default here */
		  my_entity->attributes->attribute[my_entity->attributes->idx].is_nullable = 1;
 
		  my_entity->attributes->attribute[my_entity->attributes->idx].attr_name = malloc(sizeof(strlen(sanitised_value)) +1 );
		  strcpy(my_entity->attributes->attribute[my_entity->attributes->idx].attr_name , sanitised_value);
		}
	
		else if(strcmp(parsed->key, "AttrDescription") == 0){
		  my_entity->attributes->attribute[my_entity->attributes->idx].attr_description = (char *)malloc(sizeof(strlen(sanitised_value)) +1);
		  strcpy( my_entity->attributes->attribute[my_entity->attributes->idx].attr_description , sanitised_value);
		}

		else if(strcmp(parsed->key, "Validation") == 0){
		  my_entity->attributes->attribute[my_entity->attributes->idx].validation = (char *)malloc(sizeof(strlen(sanitised_value)) +1);
		  strcpy( my_entity->attributes->attribute[my_entity->attributes->idx].validation , sanitised_value);
		}
		
		else if(strcmp(parsed->key, "Type") == 0){
		  my_entity->attributes->attribute[my_entity->attributes->idx].type = malloc(sizeof(strlen(sanitised_value)) +1 );
		  strcpy(my_entity->attributes->attribute[my_entity->attributes->idx].type , sanitised_value);
		}
   
		else if(strcmp(parsed->key, "Nullable") == 0){
		  if(strcmp(sanitised_value, "False") == 0)
		    my_entity->attributes->attribute[my_entity->attributes->idx].is_nullable = 0;

		}

		/* persistent or non persistent */
		else{
		  if(strcmp(parsed->key, "Persistent") == 0){		  
		    if(strcmp(parsed->value ,"\"False\"") == 0)
		      my_entity->persistent = 0;
		    else
		      my_entity->persistent = 1;
		  }
		}

	      }
	    if(parsed->end){
	      /*
		1) assign the attribute,
		2) allocate a new attribute struct memory for ++idx 
		3) free & create a new my_attr
	      */
	      my_entity->attributes->idx++;		
	      my_entity->attributes->attribute = realloc(my_entity->attributes->attribute,sizeof(attr) * ((my_entity->attributes->idx)+1));

	    }
	    else{
	      printf("%s !! not interested\n",curr_word);
	    }
	  }
	}
    }

  }

  return 1;
}

#endif
