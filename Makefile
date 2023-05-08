default:
	gcc neural_network.c -lm -Wall

run:
	make
	./a.out

debug:
	gcc neural_network.c -lm -Wall -g
	gdb ./a.out

wasm:
	#emcc neural_network.c -sALLOW_MEMORY_GROWTH
	emcc neural_network.c

clean:
	rm a.out*
