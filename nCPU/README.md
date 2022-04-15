# nCPU usage guide

nCPU is a version of stack-based assembly language (there is no command `mov`).

* [The usage of instruction set](#the-usage-of-instruction-set)
  * [Basics](#basics)
  * [Сalculations](#calculations)
  * [Jumps](#jumps)
  * [Functions](#functions)
  * [Memory usage](#memory-usage)
  * [Compilation && usage](#compilation--usage)

## The usage of instruction set

### Basics

As a default data type `double` type is used. The main feature of IS is that there is no command `mov`, but only `push` and `pop` to manipulate memory. There are only five registers (`rax`, `rbx`, `rcx`, `rdx`, `rex`). So here is some examples of usage such ISA features:

```asm
push 6.5  // push 6.5 into stack (here the comment can be)
// Simple comment
pop

push rax
pop rex
```

### Сalculations

There are a lot of arithmetic commands:

```asm
add     // pop 2 numbers from stack, add them and push in stack again
sub     // pop 2 numbers from stack, subtract them and push in stack again (top of stack is a subtrahend)
add     // pop 2 numbers from stack, multuply them and push in stack again
div     // pop 2 numbers from stack, divide them and push in stack again  (top of stack is a divider)

neg     // multiply the top of stack by -1

sqrt    // take the square root of the top of the stack
pow     // pop 2 numbers from stack, raises the first number to the power of the second 
exp     // changes the number on the top of the stack to its exponent value
ln      // natural logarithm
log10

mod            // pop 2 numbers from stack, leave еру remainder of one number from another
floor / ceil   // rounding a number on the top of the stack
min   / max    // leave the minimal/maximum value between two on the top of the stack

tg / cos / sin / ctg
th / ch  / sh  / cth
arccos / arcsin / arctg / arcctg

dump    // create dump-file for stack
out     // print the number on the top of the stack
hlt     // stop of executing instructions
in      // reading number from stdin and pushing it into stack
```

### Jumps

There are the following commands:

```asm
==================================
...
push 6
pop rcx

label1:

push rcx
push 0
ja :label1
...
=================================

jump :label_name  // simple jump command
ja   :label_name  // if the penultimate number in stack is more then on the top
jae  :label_name  // if the penultimate number in stack is more or equal then on the top
jb   :label_name  // if the penultimate number in stack is less then on the top
jbe  :label_name  // if the penultimate number in stack is less or equal then on the top
je   :label_name  // if the penultimate number in stack is equal to a number on the top
jne  :label_name  // if the penultimate number in stack is not equal to a number on the top

```

### Functions

After `call` instruction the return address is put on the special return addresses' stack, so such addresses cannot be overwritten by some registers. 

```asm
push 1
call :func1

hlt

func1:

pop rbx
out
push rbx

ret
```

### Memory usage

There is a 100000 elements' array of `double` type, which is like RAM.
Here is the example how to use it:

```asm
push [1000]
pop [99]

push [rbx]
pop [rdx]

push [rax + 54]
pop [rex - 87]
```

Remember, that like any CPU nCPU can call segmentation fault in case of invalid memory!

## Compilation && usage

You can easily build the project via `CMake` and desired build system.

As an example, let's consider the following: first create `proga.vasm` (.vasm is Vokerlee assembler in decryption) with such code:

```asm
push 7
call :factorial
dump
hlt

factorial:
pop rax
push rax

push rax
push 1
jbe :factorial_end

push rax
push 1
sub
call :factorial
mul

factorial_end:
ret
```

To compile this program you can do the following:

```bash
./nCPU/asm_compiler program.vasm program.ncpu
./nCPU/nCPU program.ncpu [results.txt]
```

Notice that `asm_compiler` creates `file_listing.txt` file.

All the results are in `results.txt`, also they are duplicated to `stdout`.

The program can be disassembled:

```bash
./nCPU/disasm program.ncpu program_disasm.vasm [disasm_listing.txt]
```
