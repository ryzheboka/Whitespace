# Compiles given .ws file, executes it multiple times and prints average execution time.
# It's intended to be used during optimization to compare different implementations.
# Problem: cannot handle programs which take input

#!/usr/bin/env bash

echo "Please enter the filename without extension (a .ws file is expected)"
read filename

python3 wsCompiler.py $filename.ws
nasm -fmacho64 $filename.asm && ld $filename.o

start=$(date +%s) # Problem: rounds to seconds

for value in {1..10000}
do
    [./a.out]  &>/dev/null  # run without showing the output
done

dur=$(echo "($(date +%s) - $start)/10000" | bc -l)
echo "Average execution time:"
echo $dur