# include <stdio.h>
# include <uchar.h>
# include <string.h>
# include <stdbool.h>
# include "jstml.h"

# define SCANTOKEN scantok(&token, input)
# define OPTSPACE while (TOKTYPE == SPACE) scantok(&token, input)
# define OPTSPANL while (TOKTYPE == SPACE || TOKTYPE == NLINE) scantok(&token, input)
# define DARKMAGI printf("Unknown magic: I’m stymied.")
# define ARRYLITR (int [])
# define SEGMFCHK (MAXSTACK - strlen(psdtok) -1)
# define ASTEND -6

struct protok token;
char psdtok[MAXSTACK];
char multag[MAXTAG];
bool muline;

void parse(FILE * input)
{
  SCANTOKEN;
  if (TOKTYPE == EFILE) {
    fclose(input);
    return;
  } else
    OPTSPANL;

  asstt(ARRYLITR{BGROUP, ASTEND});
  SCANTOKEN;
  asstt(ARRYLITR{SPACE, MULBEG, PARATXT, ASTEND});
  OPTSPACE;
  if (TOKTYPE == PARATXT) {
    muline = false;
    strncat(psdtok, TOKTXT, SEGMFCHK);
  } else if (TOKTYPE == MULBEG) {
    SCANTOKEN;
    asstt(ARRYLITR{SPACE, PARATXT, ASTEND});
    OPTSPACE;
    if (TOKTYPE == PARATXT) {
      muline = true;
      strncpy(multag, TOKTXT, MAXTAG);
      strncat(psdtok, TOKTXT, SEGMFCHK);
    } else
      DARKMAGI;
  } else
    DARKMAGI;
  strncat(psdtok, deliopr, SEGMFCHK);

  SCANTOKEN;
  if (!muline) {
    asstt(ARRYLITR{SPACE, PARATXT, ASTEND});
    OPTSPACE;
    if (TOKTYPE == PARATXT) {
      strncat(psdtok, TOKTXT, SEGMFCHK);
      SCANTOKEN;
      asstt(ARRYLITR{NLINE, ASTEND});
    } else
      DARKMAGI;
  } else {
    asstt(ARRYLITR{SPACE, NLINE, PARATXT, ASTEND});
    OPTSPANL;
    while (TOKTYPE == PARATXT) {
      strncat(psdtok, TOKTXT, SEGMFCHK);
      SCANTOKEN;
      asstt(ARRYLITR{SPACE, NLINE, ASTEND});
      OPTSPANL;
    }
    asstt(ARRYLITR{BGROUP, ASTEND});
    OPTSPACE;
    asstt(ARRYLITR{MULBEG, ASTEND});
    OPTSPACE;
    asstt(ARRYLITR{PARATXT, ASTEND});
    if (TOKTXT != multag)
      printf("__FILE__: unmatched tag [WRN]");
    OPTSPACE;
    asstt(ARRYLITR{EGROUP, ASTEND});
  }

  strncat(psdtok, deliopr, SEGMFCHK);
  parse(input);
}
