#include "hash_table.h"
#include <stdlib.h>

#define DEFAULT_SIZE 256

struct pair {
	void* k;
	void* p;
};

struct _hash_table {
	int(*hash)(void*);
	struct pair* slot;
	int size;
};

hash_table ht_alloc(int(*hash)(void*)) {
	hash_table tbl = (hash_table)malloc(sizeof(struct _hash_table));
	tbl->hash = hash;
	tbl->slot = (struct pair*)(malloc(DEFAULT_SIZE * sizeof(struct pair)));
}

void* ht_lookup(hash_table tbl, void* k) {
	return NULL;
}

void* ht_insert(hash_table tbl, void* k, void* v) {
	return NULL;
}

void* ht_remove(hash_table tbl, void* k, void* v) {
	return NULL;
}

void ht_free(hash_table tbl) {
	int i;
	for(i = 0; i < tbl->size; ++i) {
		free(tbl->slot + i);
	}
	free(tbl->slot);
	free(tbl);
}
