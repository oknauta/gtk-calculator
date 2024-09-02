build:
	gcc `pkg-config gtk4 --cflags` ./src/*.c `pkg-config gtk4 --libs`