#include "hash_table.h"
#include <stdlib.h>

#define DEFAULT_SIZE 256

struct pair {
	void* k;
	void* v;
};

struct _hash_table {
	int(*hash)(void*);
	int(*cmp)(void*,void*);
	struct pair* slot;
	int size;
};

hash_table ht_alloc(int(*hash)(void*), int(*cmp)(void*,void*)) {
	hash_table tbl = (hash_table)malloc(sizeof(struct _hash_table));
	tbl->hash = hash;
	tbl->cmp = cmp;
	tbl->slot = (struct pair*)(calloc(DEFAULT_SIZE,sizeof(struct pair)));
}

void* ht_lookup(hash_table tbl, void* k) {
	int i, key = tbl->hash(k);
	for(i = 0, key = (key + (i * i)) % tbl->size; i < tbl->size; ++i) {
		if(tbl->slot[key].k != NULL && tbl->cmp(tbl->slot[key].k, k) == 0) {
			return tbl->slot[key].v;
		}
	}
}

void ht_insert(hash_table tbl, void* k, void* v) {
	int i, key = tbl->hash(k);
	for(i = 0, key = (key + i * i) % tbl->size; i < tbl->size; ++i) {
		if(tbl->slot[key].k == NULL) {
			tbl->slot[key].k = k;
			tbl->slot[key].v = v;
			break;
		}
		else if(tbl->cmp(tbl->slot[key].k, k) == 0) {
			tbl->slot[key].v = v;
			break;
		}
	}
	//increase table size and try again
}

void ht_remove(hash_table tbl, void* k) {
	int i, key = tbl->hash(k);
	for(i = 0, key = (key + i * i) % tbl->size; i < tbl->size; ++i) {
		if(tbl->slot[key].k != NULL && tbl->cmp(tbl->slot[key].k, k) == 0) {
			free(tbl->slot[key].k);
			free(tbl->slot[key].v);
			tbl->slot[key].k = NULL;
			tbl->slot[key].v = NULL;
			break;
		}
	}
}

void ht_free(hash_table tbl) {
	int i;
	for(i = 0; i < tbl->size; ++i) {
		free(tbl->slot + i);
	}
	free(tbl->slot);
	free(tbl);
}
