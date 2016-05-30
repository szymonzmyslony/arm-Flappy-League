#include "fileIO.h"

// reads a file and returns a pointer to a string of that file, which is heap
//   allocated, so will need to be freed after use
char * readFile(FILE *file, char *filename) {
  file = fopen(filename, "rb");
  if (file == NULL) {
    fprintf(stderr, "Error: cannot find file to read");
    exit(EXIT_FAILURE);
  }
  fseek(file, 0L, SEEK_END);
  long sizeInBytes = ftell(file);
  rewind(file);

  char *content = malloc(sizeInBytes + 1);
  if (content == NULL) {
    fprintf(stderr, "Error: Memory allocation for content failed");
  }
  content[sizeInBytes] = '\0';
  size_t num = fread(content, 1, sizeInBytes, file);
  if (num != sizeInBytes) {
    fprintf(stderr, "Error: File only partially read");
  }
  return content;
}
