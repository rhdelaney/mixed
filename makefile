CC := gcc
CFLAGS := -Wall -Wextra -O3 -Wno-unused-parameter
LDFLAGS := -lm
TARGET := dpl
OBJS := lexer.o parser.o types.o env.o eval.o mixed.o
DIRS := environment evaluation parser lexer types

.PHONY: clean

vpath %.c $(DIRS)
vpath %.h $(DIRS)

$(TARGET): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@

mixed.o: mixed.c

env.o: env.h

eval.o: eval.h

parser.o: parser.h

lexer.o: lexer.h

types.o: types.h

error1x: 
	dpl error1.mx

error1: 
	cat error1.mx

error2x: 
	dpl error2.mx

error2: 
	cat error2.mx

error3x: 
	dpl error3.mx

arraysx: 
	dpl arrays.mx

arrays: 
	cat arrays.mx

conditionals: 
	cat conditionals.mx

conditionalsx: 
	dpl conditionals.mx

recursion:
	cat recursion.mx

recursionx:
	dpl recursion.mx

iterationx:
	dpl iteration.mx

iteration:
	cat iteration.mx

functionsx:
	dpl functions.mx

functions:
	cat functions.mx

lambdax:
	dpl lambda.mx

lambda:
	cat lambda.mx

clean:
	rm -f $(OBJS) $(TARGET)