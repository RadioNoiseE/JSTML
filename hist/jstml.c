# include <stdio.h>
# include <uchar.h>
# include <string.h>
# include <stdbool.h>
# include <stdlib.h>
# include "jstml.h"

int main(int argc, char * argv[])
{
  bool outputdef = false;
  char option;
  char outfiledef[19];
  FILE * ifp;
  FILE * ofp;

  while (--argc > 0 && (*++argv)[0] == '-') {
    option = *++argv[0];
    switch (option) {
      case 'o':
        outputdef = true;
        break;
      default:
        printf("__FILE__: illegal option %c\n", option);
        argc = 0;
        break;
    }
  }

  if (argc != (outputdef ? 2 : 1)) {
    printf("Usage: jstml [-o file] file");
    exit(EXIT_FAILURE);
  } else {
    if (outputdef) {
      strcpy(outfiledef, argv[2]);
      ofp = fopen(argv[2], "w");
    }
    else {
      strcat(strncpy(outfiledef, argv[1], strspn(argv[1], ".")), ".tex");
      ofp = fopen(outfiledef, "w");
    }
    ifp = fopen(argv[(outputdef ? 3 : 1)], "r");
  }

  parse(ifp);
  tokenize(ofp);
  printf(">=> %s complied, output written to %s", argv[(outputdef ? 3 : 1)], outfiledef);

  exit(EXIT_SUCCESS);
}
