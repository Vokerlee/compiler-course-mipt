# Akinator

Now you are in directory of fascinating game Akinator!
Current version is for Windows NT in Russian.

AG — Akinator Game.

## Usage

All you should do to download the program is:

* To download and install [Graphviz for Windows](https://graphviz.org/download/#windows).
* To download current repository and launch `../../Akinator program/akinator.exe` (this version is experimental and doesn't work well on all OS).

There are 4 regimes of the program:

* To play game (where you can add your object to AG base).
* To show all differences between two objects in AG base.
* To describe all properties of the object.
* To show the base in graph interpretation.

## Compilation

All the code in this directory can be compiled by any IDE (the simplest variant) or using manual compilation by g++. But! Before compiling convert all files from `UTF-8` format to `CP1251`, otherwise the program will say some strange phrases, while text in console is similar to elven letters.

If you find a way to make the program workable (in Russian language) without converting all files to `CP1251` (so with initial `UTF-8` format), comment `#define UTF8_CONV` code-line in `akinator_config.h` file to make the 4th regime of game valid (because in such case there is no any necessarity to convert .dot file to UTF-8 format for graphviz).

Remember that to run the program some base should be exist, so write your initial base or use ready, otherwise you will get an error.
