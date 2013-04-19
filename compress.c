#include "compress.h"
#include "bstr.h"
#include <arpa/inet.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


void compress(FILE* out, FILE* in, const char * name) {
	char buffer[CHUNK_SIZE];
	int count; int i;
	int32_t fileSz = 0, nameSz = 0;
	//writing the header
	//1: Size of file name (4 bytes)
	nameSz = htonl(strlen(name));
	fwrite(&nameSz, sizeof(int32_t), 1, out);
	//2: File name
	fwrite(name, 1, strlen(name), out);
	//TODO: Actually compress the file

	//ht_alloc();

	/* Initialize hash table with 0-255 ASCII chars */
	/*
	for(i=0;i<256;i++){
		ht_insert(_hash_table, (char)i, i);	
	}
	*/

	bstr bString;
	char inputchar;

	int string_code = getc(in);

	/*
	while((inputchar = getc(in)) != (unsigned)EOF){

	}
	*/


	//3: Size of file content (4 bytes)
	fseek(in, 0, SEEK_END);
	fileSz = htonl(ftell(in));
	fwrite(&fileSz, sizeof(int32_t), 1, out);
	fseek(in, 0, SEEK_SET);
	//4: File Content
	while(!feof(in)) {
		count = fread(buffer, 1, CHUNK_SIZE, in);
		fwrite(buffer, 1, count, out);
	}
}

void decompress(FILE* in) {
	char buffer[CHUNK_SIZE], *fileName;
	int count;
	int32_t nameSz, contentSz;
	FILE* out;
	while(!feof(in)) {
		//reading the header
		//1: Size of file name (4 bytes)
		fread(&nameSz, sizeof(int32_t), 1, in);
		nameSz = ntohl(nameSz);
		if(feof(in)) {
			return;
		}
		//2: File name
		fileName = (char*)malloc(nameSz);
		fread(fileName, 1, nameSz, in);
		out = fopen(fileName, "wb+");
		if(out == NULL) {
			printf("Decompression Error: Could not create file %s\n", fileName);
			return;
		}
		free(fileName);
		//TODO: Actually decompress the file
		//3: Size of file content (4 bytes)
		fread(&contentSz, sizeof(int32_t), 1, in);
		contentSz = ntohl(contentSz);
		//4: Actual content
		while(contentSz > 0) {
			count = fread(buffer, 1, contentSz, in);
			fwrite(buffer, 1, count, out);
			contentSz -= count;
		}
		fclose(out);
	}
}
