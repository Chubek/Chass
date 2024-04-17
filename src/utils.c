#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include "chass.h"

/*!(alloc-pp utils_heap)!*/

void trim(uint8_t *str) {
  uint8_t *end;

  while (isspace((unsigned uint8_t) * str))
    str++;

  if (*str == 0)
    return;

  end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned uint8_t) * end))
    end--;

  end[1] = '\0';
}

int strcasecmp(const uint8_t *s1, const uint8_t *s2) {
  while (*s1 &&
         tolower((unsigned uint8_t) * s1) == tolower((unsigned uint8_t) * s2)) {
    s1++;
    s2++;
  }
  return tolower((unsigned uint8_t) * s1) - tolower((unsigned uint8_t) * s2);
}

void safe_free(void **ptr) {
  if (ptr != NULL && *ptr != NULL) {
    free(*ptr);
    *ptr = NULL;
  }
}

void error_out(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  exit(EXIT_FAILURE);
}

uint8_t *read_file(const uint8_t *filename) {
  FILE *file = fopen(filename, "rb");
  if (!file)
    return NULL;

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);

  uint8_t *buffer = salloc(length + 1);
  if (!buffer) {
    fclose(file);
    return NULL;
  }

  fread(buffer, 1, length, file);
  fclose(file);
  buffer[length] = '\0';
  return buffer;
}

void debug_print(const uint8_t *msg) {
#ifdef DEBUG
  fprintf(stderr, "DEBUG: %s\n", msg);
#endif
}

intmax_t bytes_to_int(uint8_t *bytes, size_t size, bool be) {
  intmax_t target = 0;

  if (be) {
    for (size_t i = 0; i < size; i++) {
      target = (target << CHAR_BIT) | bytes[i];
    }
  } else {
    for (size_t i = 0; i < size; i++) {
      target |= ((intmax_t)bytes[i] << (i * CHAR_BIT));
    }
  }

  return target;
}
