#include <stdio.h>

#define CHUNK_SIZE 4096

void compress(FILE* out, FILE* in, const char * name);
FILE* decompress(FILE* in);
