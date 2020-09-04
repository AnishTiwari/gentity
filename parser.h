
#ifndef PARSER_H

#define PARSER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "./generators/g_sql_alchemy.h"
#include "./structure.h"
#include "./utility.h"

#define FILE_NAME "./resources/entitytypes.xml"

/* typedef struct store s; */
/* typedef s* s_t; */

/* typedef struct store_container sc; */
/* typedef sc* sc_t; */

/* struct store { */
/*   char* entity_name; */
/*   int sql_alchemy_end; */
/* }; */

/* struct store_container { */
/*   int idx; */
/*   s_t entity; */
/* }; */

/* sc_t ent; */


int parse(){

  FILE *fp;
  fp = fopen(FILE_NAME, "r");

  if( !fp ){
    perror("No such file or content found !!\n");
    return 0;
  }


  /* init all generators here */

  if(sql_alchemy_init_file() != 1){
    printf("Exiting now due to file init error\n");
    exit(0);
  }

  /* allocate memory for store container struct */
  if( (ent = (sc_t)malloc(sizeof(sc))) == NULL){
    perror("MALLOC:");
  }
  ent->idx=0;
  ent->entity = malloc(sizeof(s));

  _d_t parsed;
  en_t my_entity = NULL;
  en_c_t container =NULL;
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
	printf("CAME ACROSS SPACES TOKEN \n");
      }
      else
	{
	  printf("word parsed is %s\n",curr_word);
	  i=0;

	  if(  (strstr(curr_word, "/Entity.Attributes") != NULL)){
	    
	  }
	  else if(   (strstr(curr_word,"/Entity" )!= NULL)){

	    container->entity[container->en_idx] = *my_entity;

	    container->en_idx++;
	    container->entity = realloc(container->entity, sizeof(en) * (container->en_idx+1));


	    printf("%d is the total attributes found!!!\n",my_entity->attributes->idx);
	    for(int i=0;i < my_entity->attributes->idx ;i++){
	      printf("Attr Name is %s \t  Description is %s \t type is %s !!!!!\n", my_entity->attributes->attribute[i].attr_name,my_entity->attributes->attribute[i].attr_description,my_entity->attributes->attribute[i].type); 
	    }
	    /* CALL THE GENERATORS HERE */

	    
/* 	    ent->entity[ent->idx].entity_name = malloc(sizeof(char)* strlen(my_entity->name)); */

/* 	    strcpy( ent->entity[ent->idx].entity_name, my_entity->name); */
/* ent->entity[ent->idx].sql_alchemy_end =0 ; */


	    g_sql_alchemy(container);

/* 	    ent->entity = realloc(ent->entity, sizeof(ent->entity)); */
/* 	    ent->idx++;	    */
	  }

	  else  if(strstr(curr_word, "Entity.Attributes") != NULL){
	    /* allocate memory to contain attribute */
	    my_entity->attributes->attribute = malloc(sizeof(attr) * 1);
	  }
	  
	  else if(strstr(curr_word,"Entity" ) != NULL){

	    /* create a new struct obj entity */
	    if( (my_entity = malloc(sizeof(en))) == NULL){
	      perror("MALLOC");
	    }
	    if( (my_entity->attributes = malloc(sizeof(attr_c))) == NULL){
	      perror("MALLOC");
	    }
	    my_entity->attributes->idx =0;

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
		int x=0,y=0,counter=0;
		while(parsed->value[y] != '\0'  && counter !=2){
		  if(parsed->value[y] != '"')
		  sanitised_value[x++] = parsed->value[y++];
		  else
		    {
		      counter++;
		      y++;
		    }
		}
		sanitised_value[x] ='\0';
		/* strncpy(sanitised_value, &parsed->value[1], (strlen(parsed->value))-2); */
		printf("TTTTTTTTTTTTT: %s \n",sanitised_value);
		if(strcmp(parsed->key, "Name") == 0){
		  my_entity->name  = malloc(sizeof(strlen(sanitised_value)));
		  strcpy(my_entity->name , sanitised_value);
		}

		else if(strcmp(parsed->key, "Description") == 0){
		  my_entity->description  = malloc(sizeof(strlen(sanitised_value)));
		  strcpy(my_entity->description , sanitised_value);
		}

	
		else if(strcmp(parsed->key, "Parent") == 0){
		  my_entity->parent  = malloc(sizeof(strlen(sanitised_value)));
		  strcpy(my_entity->parent , sanitised_value);
		}
	
		else if(strcmp(parsed->key, "ParentRelation") == 0){
		  my_entity->parent_relation  = malloc(sizeof(strlen(sanitised_value)));
		  strcpy(my_entity->parent_relation , sanitised_value);
		}
		else if(strcmp(parsed->key, "AttrName") == 0){
		  my_entity->attributes->attribute[my_entity->attributes->idx].attr_name = malloc(sizeof(strlen(sanitised_value)));
		  strcpy(my_entity->attributes->attribute[my_entity->attributes->idx].attr_name , sanitised_value);
		}
	
		else if(strcmp(parsed->key, "AttrDescription") == 0){
		  my_entity->attributes->attribute[my_entity->attributes->idx].attr_description = malloc(sizeof(strlen(sanitised_value)));
		  strcpy( my_entity->attributes->attribute[my_entity->attributes->idx].attr_description , sanitised_value);
		}
	
		else if(strcmp(parsed->key, "Type") == 0){
		  my_entity->attributes->attribute[my_entity->attributes->idx].type = malloc(sizeof(strlen(sanitised_value)));
		  strcpy(my_entity->attributes->attribute[my_entity->attributes->idx].type , sanitised_value);
		}
		else{
		  if(strcmp(parsed->value ,"True") == 0)
		    my_entity->persistent = 1;
		  else
		    my_entity->persistent =0;
		}

	      }
	    if(parsed->end){

	      /* 1) assign the attribute,
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
