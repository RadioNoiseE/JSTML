# define BUFSIZE 6
# define MAXTOK 60000
# define MAXTOKOUT 70000
# define MAXSTACK 66666
# define MAXTAG 19
# define TOKTYPE token.catcode
# define TOKTXT token.param
# define DELIOPR_CHAR '>'

enum {
  SPACE,
  NLINE,
  EFILE,
  BGROUP,
  EGROUP,
  MULBEG,
  MULEND,
  PARATXT
};

int gettok(FILE *);
void ungettok(int);

struct protok {
  int catcode;
  char param[MAXTOK];
};
void creattok(struct protok *, int, char *);

void scantok(struct protok *, FILE *);

void asstt(int *);

extern char psdtok[];
char * deliopr = ">";
void parse(FILE *);

void tokenize(FILE *);
extern struct protok token;
