CFLAGS+= -std=c++0x -Wall -pedantic -O0 -ggdb -Iinclude
OBJECTS= trog.o level.o game.o character.o enemy.o geometry.o tile.o item.o \
gui.o potion.o scrollable_menu.o treasure.o stattome.o equippable.o debug.o \
enemy_type.o
LIBS= -lncurses
SRCDIR=src
INCLUDEDIR=include
CXX ?= g++

trog: ${OBJECTS}
	${CXX} ${CFLAGS} -o $@ ${OBJECTS} ${LIBS}

#Objects with a header file
%.o: ${SRCDIR}/%.cpp ${INCLUDEDIR}/*.hpp
	${CXX} ${CFLAGS} -c -o $@ $<

clean:
	-rm -f $(OBJECTS) trog

install: trog
	cp -v trog /usr/bin/

.PHONY: clean
