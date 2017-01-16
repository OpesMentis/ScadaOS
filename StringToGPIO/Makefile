all: vigenere_exec gpio

default:
	all

vigenere.o: vigenere.c vigenere.h
	gcc -c vigenere.c -o vigenere.o

gpio.o: gpio.c
	gcc -c gpio.c -o gpio.o

gpio: gpio.o vigenere.o
	gcc -o gpio gpio.o vigenere.o

vigenere_exec:
	gcc vigenere_exec.c -o vigenere_exec

test:
	gcc test.c -o test_prog

clean:
	rm vigenere_exec gpio gpio.o vigenere.o test_prog

	


