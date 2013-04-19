#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BITS 12                   /* Setting the number of bits to 12, 13*/
#define HASHING_SHIFT (BITS-8)    /* or 14 affects several constants.    */
#define MAX_VALUE (1 << BITS) - 1 /* Note that MS-DOS machines need to   */
#define MAX_CODE MAX_VALUE - 1    /* compile their code in large model if*/
                                  /* 14 bits are selected.               */
#if BITS == 14
  #define TABLE_SIZE 18041        /* The string table size needs to be a */
#endif                            /* prime number that is somewhat larger*/
#if BITS == 13                    /* than 2**BITS.                       */
  #define TABLE_SIZE 9029
#endif
#if BITS <= 12
  #define TABLE_SIZE 5021
#endif

void *malloc();

int *code_value;                  /* This is the code value array        */
unsigned int *prefix_code;        /* This array holds the prefix codes   */
unsigned char *append_character;  /* This array holds the appended chars */
unsigned char decode_stack[4000]; /* This array holds the decoded string */

/*
 * Forward declarations
 */
void compress(FILE *input,FILE *output);
//void expand(FILE *input,FILE *output);
int find_match(int hash_prefix,unsigned int hash_character);
void output_code(FILE *output,unsigned int code);
unsigned int input_code(FILE *input);
//unsigned char *decode_string(unsigned char *buffer,unsigned int code);

/********************************************************************
**
** This program gets a file name from the command line.  It compresses the
** file, placing its output in a file named test.lzw.  It then expands
** test.lzw into test.out.  Test.out should then be an exact duplicate of
** the input file.
**
*************************************************************************/

main(int argc, char *argv[])
{
FILE *input_file;
FILE *output_file;
FILE *lzw_file;
char input_file_name[81];

/*
**  The three buffers are needed for the compression phase.
*/
  code_value=(int*)malloc(TABLE_SIZE*sizeof(int));
  prefix_code=(unsigned int *)malloc(TABLE_SIZE*sizeof(unsigned int));
  append_character=(unsigned char *)malloc(TABLE_SIZE*sizeof(unsigned char));
  if (code_value==NULL || prefix_code==NULL || append_character==NULL)
  {
    printf("Fatal error allocating table space!\n");
    exit(-1);
  }
/*
** Get the file name, open it up, and open up the lzw output file.
*/
  if (argc>1)
    strcpy(input_file_name,argv[1]);
  else
  {
    printf("Input file name? ");
    scanf("%s",input_file_name);
  }
  input_file=fopen(input_file_name,"rb");
  int p = 0;
  char *new_file_name = (char*)malloc(sizeof(char)*strlen(input_file_name)-4);
  while(p<=strlen(input_file_name)){
    if(input_file_name[p] == '.'){
	int z = 0;
	while(z < p){
		new_file_name[z] = input_file_name[z];
		z++;
	}
	break;
    }
	p++;
  }

  strcat(new_file_name,".sigMM");
  lzw_file=fopen(new_file_name,"wb");
  if (input_file==NULL || lzw_file==NULL)
  {
    printf("Fatal error opening files.\n");
    exit(-1);
  };
/*
** Compress the file.
*/
  compress(input_file,lzw_file);
  fclose(input_file);
  fclose(lzw_file);
  free(code_value);
/*
** Now open the files for the expansion.
*/
  lzw_file=fopen("test.lzw","rb");
  output_file=fopen("test.out","wb");
  if (lzw_file==NULL || output_file==NULL)
  {
    printf("Fatal error opening files.\n");
    exit(-2);
  };

}

/*
** This is the compression routine.  The code should be a fairly close
** match to the algorithm accompanying the article.
**
*/

void compress(FILE *input,FILE *output)
{
unsigned int next_code;
unsigned int character;
unsigned int string_code;
unsigned int index;
int i;

  next_code=256;              /* Next code is the next available string code*/
  for (i=0;i<TABLE_SIZE;i++)  /* Clear out the string table before starting */
    code_value[i]=-1;

  i=0;
  printf("Compressing...\n");
  string_code=getc(input);    /* Get the first code                         */
/*
** This is the main loop where it all happens.  This loop runs util all of
** the input has been exhausted.  Note that it stops adding codes to the
** table after all of the possible codes have been defined.
*/
  while ((character=getc(input)) != (unsigned)EOF)
  {
    index=find_match(string_code,character);/* Replace with hash lookup */
    if (code_value[index] != -1)            
      string_code=code_value[index];        
    else                                    
    {                                       
      if (next_code <= MAX_CODE)
      {
        code_value[index]=next_code++;
        prefix_code[index]=string_code;
        append_character[index]=character; /* bstr append */
      }
      output_code(output,string_code);  /* When a string is found  */
      string_code=character;            /* that is not in the table*/
    }                                   /* I output the last string*/
  }                                     /* after adding the new one*/
/*
** End of the main loop.
*/
  output_code(output,string_code); /* Output the last code               */
  output_code(output,MAX_VALUE);   /* Output the end of buffer code      */
  output_code(output,0);           /* This code flushes the output buffer*/
  printf("\n");
}

/*
** This is the hashing routine.  It tries to find a match for the prefix+char
** string in the string table.  If it finds it, the index is returned.  If
** the string is not found, the first available index in the string table is
** returned instead.
*/

int find_match(int hash_prefix,unsigned int hash_character)
{
int index;
int offset;

  index = (hash_character << HASHING_SHIFT) ^ hash_prefix;
  if (index == 0)
    offset = 1;
  else
    offset = TABLE_SIZE - index;
  while (1)
  {
    if (code_value[index] == -1)
      return(index);
    if (prefix_code[index] == hash_prefix &&
        append_character[index] == hash_character)
      return(index);
    index -= offset;
    if (index < 0)
      index += TABLE_SIZE;
  }
}


/*
** The following two routines are used to output variable length
** codes.  They are written strictly for clarity, and are not
** particularyl efficient.
*/

unsigned int input_code(FILE *input)
{
unsigned int return_value;
static int input_bit_count=0;
static unsigned long input_bit_buffer=0L;

  while (input_bit_count <= 24)
  {
    input_bit_buffer |=
        (unsigned long) getc(input) << (24-input_bit_count);
    input_bit_count += 8;
  }
  return_value=input_bit_buffer >> (32-BITS);
  input_bit_buffer <<= BITS;
  input_bit_count -= BITS;
  return(return_value);
}

void output_code(FILE *output,unsigned int code)
{
static int output_bit_count=0;
static unsigned long output_bit_buffer=0L;

  output_bit_buffer |= (unsigned long) code << (32-BITS-output_bit_count);
  output_bit_count += BITS;
  while (output_bit_count >= 8)
  {
    putc(output_bit_buffer >> 24,output);
    output_bit_buffer <<= 8;
    output_bit_count -= 8;
  }
}
