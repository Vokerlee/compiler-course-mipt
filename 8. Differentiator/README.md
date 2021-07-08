# Differentiator (derivative calculator)

Current project is for any derivative calculation in very detailed overview (in Russian language).

## Usage

All you should do to download the program is:
* To download and install [MikTex for Windows](https://miktex.org/download) if you don't have a LaTeX compiler yet (don't remember to add compiler to variables environment!!!)
* To download current repository and launch `../../Working program/diff.exe`.

You should write a formula in `formula.txt` file by analogy with example (in downloaded program). You should understand that in case of formula's error the program just will be closed with the error message, so check it to be valid. 

There are some simple laws how how to write a formula:
* Simple formula is like in simple calculator: `(x + 6)^(sin(x)) + 7 * x`.
* Variables can be of every name, except functions' names (see below).
* There are functions `ln`, `sin`, `cos`, `tg`, `ctg`, `sh`, `ch`, `th`, `cth`, `arcsin`, `arccos`, `arctg`, `arcctg`. Argument of function must be in `()`: `sin(cos(x))`.
* The derivative is taken with respect to the first variable in the formula (example is in `Working program` directory).

## Compilation

To compile the program just use IDE with all attached files or compile the program manually. There is no any subtleties.
