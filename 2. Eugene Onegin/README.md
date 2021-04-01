# Sorting lines (Eugene Onegin)

This project is the second project in the seria of projects, devoted to introduction to industrial programming.
The language of program is Russian. It can sort lines of big texts in different regimes.

## How to use

First of all you should prepare ypur file with initial text. The second you should do is to come up with the name of result file.
So there are three different regimes of the program: rhyme-sort - the sort, ascending-sort and descending-sort (I home the names speak for themselves).
Also there is an additional option -- generator of ravings (or bredogenerator in Russian).

Here is the example of usage the program (the command is entered in the console):

`"Eugene Onegin.exe" source.txt result.txt rhyme bred+`

Here you should pass the values x1 and x2, which are the roots of the equation. If there is 1 root, then x1=x2, if the equation hass no roots, then x1 = x2 = 0.
The function returns the number of roots.
