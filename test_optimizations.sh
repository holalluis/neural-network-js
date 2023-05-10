#!/bin/bash

#compile O0 O1 O2 O3 and Os
make optim

#see sizes
ls -lh O*

#run all optimized executables and check time
for f in $(ls O*);do
  echo "=== executable $f ==="
  time -p ./$f # > /dev/null
done
