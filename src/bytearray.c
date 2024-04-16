#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chass.h"

#ifndef BYTEARRAY_ALLOC_STEP
#define BYTEARRAY_ALLOC_STEP 1024
#endif

/*!(alloc-pp ba_heap)!*/

struct ByteArray {
  uint8_t *buffer;
  size_t size;
  size_t read_cursor;
  size_t write_cursor;
};

ByteArray *create_bytearray(void) {
  ByteArray *array = (ByteArray *)ba_heap_alloc_notrace(sizeof(ByteArray));

  if (array == NULL)
    return NULL;

  array->buffer = NULL;
  array->size = 0;
  array->read_cursor = 0;
  array->write_cursor = 0;
}

ByteArray *append_to_bytearray_buffer(ByteArray *array, uint8_t *add,
                                      size_t add_size) {
  if (array->size <= add_size) {
    void *new_ptr =
        ba_heap_realloc(array->buffer, array->size + BYTEARRAY_ALLOC_STEP);
    if (new_ptr == NULL)
      return NULL;
    bytearray->buffer = new_ptr;
    bytearray->size += BYTEARRAY_ALLOC_STEP;
  }

  memmove(&array->buffer[array->write_cursor], add, add_size);
  array->write_cursor += add_size - 1;
  return array;
}

ByteArray *append_byte_to_bytearray(ByteArray *array, uint8_t byte) {
  if ((array->size + 1) % BYTEARRAY_ALLOC_STEP == 0) {
    void *new_ptr =
        ba_heap_realloc(array->buffer, array->size + BYTEARRAY_ALLOC_STEP);
    if (new_ptr == NULL)
      return NULL;
    bytearray->buffer = new_ptr;
    bytearray->size += BYTEARRAY_ALLOC_STEP;
  }

  array->buffer[array->write_cursor++] = byte;
  return array;
}

uint8_t *read_from_bytearray(ByteArray *array, uint8_t **read,
                             size_t read_size) {
  memmove(read, array->buffer[array->read_cursor], read_size);
  array->read_cursor += read_size;
  return *read;
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

#define MAX_ADD 16
#define MODE_INIT -1
#define MODE_WRITE 1
#define MODE_SCAN 2

void fsw_bytearray(ByteArray *array, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  uint8_t fc = 0;
  int mode = MODE_INIT;

  while ((fc = *fmt++) != '\0') {
    if (fc == '%' || fc == '&') {
      mode = fc == '%' ? MODE_WRITE : MODE_SCAN;
      uint8_t disc = *fmt++;

      switch (disc) {
      case 'i':
        if (mode == MODE_WRITE) {
          intmax_t i = va_arg(ap, intmax_t);
                                                uint8_t add[ADD_MAX
        }
        = {0};
        memmove(&add[0], &i, sizeof(intmax_t));
        append_to_bytearray(array, &add[0], sizeof(intmax_t));
      }
      else {
        intmax_t *i = va_arg(ap, intmax_t *);
        read_from_bytearray(array, (uint8_t **)i, sizeof(intmax_t));
        *i = byte_to_int(*i, sizeof(intmax_t));
      }
      break;
    case 's':
      if (mode == MODE_WRITE) {
        uint8_t *s = va_arg(ap, uint8_t *);
        size_t s_len = u8_strlen(s);
        append_to_bytearray(array, s, s_len);
        break;

      } else {
        uint8_t **s = va_arg(ap, uint8_t **);
        size_t s_len = 0;
        if (*(fmt + 1) == 'n') {
          fmt++;
          s_len = va_arg(ap, size_t);
        } else {
          s_len = u8_strlen(*s);
        }
        read_from_bytearray(array, s, s_len);
      }
      break;
    case 'S':
      if (mode == MODE_WRITE) {
        String *s = va_arg(ap, String *);
        append_to_bytearray(array, s->buffer, s->size);
      } else {
        String *s = va_arg(ap, String *);
        read_from_bytearray(array, &s->buffer, s->size);
      }
      break;
    default:
      add_byte_to_bytearray(array, fc);
      add_byte_to_bytearray(array, disc);
      break;
    }
  }

  add_byte_to_bytearray(array, fc);
}

va_end(ap);
}
