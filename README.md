# Simple neural network in javascript

Translation to Javascript and C of the code in the article
https://towardsdatascience.com/how-to-build-your-own-neural-network-from-scratch-in-python-68998a08e4f6
without dependencies.

To train and run the network using Javascript:
```shell
node nn.js
```

To train and run the network using C:
compile the file nn.c and run it with:
```shell
make run
```

## list of make commands
- make: just compile
- make run: compile and run
- make debug: compile with -g flag to debug the executable using `gdb`
- make wasm: compile using `emcc` compiler
- make optim: run different optimizations. Intended to use with the file `test_optimizations.sh`:
```shell
  bash test_optimizations.sh
```

## input format and requirements
- x and y are matrices.
- number of rows has to be the same
- numeric values in y have to be [0,1]
- numeric values in x can be negative
- example:
```txt
  x          y
  {0 0 1 1}  {0 1 0} 
  {0 1 1 0}  {1 1 0} 
  {1 0 1 1}  {1 0 0} 
  {1 1 1 3}  {0 0 0} 
  {1 1 1 5}  {1 0 0} 
```
