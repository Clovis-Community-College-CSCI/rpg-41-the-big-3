all: RPG

RPG: main.o menu.o game.o map.o
	g++ -g -fsanitize=address -o RPG main.o menu.o game.o map.o -lncurses

main.o: main.cc menu.h game.h
	g++ -g -fsanitize=address -c main.cc

game.o: game.cc game.h map.h actor.h inventory.h
	g++ -g -fsanitize=address -c game.cc

map.o: map.cc map.h
	g++ -g -fsanitize=address -c map.cc

clean:
	rm -f RPG main.o game.o map.o
