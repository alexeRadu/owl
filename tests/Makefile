all: multiwin

multiwin: multiwin.c
	gcc -o multiwin multiwin.c -lncurses

win-test.exe: win-test.c
	gcc -o win-test.exe win-test.c -lncurses

clean:
	-rm multiwin
