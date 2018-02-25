#include <stdio.h>
#include <stdlib.h>
#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../types/types.h"

int check(Parser *, char *);
void advance(Parser *);
Lexeme *match(Parser *, char *);
void matchNoAdvance(Parser *, char *);

char *type(Lexeme *);

int statementListPending(Parser *); 
int expressionPending(Parser *);
int initPending(Parser *);
int primaryPending(Parser *);
int parenExprPending(Parser *);
int idExprPending(Parser *);
int primitivePending(Parser *);
int operatorPending(Parser *);
int expressionListPending(Parser *); 
int blockPending(Parser *);
int statementPending(Parser *);
int ifStatementPending(Parser *);
int elsePending(Parser *); 
int whileLoopPending(Parser *);
int anonFuncPending(Parser *);
int parameterListPending(Parser *); 
int uOpPending(Parser *); 

Lexeme *statementList(Parser *);

Lexeme *statement(Parser *);

Lexeme *ifStatement(Parser *);
Lexeme *whileLoop(Parser *);
Lexeme *expression(Parser *);

Lexeme *block(Parser *);
Lexeme *elseStatement(Parser *);

Lexeme *primary(Parser *);
Lexeme *operator(Parser *);
Lexeme *init(Parser *);

Lexeme *primitive(Parser *);
Lexeme *idExpr(Parser *);
Lexeme *parenExpr(Parser *);
Lexeme *expressionList(Parser *);
Lexeme *anonFunc(Parser *);
Lexeme *uOp(Parser *);

Lexeme *parameterList(Parser *);

Lexeme *parse(FILE *fp) {
  Parser *p = malloc(sizeof(Parser));
  p->fp = fp;
  p->pending = lex(p);
  p->line = 1;
  p->out = stdout;
  Lexeme *tree = NULL;


  if(statementListPending(p)) {
    tree = statementList(p);
  }
  return tree;
}

int check(Parser *p, char *t) {
  return type(p->pending) == t;
}

void advance(Parser *p) {
  p->previous = p->pending;
  p->pending = lex(p);
}

Lexeme *match(Parser *p, char *t) {
  Lexeme *current = p->pending;
  matchNoAdvance(p,t);
  advance(p);
  return current;
}

void matchNoAdvance(Parser *p, char *t) {
  if(!check(p,t)) {
    printf("illegal\n");
    fprintf(stderr,"Syntax error\n");
    fprintf(stderr,"Expected type %s, found %s at line %d\n",t,displayLexeme(*(p->pending)),p->line);
    fprintf(stderr,"Previous type %s\n",displayLexeme(*(p->previous)));
    exit(1);
  }
}

// Helper Functions
char *type(Lexeme *l) {
  return l->type;
}

int typeEqual(Lexeme *a, Lexeme *b) {
  return strcmp(a->type,b->type) == 0;
}

// PENDING DEFINITIONS

int uOpPending(Parser *p) {
  return check(p,PLUS) || check(p,NOT);
}

int statementListPending(Parser *p) {
  return statementPending(p);
}

int statementPending(Parser *p) {
  return ifStatementPending(p) || whileLoopPending(p) || expressionPending(p);
}

int ifStatementPending(Parser *p) {
  return check(p,IF);
}

int whileLoopPending(Parser *p) {
  return check(p,WHILE);
}

int expressionPending(Parser *p) {
  return primaryPending(p) || check(p,VAR);
}

int elsePending(Parser *p) {
  return check(p,ELSE);
}

int blockPending(Parser *p) {
  return check(p,OBRACKET);
}

int primaryPending(Parser *p) {
  return primitivePending(p) || idExprPending(p) || parenExprPending(p) || check(p,OBRACKET) || anonFuncPending(p) || uOpPending(p);
}

int operatorPending(Parser *p) {
  return check(p,MINUS) || check(p,TIMES) || check(p,PLUS) || check(p,DIVIDE) || check(p,EXP) || check(p,MOD) || check(p,GREATER_THAN) || check(p,GREAT_EQ) || check(p,LESS_THAN) || check(p,LESS_EQ) || check(p,EQUAL) || check(p,NOT_EQUAL) || check(p,STRICT_EQ) || check(p,STRICT_NEQ) || check(p,OR) || check(p,AND) || check(p,XOR) || check(p,ASSIGN) || check(p,INCREMENT) || check(p,DECREMENT);
}

int initPending(Parser *p) {
  return check(p,ASSIGN);
}

int primitivePending(Parser *p) {
  return check(p,STRING) || check(p,INTEGER) || check(p,NIL);
}

int idExprPending(Parser *p) {
  return check(p,ID);
}

int parenExprPending(Parser *p) {
  return check(p,OBRACE);
}

int anonFuncPending(Parser *p) {
  return check(p,LAMBDA);
}

int parameterListPending(Parser *p) {
  return check(p,ID) || check(p,OBRACE);
}

int expressionListPending(Parser *p) {
  return expressionPending(p);
}
Lexeme *statementList(Parser *p) {
  Lexeme *a, *b = NULL;
  a = statement(p);
  if(statementListPending(p)) {
    b = statementList(p);
  }
  return cons(STATEMENTLIST,a,b);
}

Lexeme *statement(Parser *p) {
  if(ifStatementPending(p)) {
    return ifStatement(p);
  } else if(whileLoopPending(p)) {
    return whileLoop(p);
  } else {
    Lexeme *a = expression(p);
    match(p,SEMI);
    return a;
  }
}
Lexeme *ifStatement(Parser *p) {
  Lexeme *a,*b,*c;
  a = match(p,IF);
  match(p,OBRACE);
  a->left = expression(p);
  match(p,CBRACE);
  b = block(p);
  c = NULL;
  if(elsePending(p)) {
    c = elseStatement(p);
  }
  a->right = cons(JOIN,b,c);
  return a;
}

