#include "hash_table.h"
#include <stdlib.h>
#include <stdio.h>
#define DEFAULT_SIZE 512

struct pair {
	void* k;
	void* v;
};

struct _hash_table {
	int(*hash)(void*);
	int(*cmp)(void*,void*);
	struct pair* slot;
	int size;
	int free_k;
	int free_v;
};

hash_table ht_alloc(int(*hash)(void*), int(*cmp)(void*,void*), int free_k, int free_v) {
	hash_table tbl = (hash_table)malloc(sizeof(struct _hash_table));
	tbl->hash = hash;
	tbl->cmp = cmp;
	tbl->slot = (struct pair*)(calloc(DEFAULT_SIZE,sizeof(struct pair)));
	tbl->free_k = free_k;
	tbl->free_v = free_v;
	tbl->size = DEFAULT_SIZE;
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
	int i, key = tbl->hash(k), old_size;
	struct pair* old_slot;
	for(i = 0, key = (key + i * i) % tbl->size; i < tbl->size; ++i) {
		if(tbl->slot[key].k == NULL) {
			tbl->slot[key].k = k;
			tbl->slot[key].v = v;
			return;
		}
		else if(tbl->cmp(tbl->slot[key].k, k) == 0) {
			printf("cmp success\n");
			tbl->slot[key].v = v;
			return;
		}
	}
	old_slot = tbl->slot;
	old_size = tbl->size;
	tbl->slot = (struct pair*)calloc(tbl->size * 2, sizeof(struct pair));
	tbl->size *= 2;
	//rehash the table
	for(i = 0; i < old_size; ++i) {
		if(old_slot[i].k != NULL) {
			ht_insert(tbl, old_slot[i].k, old_slot[i].v);
		}
	}
	ht_insert(tbl, k, v);
	free(old_slot);
}

void ht_remove(hash_table tbl, void* k) {
	int i, key = tbl->hash(k);
	for(i = 0, key = (key + i * i) % tbl->size; i < tbl->size; ++i) {
		if(tbl->slot[key].k != NULL && tbl->cmp(tbl->slot[key].k, k) == 0) {
			if(tbl->free_k)
				free(tbl->slot[key].k);
			if(tbl->free_v)
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
		if(tbl->free_k)
			free(tbl->slot[i].k);
		if(tbl->free_v)
			free(tbl->slot[i].v);
	}
	free(tbl->slot);
	free(tbl);
}
