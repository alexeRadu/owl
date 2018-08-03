owl: main.o line.o buffer.o
	gcc -o owl main.o line.o buffer.o -lncurses

main.o: main.c line.h buffer.h error.h
	gcc -c -o main.o main.c

line.o: line.c line.h
	gcc -c -o line.o line.c

buffer.o: buffer.c buffer.h error.h
	gcc -c -o buffer.o buffer.c

ncurses-test: ncurses-test.c
	gcc -o ncurses-test ncurses-test.c -lncurses

clean:
	-rm line.o
	-rm buffer.o
	-rm main.o
	-rm owl
