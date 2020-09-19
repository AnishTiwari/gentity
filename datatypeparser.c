#include "./datatypeparser.h"

void add_user(dt_t dt) {
  HASH_ADD_INT( s, name, dt );
}


dt_t find_key( char* key, dt_t dt ){
  HASH_FIND_STR(s, key, dt);
  if(dt){
    printf("FOUND !!! %s for key %s\n", dt->basetype, key);
    return dt;
  }
  else{
    print_users();
    printf("KEY ERROR -%s-\n\n", key);
    exit(1);
  }
}


void print_users() {
  dt_t ss;

  for(ss=s; ss != NULL; ss=ss->hh.next) {
    printf("user id -%s- : name %s\n", ss->name, ss->basetype);
  }
}
int datatypeparse(){


  FILE *fp;
  fp = fopen(DATATYPE_FILE, "r");

  if( !fp ){
    perror("No such file or content found !!\n");
    return 0;
  }

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
	  i=0;

	  if( (strstr(curr_word, "/DataTypes") != NULL)){

	    printf("ABOUT TO PRINT\n");
	    print_users();
	  }
	  else if( (strstr(curr_word, "DataTypes") != NULL)){
	  }

	  else if( (strstr(curr_word, "DataType") != NULL)){
	    printf("\n\nALLOCATING UTHASH STRUCT \n");
	    my_dt = malloc(sizeof(dt));
	    
	  }

	  else{

	    parsed = split(curr_word);
	   
	    if(parsed != NULL && parsed->key != NULL && parsed->value !=NULL)
	      {
		/* assign value to dt struct */

		if(strcmp(parsed->key, "BaseType") == 0){

		  printf("%s\n\n",parsed->value);
		  
		  my_dt->basetype = malloc(sizeof(char) * strlen(parsed->value));
		  strcpy(my_dt->basetype, parsed->value);
		}

		else if(strcmp("Name",parsed->key)==0){

		  my_dt->name = malloc(sizeof(char) * strlen(parsed->value));
		  strcpy(my_dt->name, parsed->value);
		}


		else if(strcmp("Length", parsed->key) == 0){
		  my_dt->length = malloc(sizeof(char) * strlen(parsed->value));
		  strcpy(my_dt->length, parsed->value);

		}
		
	      }



	    if(strstr(curr_word, "/>") != NULL){

	      printf("ADDING DATATYEP \n");
	      add_user(my_dt);

	      HASH_FIND_STR( s, "DummyInt", my_dt);;
	      if(my_dt){
		printf("%s \n\n\n--", my_dt->basetype);
	      }
	      else{
		abort();
	      }

	    }
	    
	  }
	}
    }
  }
  
  return 0;
}
