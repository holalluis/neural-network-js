default:
	gcc neural_network.c -lm -Wall

debug:
	gcc neural_network.c -lm -Wall -g
	gdb ./a.out

wasm:
	emcc neural_network.c -sALLOW_MEMORY_GROWTH

clean:
	rm a.out*
