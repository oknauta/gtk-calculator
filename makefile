build:
	gcc `pkg-config gtk4 --cflags` main.c `pkg-config gtk4 --libs`