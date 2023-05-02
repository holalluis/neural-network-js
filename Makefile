default:
	gcc -Wall -lm neural_network.c
	./a.out

debug:
	gcc -g -Wall -lm neural_network.c
	gdb ./a.out
