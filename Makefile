all:
	gcc owl.c -o owl -lncurses

clean:
	rm owl
