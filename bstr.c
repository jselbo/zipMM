#include "bstr.h"
#include <stdlib.h>

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
unsigned int bstr_hash(bstr s) {
	unsigned int hash = 5381;
	int i;
	for(i = 0; i < s->len; ++i) {
		hash = ((hash << 5) + hash) + s->data[i];
	}

	return hash;
}

int bstr_cmp(bstr a, bstr b) {
	int i;
	for(i = 0; i < a->len; ++i) {
		if(i >= b->len) {
			return 1;	
		}
		else if(i >= a->len) {
			return 1;
		}
		else if(a->data[i] != b->data[i]){
			return 1;
		}
	}
	if(b->len > a->len) {
		return 1;
	}
	else {
		return 0;
	}
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
