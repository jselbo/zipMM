#include "bstr.h"
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_SIZE 1

struct _bstr {
	char* data;
	int len;
	int rlen;
};

bstr bstr_alloc() {
	bstr s = (bstr)malloc(sizeof(struct _bstr));
	s->data = (char*)malloc(DEFAULT_SIZE);
	s->len = 0;
	s->rlen = DEFAULT_SIZE;
}

int bstr_len(bstr s) {
	return s->len;
}

//copied from http://www.cse.yorku.ca/~oz/hash.html
unsigned int bstr_hash(void* ss) {
	bstr s = (bstr)ss;
	unsigned int hash = 5381;
	int i;
	for(i = 0; i < s->len; ++i) {
		hash = ((hash << 5) + hash) + s->data[i];
	}

	return hash;
}

int bstr_cmp(void* aa, void* bb) {
	bstr a = (bstr)aa, b = (bstr)bb;
	int i;
	if(a->len != b->len)
		return 1;
	else {
		for(i = 0; i < a->len; ++i) {
			if(a->data[i] != b->data[i]) {
				return 1;
			}
		}
		return 0;
	}
}

void* bstr_copy(void* ss) {
	int i;
	bstr s = (bstr)ss;
	bstr ret = (bstr)malloc(sizeof(struct _bstr));
	ret->data = (char*)malloc(s->rlen);
	ret->rlen = s->rlen;
	ret->len = s->len;
	for(i = 0; i < s->len; ++i) {
		ret->data[i] = s->data[i];
	}
	return ret;
}

void bstr_append(bstr s, char ch) {
	int i;
	char* newdata;
	if(s->len + 1 > s->rlen) {
		newdata = malloc(s->rlen * 2);
		for(i = 0; i < s->len; ++i) {
			newdata[i] = s->data[i];
		}
		free(s->data);
		s->data = newdata;
		s->rlen *= 2;
	}
	s->data[s->len] = ch;
	s->len++;
}

void bstr_free(bstr s) {
	free(s->data);
	free(s);
}

void bstr_free_ht(void* s) {
	bstr_free((bstr)s);
}

void bstr_print(bstr s) {
	int i;
	for(i = 0; i < s->len; ++i) {
		putchar(s->data[i]);
	}
}
