CC=gcc
CFLAGS=-Wall
SRC=test.c # mettre ici tous les fichier .c a executer
OBJ = ${SRC:.c=.o}
LDLIBS=-lGL -lGLU -lglut -DLinux -D_REENTRANT

all: test

test:${OBJ}
	$(CC) $(CFLAGS) -o test ${SRC} ${LDLIBS}

debug:${OBJ}
	$(CC) $(CFLAGS) -g -fsanitize=address -o test ${SRC} ${LDLIBS}


.PHONY:clean all

clean:
	$(RM) *.o
	$(RM) test
