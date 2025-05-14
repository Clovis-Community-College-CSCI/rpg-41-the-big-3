all: RPG

RPG: main.cc menu.cc game.cc
	g++ -o RPG main.cc menu.cc game.cc -lncurses

clean:
	rm -f RPG
