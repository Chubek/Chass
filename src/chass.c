#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chass.h"


static struct option 
long_options[] = {
	 {"cfg", optional_argument, 0, 'f'},
         {"key", required_argument, 0, 'k'},
	 {"req", required_argument, 0, 'r'},
         {"help", no_argument, 0, 'h'},
         {0, 0, 0, 0}
};



int main(int argc, char *argv[]) {
  while ((opt = getopt_long(argc, argv, "f:k:r:c", long_options,
                            &option_index)) != -1) {
    switch (opt) {
    case 'f':
      cfg = optarg;
      break;
    case 'k':
      key = optarg;
      break;
    case 'r':
      req = optarg;
      break;
    case 'h':
      display_help();
      return EXIT_FAILURE;
    default:
      warning_out("Warning: wrong arg %c", opt);
      break;
    }
  }

}
