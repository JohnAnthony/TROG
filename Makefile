CFLAGS+= -ggdb -O0 -std=c++0x -Wall -pedantic
OBJECTS= trog.o level.o game.o character.o entity.o enemy.o geometry.o tile.o
LIBS= -lncurses
CC= g++

trog: ${OBJECTS}
	${CC} ${CFLAGS} -o $@ ${OBJECTS} ${LIBS}

#Objects with a header file
%.o: %.cpp *.hpp
	${CC} ${CFLAGS} -c -o $@ $<

clean:
	-rm -f $(OBJECTS) trog

.PHONY: clean
