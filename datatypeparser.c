/* 
 * creates a hash tbale where key is the datatype 
 * found in the entitytypes.xml and value is the
 * datatypes.xml parsed value
 */

#include "./datatypeparser.h"

/* lib functions provided by uthash */
void add_user(dt_t dt) {
  HASH_ADD_STR( s, name, dt );
}

dt_t find_key( char* key, dt_t dt ){
  HASH_FIND_STR(s, key, dt);
  if(dt){
    dt_t temp_dt;
    temp_dt = dt;
    return temp_dt;
  }
  else{
    print_users();
    exit(1);
  }
}

void print_users() {
  dt_t ss;

  for(ss=s; ss != NULL; ss=ss->hh.next) {
    printf("user id -%s- : name %s\n", ss->name, ss->basetype);
  }
}
/* end of uthash lib fns: */

int datatypeparse(){

  FILE *fp;
  fp = fopen(DATATYPE_FILE, "r");

  if( !fp ){
    perror("No such file or content found !!\n");
    return 1;
  }

  int i = 0; 
  char curr_char;
  char curr_word[128] ;

  /* read the datatypes.xml file and parse it word by word*/
  while((curr_char = fgetc(fp)) != EOF ){

    if(curr_char != ' ' ){
      curr_word[i++] = curr_char;
    }
    /*  end of word reached */
    else {
      curr_word[i] = '\0';
      if(curr_word[0] == '\0'){
      }
      else
	{
	  i=0;

	  if( (strstr(curr_word, "/DataTypes") != NULL)){
	    /* do nothing */
	  }
	  else if( (strstr(curr_word, "DataTypes") != NULL)){
	  }

	  else if( (strstr(curr_word, "DataType") != NULL)){
	    my_dt = malloc(sizeof(dt));
	  }

	  else{

	    parsed = split(curr_word);
	   
	    if(parsed != NULL && parsed->key != NULL && parsed->value !=NULL)
	      {
		char* sanitised_value;

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

		/* assign value to dt struct */
		if(strcmp(parsed->key, "BaseType") == 0){
		  
		  my_dt->basetype = malloc(sizeof(char) * strlen(sanitised_value));
		  strcpy(my_dt->basetype, sanitised_value);
		}

		else if(strcmp("Name",parsed->key)==0){

		  my_dt->name = malloc(sizeof(char) * strlen(sanitised_value));
		  strcpy(my_dt->name, sanitised_value);
		}

		else if(strcmp("Length", parsed->key) == 0){
		  char* temp;
		  my_dt->length = strtol(sanitised_value, &temp, 10);

		}
		/* 		else if(strcmp("Rule", parsed->key) == 0){ */
		/*   my_dt->rule = malloc(sizeof(char) * strlen(sanitised_value)); */
		/*   strcpy(my_dt->rule, sanitised_value); */
		/* } */		
	      }


	    if(strstr(curr_word, "/>") != NULL){
	      /* Adding the struct to the uthash hash table */
	      add_user(my_dt);

	    }
	    
	  }
	}
    }
  }
  
  return 0;
}
