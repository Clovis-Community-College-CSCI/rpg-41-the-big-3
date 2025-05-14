all: RPG

RPG: main.cc menu.cc game.cc map.cc
	g++ -o RPG main.cc menu.cc game.cc map.cc -lncurses

clean:
	rm -f RPG
