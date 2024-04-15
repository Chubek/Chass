#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "chass.h"

#ifndef MAX_BUCKETS
#define MAX_BUCKETS 4096
#endif

// Note: Preprocess with allocppx.pl
/*!(alloc-pp db_heap)!*/


struct Item {
	String *key;
	String *value;
	Item *next;
};

struct Hashtbl {
	Item *buckets[MAX_BUCKETS];
	ByteArray *sercon;	// Serialized Contents
};


Item *add_item(Hashtbl *tbl, String *key, String *value) {
	uint16_t idx = string_djb2(key);

	Item *node = (Item *)db_heap_alloc_notrace(sizeof(Item));
	
	if (node == NULL)
		error_out("Erro: Allocation failed");

	node->key = string_dup(key);
	node->value = string_dup(value);
	node->next = tbl->buckets[idx];
	tbl->buckets[idx] = node;

	return node;
}


Item *find_item(Hastbl *tbl, String *key) {
	uint16_t idx = string_djb2(key); 

	Item *current = NULL;

	for (current = tbl->buckets[idx]; current != NULL; current = current->next) {
		if (strings_are_equal(key, current->key))
			return current;
	}

	return NULL;
}
