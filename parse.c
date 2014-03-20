#include <string.h>

char* chop (char*);
char** parse (char*);

char* chop(char* &input) {
  int length, size;
  char *block;
  size_t i;
  int quit;

  quit = 0;

  for (i = 0; i < strlen(input)|| !quit; i++) {
    if (input[i] == '\n' || input[i] == '\t' || input[i] == '&' || input[i] == ';') {
      strncpy(block, input, i+1);
      strncpy(input, input+i, strlen(input)-i);
      quit = 1;
    }
 
  }
  return block;
  


  //input: string until user hits enter (\n \t what have you, included in input)
  //output: cground
  // changes input to whatever is left
}

char** parse (char* command) {
  // if job # not specific, store "-1" instead of #
  char* token;
  char** list;
  int i, j; //i = command #, j = token # for current command

  token = strtok(command, "");
  while(token) {
    get_rid_of_white_space(token);
    if (token ==";" || end_of_token(token) == ";") {
      j++; //move to next command
    }
  }
}
