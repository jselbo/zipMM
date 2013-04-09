#ifndef HASH_TABLE_H
#define HASH_TABLE_H

struct _hash_table;

typedef struct _hash_table * hash_table;

hash_table ht_alloc(int(*hash)(void*), int(*cmp)(void*, void*));
void* ht_lookup(hash_table tbl, void* k);
void ht_insert(hash_table tbl, void* k, void* v);
void ht_remove(hash_table tbl, void* k);
void ht_free(hash_table tbl);

#endif
