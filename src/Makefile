#Macros
CC = gcc
CFLAGS = -Wall -ldl -lm
SRC = *.c *.h
OBJ = main.o sintactic.tab.o lex.yy.o ST.o commands.o

#Reglas explicitas
all: $(OBJ)
	$(CC) $(CFLAGS) -o FreeNoteMath $(OBJ)

clean:
	rm $(OBJ)

cleanall:
	rm $(OBJ) FreeNoteMath

#Reglas implicitas
main.o: main.c ST.h commands.h sintactic.tab.h
sintactic.tab.o: sintactic.tab.c sintactic.tab.h ST.h
lexx.yy.o: lex.yy.c lex.yy.h sintactic.tab.h ST.h
ST.o: ST.c ST.h sintactic.tab.h
commands.o: commands.h commands.c
