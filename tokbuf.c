# include <stdio.h>
# include <uchar.h>
# include "jstml.c"

char buffer[BUFSIZE];
int bufpos = 0;

int gettok(FILE * instream)
{
  return (bufpos > 0) ? buffer[--bufpos] : fgetc(instream);
}

void ungettok(int c)
{
  if (bufpos >= BUFSIZE)
    printf("__FILE__: buffer overflow [ERR]");
  else
    buffer[bufpos++] = c;
}
