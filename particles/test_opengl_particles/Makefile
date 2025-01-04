CC=gcc
CFLAGS=-Wall
SRC=window3d.c # mettre ici tous les fichier .c a executer
OBJ = ${SRC:.c=.o}
LDLIBS=-lGL -lGLU -lglut -lm

all: test

test:${OBJ}
	$(CC) $(CFLAGS) -o test ${SRC} ${LDLIBS}

debug:${OBJ}
	$(CC) $(CFLAGS) -g -fsanitize=address -o test ${SRC} ${LDLIBS}


.PHONY:clean all

clean:
	$(RM) *.o
	$(RM) test
