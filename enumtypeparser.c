#include "./enumtypeparser.h"

et_t prev_ = NULL;


void add_new_enum(ec_t ec_ ) {
  printf("Adding Enum %s to UTHASH\n", ec_->enum_name);
  HASH_ADD_STR( s1, enum_name, ec_ );
}

ec_t find_enum( char* key, ec_t ec_ ){
  HASH_FIND_STR(s1, key, ec_);
  if(ec_){
    ec_t temp_ec;
    temp_ec = ec_;
    return temp_ec;
  }
  else{
    print_enums();
    exit(1);
  }
}

void print_enums() {
  ec_t ss;

  for(ss=s1; ss != NULL; ss=ss->hh.next) {
    printf("Val is %s\n", ss->enum_name);
  }
}

/* util for writing to enum classes */
void write_enum_class(){
  ec_t ss;

  FILE* fp_e ;
  FILE *fp_ts;
  
  fp_e = fopen(ENUM_CLASS_FILE, "w+");
  fp_ts = fopen(ENUM_TS_MODEL, "w+");

  for(ss=s1; ss != NULL; ss=ss->hh.next) {

    fprintf(fp_e, "class %s:\n", ss->enum_name);
    fprintf(fp_ts, "enum %s{\n", ss->enum_name);

    et_t temp_et = ss->enums;

    while(temp_et != NULL){
      
      fprintf(fp_e, "\t%s = '%s'\n", temp_et->value, temp_et->value);
      fprintf(fp_ts, "\t%s,\n", temp_et->value);
      
      temp_et = temp_et->next;
    }
    
    fprintf(fp_e, "\n\n");
    fprintf(fp_ts, "}\n\n");
    
  }
  fclose(fp_e);
  fclose(fp_ts);
  printf("WRITTEN: Enum classes\n");
}

/* end of uthash lib fns: */
int enumtypeparse(){

  FILE * fp_ep;
  fp_ep = fopen(ENUMTYPE_FILE, "r");

  if( !fp_ep ){
    perror("No such file or content found !!\n");
    return 1;
  }

  int i = 0; 
  char curr_char;
  char curr_word[108] ;

  while((curr_char = fgetc(fp_ep)) != EOF ){

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

	  if( (strstr(curr_word, "/EnumTypes") != NULL)){
	    /* do nothing */
	  }
	  else if( (strstr(curr_word, "EnumTypes") != NULL)){
	  }
	  else if(strstr(curr_word, "/EnumType") != NULL){

	    /* assigning enum to enum container */
	    my_ec->enums = my_et;
	    add_new_enum(my_ec);

	  }

	  else if( (strstr(curr_word, "EnumType") != NULL)){
	    my_ec = malloc(sizeof(ec));
	    
	  }

	  else if(strstr(curr_word, "EnumVal" ) != NULL){
	    my_et  = malloc(sizeof(et));
	     
	  }
	  
	  else{
	    parsed = split(curr_word);
	   
	    if(parsed != NULL && parsed->key != NULL && parsed->value != NULL)
	      {
		char* sanitised_value;

		sanitised_value = malloc(sizeof(char) * strlen(parsed->value));
		int x=0, y=0, counter=0;
		while(parsed->value[y] != '\0'  && counter !=2){
		  if(parsed->value[y] != '"')
		    sanitised_value[x++] = parsed->value[y++];
		  else
		    {
		      counter++;
		      y++;
		    }
		}
		sanitised_value[x] = '\0';

		/* assigning values */
		if(strcmp(parsed->key , "Value") == 0){
		  my_et->value = malloc(sizeof(char) * strlen(sanitised_value));
		  strcpy(my_et->value, sanitised_value);

		  /* assign the pointer to next enum */
		  my_et->next = prev_;
		  prev_ = my_et;	  
		  
		}
		
	    	else if(strcmp(parsed->key , "Name") == 0){
		  my_ec->enum_name = malloc(sizeof(char) * strlen(sanitised_value));
		  strcpy(my_ec->enum_name, sanitised_value);

		}
	
	      }
	  }
	}
    }
  } /* end of file read  */
  
  return 0;
}
