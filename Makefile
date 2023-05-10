# make variables
CC     = gcc
OPTIM  = -O2
CFLAGS = -Wall -Wextra

# make "recipes"
# "target": "dependencies"
#   "actions"

nn: nn.c
	$(CC) $(CFLAGS) $^ $(OPTIM) -lm -o $@

run: nn
	./nn

debug:
	$(CC) $(CFLAGS) nn.c -lm -g -o nn
	gdb nn

wasm: nn.c
	#emcc neural_network.c -sALLOW_MEMORY_GROWTH
	emcc nn.c

optim:
	#compile O0, O1, O2, O3 and Os:
	$(CC) $(CFLAGS) nn.c -O0 -lm -o O0
	$(CC) $(CFLAGS) nn.c -O1 -lm -o O1
	$(CC) $(CFLAGS) nn.c -O2 -lm -o O2
	$(CC) $(CFLAGS) nn.c -O3 -lm -o O3
	$(CC) $(CFLAGS) nn.c -Os -lm -o Os

status:
	git diff --stat

push:
	git add *
	git commit
	git push

clean:
	rm -f nn a.out* O*
