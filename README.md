# Whitespace compiler for 64-bit macOS
Translates programs from whitespace into x86_64 nasm assembly. 
wsCompiler.py creates an equivalent .asm file for the given .ws file. To run the .asm file, execute the command given at the top of the file (requires an up-to-date version of nasm).
Unclear parts of the standard are implemented by mimicking the behaviour of the [original implementation](https://web.archive.org/web/20150717140342/http://compsoc.dur.ac.uk:80/whitespace/download.php) (v 0.2).

## Note on the repository structure
Assembly snippets equivalent to different Whitespace instructions are saved in the folder "translations". "start.txt" contains the header of the .asm file and "end.txt" contains functions used in the
body of the program.  WsCompiler.py, reads the Whitespace instructions, combines the corresponding snippets and replaces command arguments.

## Additional Whitespace interpreter in C
The Interpreter is not fully implemented yet, so it does't work.
Although a Whitespace interpreter in C has been already published on GitHub, I would like to try writing one by my own as a learning experience.

## Why

It's a toy project to get learning experience in general and particularly to try out assembly and c. Inspired by [this very fun course](https://www.nand2tetris.org/).

## References
- [Specification of the whitespace language](https://web.archive.org/web/20151108084710/http://compsoc.dur.ac.uk:80/whitespace/tutorial.html) 

## List of implemented commands 
- Push (Integer)
- Duplicate
- Swap
- Discard
- OutputChar
- OutputNumber
- Add
- Subtract
- Multiply
- Divide
- Modulo - difference to original implementation: remainder always has the sign of the dividend
- Store 
- Retrieve
- InputNumber
- InputChar 
- Label (String)
- Call (Label)
- Jump (Label)
- Jump If Zero (Label)
- Jump If Neg (Label)
- Return
- End