Lexeme *whileLoop(Parser *p) {
  Lexeme *a;
  a = match(p,WHILE);
  match(p,OBRACE);
  a->left = expression(p);
  match(p,CBRACE);
  a->right = block(p);
  return a;
}
Lexeme *expression(Parser *p) {
  Lexeme *a, *b, *c;
  if(primaryPending(p)) {
    a = primary(p);
    if(operatorPending(p)) {
      b = operator(p);
      c = expression(p);
      b->left = a;
      b->right = c;
      return b;
    }
    return a;
  } else {
    a = match(p,VAR);
    a->left = match(p,ID);
    a->right = NULL;
    if(initPending(p)) {
      a->right = init(p);
    } else {
      a->right = lexeme(NIL);
    }
    return a;
  }
}

Lexeme *uOp(Parser *p) {
  if(check(p,PLUS)) {
    return match(p,PLUS);
  } else {
    return match(p,NOT);
  }
}
Lexeme *block(Parser *p) {
  Lexeme *a = NULL;
  match(p,OBRACKET);
  if(statementListPending(p)) {
    a = statementList(p);
  }
  match(p,CBRACKET);
  return a;
}

Lexeme *elseStatement(Parser *p) {
  Lexeme *a = match(p,ELSE);
  if(check(p,IF)) {
    a = ifStatement(p);
  } else {
    a->left = block(p);
  }
  return a;;
}

Lexeme *primary(Parser *p) {
  if(primitivePending(p)) {
    return primitive(p);
  } else if(idExprPending(p)) {
    return idExpr(p);
  } else if(parenExprPending(p)) {
    return parenExpr(p);
  } else if(uOpPending(p)) {
    Lexeme *a = uOp(p);
    a->right = primary(p);
    return a;
  } else if(check(p,OBRACKET)) {
    Lexeme *a = lexeme(ARRAY);
    match(p,OBRACKET);
    if(expressionListPending(p)) {
      a->right = expressionList(p);
    }
    match(p,CBRACKET);
    return a;
  } else {
    return anonFunc(p);
  }
}

Lexeme *operator(Parser *p) {
  return match(p,p->pending->type);
}

Lexeme *init(Parser *p) {
  match(p,ASSIGN);
  return expression(p);
}

Lexeme *primitive(Parser *p) {
  if(check(p,STRING)) {
    return match(p,STRING);
  } else if(check(p,NIL)) {
      return match(p,NIL);
  } else {
    return match(p,INTEGER);
  }
}

Lexeme *idExpr(Parser *p) {
  Lexeme *a, *b = NULL, *e = NULL;
  a = match(p,ID);
  if(check(p,OBRACE)) {
    match(p,OBRACE);
    if(expressionListPending(p)) {
      b = expressionList(p);
    }
    match(p,CBRACE);
    return cons(FUNCCALL,a,b);
  } else if(check(p,OBRACKET)) {
    match(p,OPAREN);
    b = expression(p);
    match(p,CPAREN);
    return cons(ARRAYCALL,a,b);
  } else if(check(p,PERIOD)) {
    b = match(p,PERIOD);
    e = primary(p);
    b->left = a;
    if(!strcmp(e->type,FUNCCALL)) {
      b->right = e->left;
      e->left = b;
      return e;
    }
    b->right = e;
    return b;
  }
  return a;
}

Lexeme *parenExpr(Parser *p) {
  Lexeme *a, *b = NULL, *e = NULL;
  match(p,OBRACE);
  a = expression(p);
  match(p,CBRACE);
  if(check(p,OBRACE)) {
    match(p,OBRACE);
    if(expressionListPending(p)) {
      b = expressionList(p);
    }
    match(p,CBRACE);
    return cons(FUNCCALL,a,b);
  } else if(check(p,OBRACKET)) {
    match(p,OPAREN);
    b = expression(p);
    match(p,CPAREN);
    return cons(ARRAYCALL,a,b);
  } else if(check(p,PERIOD)) {
    b = match(p,PERIOD);
    e = primary(p);
    b->left = a;
    if(!strcmp(e->type,FUNCCALL)) {
      b->right = e->left;
      e->left = b;
      return e;
    }
    b->right = e;
    return b;
  }
  return a;
}

Lexeme *expressionList(Parser *p) {
  Lexeme *a, *b = NULL;
  a = expression(p);
  if(check(p,COMMA)) {
    match(p,COMMA);
    b = expressionList(p);
  }
  return cons(JOIN,a,b);
}

Lexeme *anonFunc(Parser *p) {
  Lexeme *a;
  a = match(p,LAMBDA);
  match(p,OBRACE);
  if(parameterListPending(p)) {
    a->left = parameterList(p);
  }
  match(p,CBRACE);
  a->right = block(p);
  return a;
}

Lexeme *parameterList(Parser *p) {
  Lexeme *a, *b = NULL;
  if(check(p,OBRACE)) {
    a = match(p,OBRACE);
    a->left = match(p,ID);
    match(p,CBRACE);
  } else {
    a = match(p,ID);
  }
  if(check(p,COMMA)) {
    match(p,COMMA);
    b = parameterList(p);
  }
  return cons(JOIN,a,b);
}

void displayTree(Lexeme *tree, char *head) {
  if(tree != NULL) {
    char *newHead = malloc(sizeof(char) * 1024);
    strcat(newHead,head);
    strcat(newHead,"\t");
    displayTree(tree->left,newHead);
    printf("%s%s\n",head,displayLexeme(*tree));
    displayTree(tree->right,newHead);
  }
}
