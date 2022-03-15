# Akinator

Now you are in directory of fascinating game Akinator!
Current version is for Windows NT in Russian.

AG — Akinator Game.

## Usage

All you should do to download the program is:

* To download and install [Graphviz for Windows](https://graphviz.org/download/#windows).
* To download current repository.
* Launch `make.bat`, after that `build` directory will be created
* Assemble the project in `build` directory through `Cmake` and any compiler
* Everything is done and you can enjoy

There are 4 regimes of the program:

* To play game (where you can add your object to AG base).
* To show all differences between two objects in AG base.
* To describe all properties of the object.
* To show the base in graph interpretation.

## Compilation aspects

You download all sources in `UTF-8`, but `make.bat` generate `CP1251` sources, `UTF-8` you can find in `src_bak`. Also some important manipulations are done when laanching `make.bat`.
