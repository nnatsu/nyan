#include "parse.h"

#define WHITESPACE " \n\t\r"

char* chop(char* input) {
    char *block;
    size_t i;
    int quit = 0;
    
    block = malloc(sizeof(input));

    for (i = 0; i < strlen(input) && !quit; i++) {
        if (input[i] == '\n' || input[i] == '\t' || input[i] == '&' || input[i] == ';') {

            strncpy(block, input, i+1);
            quit = 1;
        }
    }
    return block;
  
}



//input: string until user hits enter (\n \t what have you, included in input)
//output: cground
// changes input to whatever is left

char** parse (char* command) {
  // if job # not specific, store "-1" instead of #
  char* token;
  char** list;
  int i, size; //i = command #

  token = strtok(command, WHITESPACE);
  size = (int) sizeof(list);
  
  while(token != NULL) {

    list = (char **) realloc(list, sizeof(char)*size + sizeof(token)); //reallocation of more memory
    list[i] = token;
    token = strtok(NULL, WHITESPACE);
    size += (int) sizeof(token);
    i++;

  }

  list = (char **) realloc(list, sizeof(char)*size+sizeof(token));
  list[i] = token; //NULL terminator

  return list;

}
