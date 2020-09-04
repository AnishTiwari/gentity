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
int g_line = 0;
typedef struct store s;
typedef s* s_t;

typedef struct store_container sc;
typedef sc* sc_t;

struct store {
  char* entity_name;
 long  int sql_alchemy_end;
};

struct store_container {
  int idx;
  s_t entity;
};

sc_t ent;


/* given an entity name fine the end of its class in the file written to*/
int find_end(char* entity, sc_t ent){

  for(int i=0; i< ent->idx ;i++){
    printf("%s is being compared with %s",ent->entity[i].entity_name, entity);
    if(strcmp(ent->entity[i].entity_name, entity) == 0){
      
      return ent->entity[i].sql_alchemy_end;
    }
  }
 
    return -1;
    
}

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
    printf(" %d ) %s ", i, container->entity[i].name); 
    printf("Its Attribute are :::\n\n");
    for(int j=0; j < container->entity[i].attributes->idx ; j++){
      printf("%s \t %s \n",container->entity[i].attributes->attribute[j].attr_name, container->entity[i].attributes->attribute[j].attr_description);

    }
  }
  /*  opening file in append mode  */
  /* fp1 = fopen( SQL_ALCHEMY_FILE_NAME,"r+"); */
  /* /\* FILE* tmp; *\/ */
  /* /\* tmp = freopen(0, "r+b", fp1); *\/ */
  /* /\* if (tmp) fp1 = tmp; *\/ */
  
  /* printf("%s\n",_HEADER); */


  /* ent->entity[ent->idx].sql_alchemy_end += strlen(_HEADER); */
  /* /\* g_line += 2; *\/ */
  /* if(my_entity->description != NULL){ */
  /*   fprintf(fp1,"# %s\n",my_entity->description); */
  /* } */
  /* char* temp_name = malloc(sizeof(char)* (strlen(my_entity->name))) ; */
  /* strcpy(temp_name,my_entity->name); */
  /* to_upper(&temp_name[0]); */
  /* fprintf(fp1, "class %s(db.Model):\n",temp_name); */
  
  /* fprintf(fp1, "\t__tablename__ = %s", my_entity->name); */
  
  /* for(int i=0;i < my_entity->attributes->idx;i++){ */
  /*   if(my_entity->attributes->attribute[i].attr_description != NULL){ */
  /*     fprintf(fp1, "\n\t#%s",my_entity->attributes->attribute[i].attr_description); */
  
  /*   } */
  /*   fprintf(fp1, "\n\t%s = db.Column(db.Integer) ",my_entity->attributes->attribute[i].attr_name); */
    
  /*   /\* g_line += 2; *\/ */
  /* } */
  
  /* /\* add relationship when parent, parent_relation are set in entitytypes.xml *\/ */
  /* if(my_entity->parent != NULL){ */

  /*   /\* ONE TO ONE RELATION *\/ */
  /*   if(strcmp(my_entity->parent_relation ,"OneToOne") == 0) */
  /*     { */
  /* 	int end = find_end(my_entity->parent, ent); */
  /* 	if(end == -1){ perror("WRONG ENTIty:"); exit(0);} */

  /* 	/\* bringing file pointer to appropritae position before writing  *\/ */
  /* 	rewind(fp1); */
  /* 	fputs("\n000000000000000000000000", fp1); */
  /* 	fseek(fp1, end, SEEK_CUR); */
  /* 	fputs( "\n\t pop",fp1); */
  /* 	 /\* fseek(fp1, 0L, SEEK_END); *\/ */
  /* 	fprintf(fp1,"\n\t%s_id = db.Column(db.Integer,db.ForeignKey('%s.id'), unique=True)",my_entity->parent,my_entity->parent); */
	
  /*     } */

  /*   /\* ONE TO MANY *\/ */
  /*   else if(strcmp(my_entity->parent_relation, "OneToMany") == 0){ */

      
  /*   } */
  /*   /\* MANY TO MANY *\/ */
  /*   else{ */

  /*   } */
  /* } */


  /* fputs("\n\n",fp1); */


  /* printf("END IS %d, %ld",ent->idx, ent->entity[ent->idx].sql_alchemy_end); */
  
  /* fseek(fp1, 0L, SEEK_END); */
  
  /* long int f =ftell(fp1); */
  /* ent->entity[ent->idx].sql_alchemy_end = f; */
  /* fputs("---\n", fp1); */
  /* fclose(fp1); */
}

#endif
