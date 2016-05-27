#include "assemble.h"

int main(int argc, char **argv) {

  readfile(); //TODO Finish this
  
  // We may assume that no line is longer than 511 characters;
  char *line = calloc(MAX_LINE_LENGTH + 1, sizeof(char));
  checkAllocError(line, (MAX_LINE_LENGTH + 1) * sizeof(char));
  
  // -- First pass
  
  for(int i = 0; file[i]; i++) {
    
    // End of line reached
    if(file[i] == '\n') {
      line[cIndex] = "\0";
      
      cIndex = 0;
    } else {
      line[cIndex] = file[i];
    }
    
    cIndex++;
  }
  
  // -- Second pass
  // The index of the current character in the line
  int cIndex = 0;
  
  for(int i = 0; file[i]; i++) {
    
    // End of line reached
    if(file[i] == '\n') {
      line[cIndex] = "\0";
      
      cIndex = 0;
    } else {
      line[cIndex] = file[i];
    }
    
    cIndex++;
  }
  
  return EXIT_SUCCESS;
}
