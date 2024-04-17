#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chass.h"

static struct option long_options[] = {{"file", required_argument, 0, 'f'},
                                       {"key", required_argument, 0, 'k'},
                                       {"obscure", no_argument, 0, 'o'},
                                       {0, 0, 0, 0}};

int main(int argc, char *argv[]) {
  int opt;
  int option_index = 0;

  while ((opt = getopt_long(argc, argv, "f:k:o", long_options,
                            &option_index)) != -1) {
    switch (opt) {
    case 'f':
      break;
    case 'k':
      break;
    case 'o':
      break;
    case '?':
    default:
      exit(EXIT_FAILURE);
    }
  }

  return 0;
}
