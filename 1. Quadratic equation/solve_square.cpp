#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>
#include <assert.h>
#include <float.h>

//-----------------------------------------------------------------------------
//! Solution to a square eqution ax2 + bx + c = 0
//! @param [in] x1   x1 - the first answer = 0
//! @param [in] x2   x2 - the second answer = 0
//! @param [out] roots   roots - nubmer of unique answers
//!
//! @version 228.1488
//! @authors Romaaaaaa
//!
//! @brief enters coeffients a,b,c; changes x1 and x2 from 0 to their answers accordingly and return the nubmer of unique answers, using square_eq_sol()
//-----------------------------------------------------------------------------

int solve_square(double* x1, double* x2);

//-----------------------------------------------------------------------------
//! iszero function
//! @param [in] value
//! @param [out] 1 / 0
//!
//! @version 12.44 beta
//! @authors Romaaaaaa
//!
//! @brief returns 1 if value is too small (accuracy about 1e-8) and 0 if not
//-----------------------------------------------------------------------------

int iszero(double value);

//-----------------------------------------------------------------------------
//! Solution to a square eqution ax2 + bx + c = 0
//! @param [in] a   a - coffecient
//! @param [in] b   b - coffecient
//! @param [in] c   c - coffecient
//! @param [in] x1   x1 - the first answer = 0
//! @param [in] x2   x2 - the second answer = 0
//! @param [out] roots  roots - nubmer of unique answers = 0
//!
//! @version 4.0
//! @authors Romaaaaaa
//!
//! @brief changes x1 and x2 from 0 to their answers accordingly and return the nubmer of unique answers
//-----------------------------------------------------------------------------

int square_eq_sol(double a, double b, double c, double* x1, double* x2);

//-----------------------------------------------------------------------------
//! Solution to a linear eqution ax + b = 0
//! @param [in] a   a - coffecient
//! @param [in] b   b - coffecient
//! @param [in] x1   x1 - the first answer = 0
//! @param [in] x2   x2 - the second answer = 0
//! @param [out] roots   roots = -1 if there are infinite roots, roots = 0 if there are not any root, roots = 1 if x1 = x2 = the single answer of equation
//!
//! @version 4.0
//! @authors Romaaaaaa
//!
//! @brief make x1 = x2 = answer of equation if it exists
//-----------------------------------------------------------------------------

int lin_eq_sol(double a, double b, double* x1, double* x2);

//-----------------------------------------------------------------------------
//! Test to the function square_eq_sol
//! @param [in] x1   x1 - the first answer  = 0
//! @param [in] x2   x2 - the second answer = 0
//!
//! @version 4.0
//! @authors Romaaaaaa
//!
//! @brief tests the function square_eq_sol
//-----------------------------------------------------------------------------

void solve_square_test(double* x1, double* x2);

//-----------------------------------------------------------------------------
//! Results of function square_eq_sol
//! @param [in] roots
//! @param [in] x1   x1 - the first answer = 0
//! @param [in] x2   x2 - the second answer = 0
//! @param [out] roots   roots = -1 if there are infinite roots, roots = 0 if there are not any root, roots = 1 if x1 = x2 = the single answer of equation
//!
//! @version 4.0
//! @authors Romaaaaaa
//!
//! @brief wrire the results to the function square_eq_sol, using roots - number of answers
//-----------------------------------------------------------------------------

void res_sq_sol(int roots, double* x1, double* x2);

int main()
{
    setlocale(LC_ALL, "Russian");

    double x1 = 0, x2 = 0;

    int roots = solve_square(&x1, &x2);
    assert(roots >= -1 && roots <= 2);

    res_sq_sol(roots, &x1, &x2);

    solve_square_test(&x1, &x2);

    return 0;
}

int solve_square(double* x1, double* x2)
{
    assert(x1 != NULL);
    assert(x2 != NULL);
    assert(isfinite(*x2));
    assert(isfinite(*x1));
    assert(isfinite(*x2));

    printf("Решаем за Вас квадратное уравнение бесплатно!\n");
    printf("Для начала введите три коэффициента квадратного уравнения a, b и c:\n");

    double a = 0, b = 0, c = 0;

    while (scanf("%lf %lf %lf", &a, &b, &c) != 3)
    {
        printf("Пожалуйста, введите именно числа, и именно три\n");
        fflush(stdin);
    }

    int roots = square_eq_sol(a, b, c, x1, x2);

    return roots;
}

