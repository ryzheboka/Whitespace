# Compiles given .ws file, executes it multiple times and prints average execution time.
# It's intended to be used during optimization to compare different implementations.
# Problem: cannot handle programs which take input

#!/usr/bin/env bash
echo "Does the program you want to run use input? (enter 'yes' if so)"
read hasinput

if [ $hasinput = yes ]
then
    echo "Sorry, that part isn't implemented yet"

else
    echo "Please enter the filename without extension (a .ws file is expected)"
    read filename

    python3 wsCompiler.py $filename.ws
    nasm -fmacho64 $filename.asm && ld $filename.o

    start=$(python -c 'import time; print time.time()')

    for value in {1..10000}
    do
        [./a.out]  &>/dev/null  # run without showing the output
    done
fi
dur=$(echo "($(python -c 'import time; print time.time()') - $start)/10000" | bc -l)
echo "Average execution time:"
echo $dur