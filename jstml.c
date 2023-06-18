// ///////File jstml.c////////
// /part of the JSTML project/
// //the c standalone parser//
// /////author Jing Huang/////
// /////Copyright(c) 2023/////
// ////////MIT License////////

# include <uchar.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <assert.h>

# define MAXCHAR 10240
# define MAXTOK 8192
# define MAXTAG 512
# define MAXFLN 19
# define TOKTYPE token.catcode
# define TOKTXT token.param
# define SCANTOKEN scantok(&token)
# define OPTSPACE if (TOKTYPE == SPACE) scantok(&token)
# define OPTSPANL if (TOKTYPE == SPACE || TOKTYPE == NLINE) scantok(&token)
# define DARKMAGI printf("Unknown magic: I'm stymied.\n")
# define ARRYLITR (int [])
# define SEGMFCHK (MAXTOK - strlen(psd_token) - 1)
# define DELISTR ">"
# define DELICHR '>'
# define ASRTEND -6
# define TAG 1
# define TXT 0
# define EOT -1

struct protok
{
  int catcode;
  char param[MAXTOK];
};
struct protok token;

char file_tl[MAXCHAR];
 
void readfile(FILE * in_stream, char * pt_file_tl)
{
  int cnt, chr;

  while (cnt < MAXCHAR) {
    chr = fgetc(in_stream);
    if (chr == EOF) {
      *pt_file_tl++ = '\f';
      break;
    } else {
      *pt_file_tl++ = chr;
      cnt++;
    }
  }
  *pt_file_tl = '\0';

  fclose(in_stream);
  return;
}

int file_pos = 0;

char gettok(void)
{
  char chr;

  if (file_pos >= 0) {
    chr = file_tl[file_pos];
    file_pos++;
    return chr;
  }
}

void ungettok(char chr)
{
  if (file_pos >= (MAXCHAR - 1))
    printf("ungettok: buffer overflow [ERR]\n");
  else
    file_tl[--file_pos] = chr;
  return;
}

void creattok(struct protok * pt_token, int cc, char * pt_param)
{
  pt_token->catcode = cc;
  strncpy(pt_token->param, pt_param, (MAXTOK - 1));
  return;
}

enum
{
  SPACE,
  NLINE,
  EFILE,
  BGROUP,
  EGROUP,
  MULBEG,
  MULEND,
  PARATXT
};


void scantok(struct protok * pt_token)
{
  char tok, paratxt[MAXTOK], *pt_paratxt;
  pt_paratxt = paratxt;

  tok = gettok();
  switch (tok) {
    case ' ':
    case '\t':
      while ((tok = gettok()) == ' ' || tok == '\t');
      ungettok(tok);
      return creattok(pt_token, SPACE, " ");
    case '\n':
      while ((tok = gettok()) == '\n');
      ungettok(tok);
      return creattok(pt_token, NLINE, "\n");
    case '\f':
      return creattok(pt_token, EFILE, "\f");
    case '<':
      return creattok(pt_token, BGROUP, "<");
    case '>':
      return creattok(pt_token, EGROUP, ">");
    case '*':
      return creattok(pt_token, MULBEG, "*");
    case '/':
      return creattok(pt_token, MULEND, "/");
    default:
      for (*pt_paratxt++ = tok; (tok = gettok()) != '<' && tok != '>' && tok != '\n' && tok != '\f'; *pt_paratxt++ = tok);
      *pt_paratxt = '\0';
      ungettok(tok);
      return creattok(pt_token, PARATXT, paratxt);
  }
}

void asstt(int * pt_toktype)
{
  int tktp_asrt = 0;

  while (*pt_toktype != -6) {
    if (*pt_toktype == TOKTYPE) {
      tktp_asrt = 1;
      break;
    } else
      pt_toktype++;
  }

  assert(tktp_asrt);
 
  return;
}

char psd_token[MAXTOK] = "";
char multag[MAXTAG];
int muline = 0;