int square_eq_sol(double a, double b, double c, double* x1, double* x2)
{
    assert(x1 != NULL);
    assert(x2 != NULL);
    assert(isfinite(a));
    assert(isfinite(b));
    assert(isfinite(c));

    if (iszero(a))
        return lin_eq_sol(b, c, x1, x2);
    else
    {
        double d = b * b - 4 * a * c;
        assert(isfinite(d));

        if ((iszero(d) == 0) && (d < 0))
        {
            *x1 = *x2 = 0;
            return 0;
        }
        else if (iszero(d))
        {
            if (iszero(-b / 2 / a))
                *x1 = *x2 = fabs(-b / 2 / a);
            else *x1 = *x2 = -b / 2 / a;
            return 1;
        }
        else if ((iszero(d) == 0) && (d > 0))
        {
            *x1 = (-b - sqrt(d)) / 2 / a;
            *x2 = (-b + sqrt(d)) / 2 / a;
            return 2;
        }
    }

    return 0;
}

int lin_eq_sol(double a, double b, double* x1, double* x2)
{
    assert(x1 != NULL);
    assert(x2 != NULL);
    assert(isfinite(a));
    assert(isfinite(b));

    if (iszero(a))
    {
        if (iszero(b))
        {
            *x1 = *x2 = 0;
            return -1;
        }
        else
        {
            *x1 = *x2 = 0;
            return 0;
        }
    }
    else
    {
        assert(isfinite(-b / a));

        if (iszero(-b / a))
            *x1 = *x2 = fabs(-b / a);
        else *x1 = *x2 = -b / a;

        return 1;
    }
}

void res_sq_sol(int roots, double* x1, double* x2)
{
    assert(x1 != NULL);
    assert(x2 != NULL);
    assert(isfinite(*x1));
    assert(isfinite(*x2));
    assert(roots >= -1 && roots <= 2);
    if (roots == 0)
        printf("К сожалению, корней нет\nВ переменные x1 и x2 помещены нули\n");
    else if (roots == 1)
        printf("Оба корня совпадают и равны %.8lf\n", *x1);
    else if (roots == 2)
        printf("Корни уравнения равны %.8lf и %.8lf\n", *x1, *x2);
    else if (roots == -1)
        printf("Множество корней уравнения бесконечно\nВ переменные x1 и x2 помещены нули\n");
}

int iszero(double value)
{
    assert(isfinite(value));

    if (fabs(value) < 1e-6)
        return 1;
    else
        return 0;
}

#define TEST_SUC(i, a, b, c, X1, X2)            \
    square_eq_sol(a, b, c, x1, x2);             \
    if (*x1 == X1 && *x2 == X2)                 \
        printf("ТЕСТ "#i" УСПЕШНО ЗАВЕРШЁН\n"); \
    else printf("ТЕСТ "#i" ЗАВАЛЕН\n");

void solve_square_test(double* x1, double* x2)
{
    assert(x1 != NULL);
    assert(x2 != NULL);

    TEST_SUC(1, 2, 3, 4, 0, 0)
    TEST_SUC(2, 2, 10, 0, -5, 0)
    TEST_SUC(3, 0, 0, 0.5, 0, 0)
    TEST_SUC(4, 0, 10, 0, 0, 0)
    TEST_SUC(5, 6, 6, 1.5, -0.5, -0.5)
    TEST_SUC(6, 3, 3, 3, 0, 0)
    TEST_SUC(7, 4, 14, 3, (-14 - sqrt(148)) / 8, (-14 + sqrt(148)) / 8)
    TEST_SUC(8, 1, 2, 1, -1, -1)
    TEST_SUC(9, 4, 100, 35, (-100 - sqrt(10000 - 16 * 35)) / 8, (-100 + sqrt(10000 - 16 * 35)) / 8)
    TEST_SUC(10, 2, 11, 0, -5.5, 0)
}
