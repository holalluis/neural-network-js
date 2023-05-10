#!/bin/bash

#compile
make optim

#see sizes
ls -lh O*

#run all optimized executables and check time
for f in $(ls O*);do
  echo "=== executable $f ==="
  time -p ./$f # > /dev/null
done
