#include "hash_table.h"
#include <stdlib.h>

#define DEFAULT_SIZE 512
#define LOAD_FACTOR_THRESHOLD .75
#define KEY_INC(K, I, M) K = ((K % M) + ((I % M) * (I % M)) % M) % M

//key value pair
struct pair {
	void* k;
	void* v;
};

struct _hash_table {
	//hashing function
	unsigned int(*hash)(void*);
	//key comparator function
	int(*cmp)(void*,void*);
	//key copy function
	void*(*key_copy)(void*);
	//key free function
	void(*key_free)(void*);
	//key value pair bucket
	struct pair* slot;
	//size of bucket
	unsigned int size;
	//whether or not we free values
	int free_v;
	//total number of occupied slots in the bucket
	unsigned int occupied;
};

//allocate a hash table
hash_table ht_alloc(unsigned int(*hash)(void*), int(*cmp)(void*,void*), void*(*key_copy)(void*), void(*key_free)(void*), int free_v) {
	hash_table tbl = (hash_table)malloc(sizeof(struct _hash_table));
	tbl->hash = hash;
	tbl->cmp = cmp;
	tbl->key_copy = key_copy;
	tbl->key_free = key_free;
	tbl->slot = (struct pair*)(calloc(DEFAULT_SIZE,sizeof(struct pair)));
	tbl->free_v = free_v;
	tbl->size = DEFAULT_SIZE;
	tbl->occupied = 0;
}

/*
 *Lookup a value with the given key in the hash table
 */
void* ht_lookup(hash_table tbl, void* k) {
	unsigned int i, key = tbl->hash(k) % tbl->size;
	//for every slot in the hash table
	for(i = 0; i < tbl->size; ++i) {
		//quadratic probe to the correct slot
		KEY_INC(key, i, tbl->size);
		//if the slot is not empty and it contains our key
		if(tbl->slot[key].k != NULL) {
			if(tbl->cmp(tbl->slot[key].k, k) == 0) {
				return tbl->slot[key].v;
			}
		}
		//remove this clause if we can remove from hash tables
		else {
			return NULL;
		}
	}
	//we didn't find anything
	return NULL;
}

/*
 *Insert into a hash table
 */
void ht_insert(hash_table tbl, void* k, void* v) {
	unsigned int i, old_size, key = tbl->hash(k) % tbl->size;
	struct pair* old_slot;
	//for every possible slot in the hash table
	for(i = 0; i < tbl->size; ++i) {
		//quadratic probe to the correct slot
		KEY_INC(key, i, tbl->size);
		//if the slot is empty or it is occupied by our key, put ourkey and value in
		if(tbl->slot[key].k == NULL) {
			tbl->slot[key].k = tbl->key_copy(k);
			tbl->slot[key].v = v;
			break;
		}
		else if(tbl->cmp(tbl->slot[key].k, k) == 0) {
			tbl->slot[key].v = v;
			break;
		}
		
	}
	//increase the number of occupied slots
	tbl->occupied++;
	//if the load factor is above the threshold
	if((double)tbl->occupied / (double)tbl->size > LOAD_FACTOR_THRESHOLD) {
		//resize the table
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
	
}

/*
void ht_remove(hash_table tbl, void* k) {
	unsigned int i, key = tbl->hash(k) % tbl->size;
	for(i = 0; i < tbl->size; ++i) {
		KEY_INC(key, i, tbl->size);
		if(tbl->slot[key].k != NULL) {
			if(tbl->cmp(tbl->slot[key].k, k) == 0) {
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
}
*/

/*
 *Free a hash table
 */
void ht_free(hash_table tbl) {
	int i;
	for(i = 0; i < tbl->size; ++i) {
		if(tbl->slot[i].k != NULL) {
			tbl->key_free(tbl->slot[i].k);
		}
		if(tbl->free_v)
			free(tbl->slot[i].v);
	}
	free(tbl->slot);
	free(tbl);
}
