#include "fileIO.h"

// reads a file and returns a pointer to a string of that file, which is heap
//   allocated, so will need to be freed after use
char *readFile(char *filename) {
  FILE *file;
  file = fopen(filename, "rb");
  if (file == NULL) {
    fprintf(stderr, "Error: cannot find file to read");
    exit(EXIT_FAILURE);
  }
  fseek(file, 0L, SEEK_END);
  long int sizeInBytes = ftell(file);
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
  fclose(file);
}

// adds bytes to the file at the given byte index
// numBytes should be less than or equal to the length of the array bytes
void addBytesToFile(char *filename, long int byteIndex, char *bytes, 
        int numBytes){
  FILE *file = fopen(filename, "rb+");
  if (file == NULL) {
    fprintf(stderr, "Error: cannot find file to write byte to");
    exit(EXIT_FAILURE);
  }
  fseek(file, byteIndex, SEEK_SET);
  size_t num = fwrite(bytes, 1, numBytes, file);
  if (num != numBytes) {
    fprintf(stderr, "Error: File only partially written to");
  }

  fclose(file);
}

// appends bytes to the end of a file
// numBytes should be less than or equal to the length of the array bytes
// returns the new end of the file
long int appendBytes(char *filename, char *bytes, int numBytes){
  FILE *file = fopen(filename, "ab");
  if (file == NULL) {
    fprintf(stderr, "Error: cannot find file to write byte to");
    exit(EXIT_FAILURE);
  }
  // writing will move pointer to end of file
  size_t num = fwrite(bytes, 1, numBytes, file);
  if (num != numBytes) {
    fprintf(stderr, "Error: File only partially written to");
  }
  fseek(file, 0L, SEEK_END);
  long int newSize = ftell(file);
  fseek(file, 0L, SEEK_SET);
  fclose(file);
  return newSize;
}
