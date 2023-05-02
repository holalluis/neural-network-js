default:
	gcc neural_network.c -lm -Wall
	./a.out

debug:
	gcc neural_network.c -lm -Wall -g
	gdb ./a.out
