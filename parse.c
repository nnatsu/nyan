

char* chop (char*);
char** parse (char*);

char* chop(char* &input) {
  


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
  whilte(token) {
    get_rid_of_white_space(token);
    if (token ==";" || end_of_token(token) == ";") {
      j++; //move to next command
    }
  }
}
