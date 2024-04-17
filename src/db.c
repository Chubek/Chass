#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chass.h"

#ifndef MAX_BUCKET
#define MAX_BUCKET 4096
#endif

#define MAGIC_SIG "ChassDB\x01"
#define MAGIC_SIG_SIZE 8

// Note: Preprocess with allocppx.pl
/*!(alloc-pp db_heap)!*/

struct Item {
  String *key;
  String *value;
  Item *next;
};

struct Hashtbl {
  Item *buckets[MAX_BUCKET];
  ByteArray *sercon; // Serialized Contents
};

Item *add_item(Hashtbl *tbl, String *key, String *value) {
  uint16_t idx = string_djb2(key);

  Item *node = (Item *)db_heap_alloc_notrace(sizeof(Item));

  if (node == NULL)
    error_out("Error: Allocation failed");

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

void serialize_hashtbl(Hashtbl *tbl) {
  tbl->sercon = create_bytearray();

  if (tbl->sercon == NULL)
	  error_out("Error: Can't create bytearray");

  fws_bytearray(tbl->sercon, "%S%i", MAGIC_SIG, MAX_BUCKET);

  Item *bucket = NULL;
  for (size_t num_bucket = 0; num_bucket < MAX_BUCKET; num_bucket++) {
    bucket = tbl->bucket[num_bucket];
    fws_bytearray(tbl->sercon, "%i", num_bucket);
    while (bucket != NULL) {
      fws_bytearray(tbl->sercon, "%S%S%i", bucket->key, bucket->value,
                    num_bucket);
      bucket = bucket->next;
    }
    fws_bytearray(tbl->sercon, "%S%S%i", NULL_STR, NULL_STR, -num_bucket);
  }

  fws_bytearray(tbl->sercon, "%i", -1);
}

void deserialize_hashtbl(Hashtbl *tbl) {
  if (tbl->sercon == NULL)
    return;

  char magic_sig[MAGIC_SIG_SIZE + 1] = {0};
  size_t num_buckets = 0;
  fws_bytearray(tbl->sercon, "&s&i", &magic_sig[0], MAGIC_SIG_SIZE,
                &num_buckets);

  if (strncmp(&magic_sig, MAGIC_SIG, MAGIC_SIG_SIZE) != 0)
    error_out("Not a valid ChassDB file");

  if (num_bucket != MAX_BUCKETS)
    error_out("Number of buckets don't match");

  ssize_t num_bucket = 0;
  String *key, *value;
  Item **current_p = NULL, *current = NULL;

  while (num_bucket != -1) {
    fws_bytearray(tbl->sercon, "&i", &num_bucket);

    if (num_bucket > 0) {
      current_p = &tbl->buckets[num_bucket];
      *current_p = current = db_heap_alloc_notrace(sizeof(Item));
      size_t num_bucket_neg = 0;
      while (true) {
        fws_bytearray(tbl->sercon, "&s&s&i", &key, &value,
                      &num_bucket_neg);
        current->key = key;
        current->value = value;
        if (num_bucket_neg == -num_bucket)
          break;
        else {
          current->next = (Item *)db_heap_alloc_notrace(sizeof(Item));
          current = current->next;
        }
      }
    }
  }
}
