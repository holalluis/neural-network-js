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

#neteja
make clean

# RESULTS
#  executable O0 | real 4.16 user 4.16 sys 0.00 | real 3.33 user 3.32 sys 0.00
#  executable O1 | real 2.35 user 2.34 sys 0.00 | real 1.70 user 1.68 sys 0.01
#  executable O2 | real 2.25 user 2.25 sys 0.00 | real 1.64 user 1.61 sys 0.01
#  executable O3 | real 2.23 user 2.22 sys 0.00 | real 1.59 user 1.57 sys 0.00
#  executable Os | real 2.52 user 2.51 sys 0.00 | real 1.88 user 1.86 sys 0.00
