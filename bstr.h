#ifndef BSTR_H
#define BSTR_H

struct _bstr;

typedef struct _bstr* bstr;

//alloc a bstr
bstr bstr_alloc();
//give the length of a binary string
int bstr_len(bstr s);
//hash a binary string
unsigned int bstr_hash(void* ss);
//compare two binary strings for equality (0 for equal and anything else for no)
int bstr_cmp(void* aa,void* bb);
//copy a bstr
void* bstr_copy(void* ss);
//append 1 character to a binary string
void bstr_append(bstr s, char ch);
//free a bstr
void bstr_free(bstr s);
//free a bstr hash
void bstr_free_ht(void* ss);
//print a bstr
void bstr_print(bstr s);
#endif
