# include <stdio.h>
# include <uchar.h>
# include <string.h>
# include "jstml.h"

void creattok(struct protok * ptoken, int c, char * p)
{
  ptoken->catcode = c;
  strncpy(ptoken->param, p, (MAXTOK -1));
  return;
}
