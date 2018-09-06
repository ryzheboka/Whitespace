# Whitespace compiler for 64-bit macOS 
Translates programs from whitespace into x86_64 nasm assembly. 
wsCompiler.py creates an equivalent .asm file for the given .ws file. To run the .asm file, execute the command given at the top of the file (requires an up-to-date version of nasm).
Unclear parts of the standard are implemented by mimicking the behaviour of the [original implementation](https://web.archive.org/web/20150717140342/http://compsoc.dur.ac.uk:80/whitespace/download.php) (v 0.3).

## Why

It's a toy project to get learning experience in general and particularly to try out assembly. Inspired by [this very fun course](https://www.nand2tetris.org/).

## References
- [Specification of the whitespace language](https://web.archive.org/web/20151108084710/http://compsoc.dur.ac.uk:80/whitespace/tutorial.html) 

## List of currently implemented commands
- Push (maximum 32-bit integers)
- Duplicate
- Swap
- Discard
- OutputChar
- OutputNumber
- Add
- Subtract
- Multiply
- Divide
- Modulo (Difference to original implementation: remainder always has the sign of the dividend)
- Store 
- Retrieve
- InputNumber
- InputChar

Hopefully more coming.
