#include "parse.h"
#include "exec.h"

#define WHITESPACE " \n\t\r"

Cground chop(char* input) {
    char *block;
    size_t i;
    int quit = 0;
    Cground result;

    block = (char*)malloc(sizeof(input));
    
    for (i = 0; i < strlen(input) && !quit; i++) {
        if (input[i] == '\n' || input[i] == '\t' || input[i] == ';') {
            strncpy(block, input, i+1);
            quit = 1;
	    result.foreground = 0; //????
        }
	if (input[i] == '&') {
          printf("found&\n");
	  strncpy(block, input, i+1);
	  quit = 1;
	  result.foreground = 1; //??    ? ??????
	}
    }
    
    result.command = block;
    return result;
  
}



//input: string until user hits enter (\n \t what have you, included in input)
//output: cground
// changes input to whatever is left
int arglen;
char** parse (char* command) {
  // if job # not specific, store "-1" instead of #
  char* token;
  char* whole;
  char** list;
  char** buffer;
  int i=0, size; //i = command #

  whole = (char*) malloc(sizeof(command));
  strncpy(whole, command, 256);
  //whole = command;
  token = strtok(whole, WHITESPACE);
  list = malloc(1);
  buffer = malloc(1);
  size = (int) sizeof(list);
  
  
  while(token != NULL) {
    buffer = (char **) realloc(buffer, sizeof(char)*size + sizeof(token)); //reallocate more memory
    if (!buffer) exit(1);
    else list = (char **) buffer;
    
    list[i] = token;
    token = strtok(NULL, WHITESPACE);
    size += (int) sizeof(token);
    i++;
  }

  list = (char **) realloc(list, sizeof(char)*size+sizeof(token));
  list[i] = token; //NULL terminator
  arglen = i;
  return list;

}