void parse(void)
{
  SCANTOKEN;
  OPTSPANL;

  if (TOKTYPE == EFILE)
    return;
  else
    OPTSPANL;

  asstt(ARRYLITR{BGROUP, ASRTEND});
  SCANTOKEN;
  asstt(ARRYLITR{SPACE, MULBEG, PARATXT, ASRTEND});
  OPTSPACE;

  if (TOKTYPE == PARATXT) {
    muline = 0;
    strncat(psd_token, TOKTXT, SEGMFCHK);
  } else if (TOKTYPE == MULBEG) {
    SCANTOKEN;
    asstt(ARRYLITR{SPACE, PARATXT, ASRTEND});
    OPTSPACE;
    if (TOKTYPE == PARATXT) {
      muline = 1;
      strncpy(multag, TOKTXT, (MAXTAG - 1));
      strncat(psd_token, TOKTXT, SEGMFCHK);
    } else
      DARKMAGI;
  } else
    DARKMAGI;
  SCANTOKEN;
  asstt(ARRYLITR{SPACE, EGROUP});
  OPTSPACE;
  strncat(psd_token, DELISTR, SEGMFCHK);

  SCANTOKEN;
  if (!muline) {
    asstt(ARRYLITR{SPACE, PARATXT, ASRTEND});
    OPTSPACE;
    if (TOKTYPE == PARATXT) {
      strncat(psd_token, TOKTXT, SEGMFCHK);
      SCANTOKEN;
      asstt(ARRYLITR{NLINE, ASRTEND});
    } else
      DARKMAGI;
  } else {
    asstt(ARRYLITR{SPACE, NLINE, PARATXT, ASRTEND});
    OPTSPANL;
    while (TOKTYPE == PARATXT) {
      strncat(psd_token, TOKTXT, SEGMFCHK);
      SCANTOKEN;
      asstt(ARRYLITR{NLINE, ASRTEND});
      OPTSPANL;
    }
    asstt(ARRYLITR{BGROUP, ASRTEND});
    SCANTOKEN;
    OPTSPACE;
    asstt(ARRYLITR{MULEND, ASRTEND});
    SCANTOKEN;
    OPTSPACE;
    asstt(ARRYLITR{PARATXT, ASRTEND});
    if (strcmp(TOKTXT, multag))
      printf("parse: unmatched tag [WRN]\n");
    SCANTOKEN;
    OPTSPACE;
    asstt(ARRYLITR{EGROUP, ASRTEND});
  }
  strncat(psd_token, DELISTR, SEGMFCHK);

  parse();
}

int status;
char texified_token[MAXTOK];

void texify(FILE * out_stream)
{
  char texout_tl[MAXTOK];

  if (status == TAG)
    strcpy(texout_tl, "\\jstml@tag{");
  else if (status == TXT)
    strcpy(texout_tl, "\\jstml@txt{");
  strncat(strncat(texout_tl, texified_token, (MAXTOK - strlen(texout_tl) - 1)), "}\n", (MAXTOK - strlen(texout_tl) - 1));

  fputs(texout_tl, out_stream);

  texified_token[0] = '\0';

  return;
}

void tokenize(FILE * out_stream)
{
  char tok;
  int num = 0, tok_pos = 0, cnt;

  while (++num) {
    for (cnt = 0; (tok = psd_token[tok_pos++]) != DELICHR; cnt++)
      texified_token[cnt] = tok;
    texified_token[cnt] = '\0';
    status = (num % 2);
    texify(out_stream);
    if (psd_token[tok_pos] == '\0')
      num = EOT;
  }

  fclose(out_stream);

  return;
}

int main(int argc, char * argv[])
{
  int output_default = 1;
  char option;
  char output_fname[MAXFLN];
  FILE * ifp;
  FILE * ofp;

  while (--argc > 0 && (*++argv)[0] == '-') {
    option = *++argv[0];
    switch (option) {
      case 'o':
        output_default = 0;
        break;
      default:
        printf("__FILE__: illegal option %c\n", option);
        argc = 0;
        break;
    }
  }

  if (argc != (output_default ? 1 : 2)) {
    printf("Usage: jstml [-o output] input\n");
    
    exit(EXIT_FAILURE);
  } else {
    if (output_default)
      strcat(strcpy(output_fname, *argv), ".tex");
    else
      strcpy(output_fname, *argv++);
    
    ofp = fopen(output_fname, "w");
    ifp = fopen(*argv, "r");
    
    readfile(ifp, file_tl);
    parse();
    tokenize(ofp);
    printf("=> %s parsed, output written to %s\n", *argv, output_fname);

    exit(EXIT_SUCCESS);
  }
}
