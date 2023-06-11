# include <stdio.h>
# include <uchar.h>
# include <string.h>
# include "jstml.h"

# define TAG 1
# define TXT 0
# define EOT -1

int stat;
char texifiedtok[MAXSTACK];

void texify(FILE * outstream)
{
  char texouttl[MAXTOKOUT];
  if (stat == TAG) {
    strcpy(texouttl, "\\jstml@tag{");
    strncat(strncat(texouttl, texifiedtok, (MAXTOKOUT - strlen(texouttl) -1)), "}", ((MAXTOKOUT - strlen(texouttl) -1)));
  }
  else {
    strcpy(texouttl, "\\begin{jstmltxt}");
    strncat(strncat(texouttl, texifiedtok, (MAXTOKOUT - strlen(texouttl) -1)), "\\end{jstmltxt}", ((MAXTOKOUT - strlen(texouttl) -1)));
  }
  fputs(texouttl, outstream);
  fputc('\n', outstream);

  for (int cnt = 0; texifiedtok[cnt] != '\0'; cnt++)
    texifiedtok[cnt] = '\0';
}

void tokenize(FILE * outstream)
{
  char tok;
  int num = 0;

  while (++num) {
    for (int cnt = 0; (tok = psdtok[cnt]) != DELIOPR_CHAR && tok != '\0'; cnt++)
      strncat(texifiedtok, &tok, (MAXSTACK - strlen(texifiedtok) -1));
    stat = (num % 2);
    texify(outstream);
    if (tok == '\0')
      num = EOT;
  }

  fclose(outstream);
}
