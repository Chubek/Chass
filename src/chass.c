#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chass.h"

static struct option long_options[] = {{"cfgfile", optional_argument, 0, 'f'},
                                       {"key", required_argument, 0, 'k'},
                                       {"clipboard", no_argument, 0, 'c'},
                                       {"help", no_argument, 0, 'h'},
                                       {0, 0, 0, 0}};

int main(int argc, char *argv[]) {
  int opt;
  int option_index = 0;

  char *config_file = CONFIG_FILE_DEFAULT;
  uint8_t *key = NULL;
  bool clipboard = false;

  while ((opt = getopt_long(argc, argv, "f:k:o", long_options,
                            &option_index)) != -1) {
    switch (opt) {
    case 'f':
      config_file = optarg;
      break;
    case 'k':
      key = optarg;
      break;
    case 'c':
      clipboard = true;
      break;
    case 'h':
      display_help_and_exit();
      break;
    default:
      break;
    }
  }

  return 0;
}
