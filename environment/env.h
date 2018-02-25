#ifndef ENV_CS403
#define ENV_CS403
#include "../lexer/lexer.h"

Lexeme *create();
Lexeme *lookup(Lexeme *, Lexeme *);
Lexeme *update(Lexeme *, Lexeme *, Lexeme *);
Lexeme *insert(Lexeme *, Lexeme *, Lexeme *);
Lexeme *extend(Lexeme *, Lexeme *, Lexeme *);

void displayEnv(Lexeme *);

#endif
