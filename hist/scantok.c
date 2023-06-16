# include <stdio.h>
# include <uchar.h>
# include "jstml.h"

void scantok(struct protok * ptoken, FILE * instream)
{
  int tok;
  char paratxt[MAXTOK];
  char *ptparatxt = paratxt;

  tok = gettok(instream);
  switch (tok) {
    case ' ':
    case '\t':
      while ((tok = gettok(instream)) == ' ' || tok == '\t');
      ungettok(tok);
      return creattok(ptoken, SPACE, " ");
    case '\n':
      return creattok(ptoken, NLINE, "\n");
    case EOF:
      return creattok(ptoken, EFILE, " ");
    case '<':
      return creattok(ptoken, BGROUP, "<");
    case '>':
      return creattok(ptoken, EGROUP, ">");
    case '*':
      return creattok(ptoken, MULBEG, "*");
    case '/':
      return creattok(ptoken, MULEND, "/");
    default:
      for (*ptparatxt++ = tok; (tok = gettok(instream)) != '<' && tok != '>' && tok != '\n' && tok != EOF; *ptparatxt++ = tok)
      ungettok(tok);
      *ptparatxt = '\0';
      return creattok(ptoken, PARATXT, paratxt);
  }
}
