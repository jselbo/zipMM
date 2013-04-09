#ifndef BSTR_H
#define BSTR_H

struct _bstr;

typedef struct _bstr* bstr;

//alloc a bstr
bstr bstr_alloc();
//give the length of a binary string
int bstr_len(bstr s);
//hash a binary string
unsigned int bstr_hash(bstr s);
//compare two binary strings for equality (0 for equal and anything else for no)
int bstr_cmp(bstr a, bstr b);
//append 1 character to a binary string
void bstr_append(bstr s, char ch);
//free a bstr
void bstr_free(bstr s);
#endif
