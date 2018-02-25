#ifndef LEXER_CS403
#define LEXER_CS403
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct parser Parser;

typedef struct lexeme {
  char *type;
  char *sval;
  int ival;
  double rval;

  struct lexeme *left;
  struct lexeme *right;

  struct lexeme **aval;

  struct lexeme *(*builtin)(struct lexeme *,struct lexeme *);
} Lexeme;

Lexeme *lex(Parser *);
Lexeme *lexeme(char *);
Lexeme *cons(char *,Lexeme *, Lexeme *);
Lexeme *car(Lexeme *);
Lexeme *cdr(Lexeme *);
void setCar(Lexeme *, Lexeme *);
void setCdr(Lexeme *, Lexeme *);
char *displayLexeme(Lexeme);

#endif
