#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chass.h"

#ifndef BYTEARRAY_ALLOC_STEP
#define BYTEARRAY_ALLOC_STEP 1024
#endif

/*!(alloc-pp barr_heap)!*/

struct ByteArray {
  uint8_t *buffer;
  size_t size;
  size_t cursor;
};

ByteArray *create_bytearray(void) {
  ByteArray *array = (ByteArray *)barr_heap_alloc_notrace(sizeof(ByteArray));

  if (array == NULL)
    return NULL;

  array->buffer = NULL;
  array->size = 0;
  array->cursor = 0;
}

ByteArray *read_filepath_into_bytearray(char *path) {
  if (path == NULL)
    return NULL;

  ByteArray *array = create_bytearray();

  if (array == NULL)
    return NULL;

  FILE *stream = fopen(path, "r");

  if (stream == NULL)
    return NULL;

  if (fseek(stream, 0, SEEK_END) < 0) {
    fclose(stream);
    return NULL;
  }

  if (rewind(stream) < 0) {
    fclose(stream);
    return NULL;
  }

  if ((array->size = ftell(stream)) < 0) {
    fclose(stream);
    return NULL;
  }

  array->buffer = (uint8_t *)ba_heap_alloc(array->size * sizeof(uint8_t));

  if (fread(array->buffer, array->size, sizeof(uint8_t), stream) !=
      array->size) {
    fclose(stream);
    return NULL;
  }

  fclose(stream);
  return array;
}
