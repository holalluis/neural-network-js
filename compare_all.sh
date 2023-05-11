#!/bin/bash

# Compare speed of:
#   - python
#   - javascript
#   - C
#   - C compiled to JS (wasm)

#python
echo "python nn.py"
time python nn.py >/dev/null

#javascript
echo "node nn.js"
time node nn.js >/dev/null

#C compiled to JS (wasm)
make wasm
echo "node a.out.js"
time node a.out.js >/dev/null

#C
make
echo "./nn"
time ./nn >/dev/null

#neteja
make clean

# RESULTS
#  python nn.py  | real 0m19.615s
#  node   nn.js  | real 0m35.489s
#  node a.out.js | real 0m5.313s
#  ./nn          | real 0m2.222s
