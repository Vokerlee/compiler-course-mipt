# Sorting lines (Eugene Onegin)

This project is the second project in the seria of projects, devoted to introduction to industrial programming.
The language of program is Russian. It can sort lines of big texts in different regimes.

## How to use

First of all you should prepare ypur file with initial text. The second you should do is to come up with the name of result file.
So there are three different regimes of the program: rhyme-sort - the sort, ascending-sort and descending-sort (I home the names speak for themselves).
Also there is an additional option - generator of ravings (or bredogenerator in Russian).

Here is the example of usage the program (the command is entered in the console):

`"Eugene Onegin.exe" source.txt result.txt rhyme bred+`

Except the `rhyme` regime, the regimes `ascending` and `descending` can be used. If you don't want to create bredogenerator file, use `bred-`.

## How it works

The main idea of the implementation of the program is that text is splitted in lines, which are sorted with `megasort`. This is the original way to sort the lines in the following way: at the beginning the `quicksort` is used, and when `LIM_QSORT` - number of the elements in recursion entry is reached, the `insertion` sort is activated to avoid costly recursive occurrences of quick sort. In practise it boosts the program by 30%

