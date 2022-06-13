#!/bin/sh
CFLAGS="-Wall -Wextra -Werror"
CC=gcc
mkdir -p bin/
${CC} ${CFLAGS} src/cat.c -o bin/cat
${CC} ${CFLAGS} src/factor.c -lm -o bin/factor
${CC} ${CFLAGS} src/true.c -o bin/true
${CC} ${CFLAGS} src/false.c -o bin/false
${CC} ${CFLAGS} src/yes.c -o bin/yes