# Compiles given .ws file, executes it multiple times and prints average execution time.
# It's intended to be used during optimization to compare different implementations.
# Problem: cannot handle programs which take input

#!/usr/bin/env bash

echo "Please enter the filename without extension (a .ws file is expected)"
read filename

python3 wsCompiler.py $filename.ws
nasm -fmacho64 $filename.asm && ld $filename.o

echo "Does" $filename "use input? (enter 'yes' if so)"
read hasinput

if [ $hasinput = yes ]
then
    echo "Please enter the .exp program that handles your input"
    read filenameexp
    start=$(python -c 'import time; print time.time()')

    for value in {1..5000}
    do
        $filenameexp  &>/dev/null  # run without showing the output
    done
else

    start=$(python -c 'import time; print time.time()')

    for value in {1..5000}
    do
        [./a.out]  &>/dev/null  # run without showing the output
    done
fi
dur=$(echo "($(python -c 'import time; print time.time()') - $start)/5000" | bc -l)
echo "Average execution time:"
echo $dur