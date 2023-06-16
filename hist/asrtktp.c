# include <stdio.h>
# include <uchar.h>
# include <assert.h>
# include "jstml.h"

void asstt (int * toktype)
{
  int tktpasrt = 0;
  
  while (*toktype != -6) {
    if (*toktype == TOKTYPE)
      tktpasrt = 1;
    toktype++;
  }

  assert(tktpasrt);
}
