nn: nn.c
	gcc -Wall nn.c -O3 -lm -o nn

run: nn
	./nn

debug: nn.c
	gcc -Wall nn.c -lm -g -o nn
	gdb nn

wasm: nn.c
	#emcc neural_network.c -sALLOW_MEMORY_GROWTH
	emcc nn.c

optim:
	gcc -Wall nn.c -O0 -lm -o O0
	gcc -Wall nn.c -O1 -lm -o O1
	gcc -Wall nn.c -O2 -lm -o O2
	gcc -Wall nn.c -O3 -lm -o O3
	gcc -Wall nn.c -Os -lm -o Os

clean:
	rm -f nn a.out* O*
