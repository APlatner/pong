VERSION := 1.0.0
CPP := clang++
SRCDIR := src
BINDIR := bin
TARGET := ${BINDIR}/pong${VERSION}

SRCS := ${shell find ${SRCDIR} -type f -name *.cpp}
# OBJECTS := ${patsubst ${SRCDIR}/%, ${BLDDIR}/%, ${SRCS:.cpp=.o}}
CFLAGS := -std=c++17 -g -Wall -Wpedantic
LDFLAGS := -lsfml-window -lsfml-graphics -lsfml-system

.PHONY: run clean

run: ${TARGET}
	@echo " Running..."
	./${TARGET}

clean:
	rm -f ${TARGET}

${TARGET}: ${SRCS}
	@mkdir -p $(@D)
	${CPP} ${CFLAGS} $< -o $@ ${LDFLAGS}

