# Solve quadratic equations

This project is the first in the seria of projects, devoted to introduction to industrial programming.
The language of program is Russian. It can solve quadratic equation in real numbers.

## How to use

All you need to solve your equation is to use this function:
```C++
    int solve_square(double* x1, double* x2);
``` 
Here you should pass the values x1 and x2, which are the roots of the equation. If there is 1 root, then x1=x2, if the equation hass no roots, then x1 = x2 = 0
The function returns the number of roots.
