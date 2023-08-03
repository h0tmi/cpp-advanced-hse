#pragma once

#include <stdlib.h>

/* clang-format off */

/*****************************************************************************
 * mySplineSnd() -- Arthur Taylor / TDL
 *
 * PURPOSE
 *    Given arrays x[0..n-1], y[0..n-1] containing a tabulated function
 * (ie y[i] = f(x[i]), with x[i] < x[i+1], and given the second derivative at
 * points 0, n - 1, this procedure returns an array y2[0..n-1] that contains
 * the second derivatives of the interpolating function at the x[i].
 *
 * ARGUMENTS
 *  x = The monitonic increasing x values (xa[i] < xa[i+1] not equal) (Input)
 *  y = The y-axis (y[i] = f(x[i])) (Input)
 *  n = The number of elements in x, y, y2 (Input)
 *  A = y's second derivative at y[0] (Input)
 *  B = y's second derivative at y[n0-1] (Input)
 * y2 = The resulting second derivatives (already allocated) (Output)
 *
 * RETURNS: int
 * -1 = Memory allocation error.
 * -2 = x is not monitonic increasing.
 *  0 = ok
 *
 * HISTORY
 * 11/1997 AAT Commented.
 * 10/2003 AAT Revisited.
 *  4/2007 AAT Adjusted indexing [0..n-1] instead of [1..n].
 *
 * NOTES
 *     Similar to spline, we were solving:
 * |2(dx2)   dx2        0         0       0   ||f''_1|     |df2/dx2 -   C   |
 *	| dx2  2(dx3+dx2)   dx3        0       0   ||f''_2| = 6*|df3/dx3 -df2/dx2|
 *	|  0      dx3    2(dx3+dx4)   dx4      0   ||f''_3|     |df4/dx4 -df3/dx3|
 *	|  0       0        dx4    2(dx4+dx5) dx5  ||f''_4|     |df5/dx5 -df4/dx4|
 *	|  0       0         0        dx5    2(dx5)||f''_5|     |  D     -df5/dx5|
 * where dx2 = x[1]-x[0], C = first deriv at x[0], D = first deriv at x[n-1].
 *
 * However in this case we know f''_1 (A) and f''_5 (B)
 * So we solve...
 *	|2(dx3+dx2)   dx3        0     ||f''_2| = 6*|df3/dx3 -df2/dx2 -1/6*dx2*A|
 *	|   dx3    2(dx3+dx4)   dx4    ||f''_3|     |df4/dx4 -df3/dx3           |
 *      |    0        dx4    2(dx4+dx5)||f''_4|     |df5/dx5 -df4/dx4 -1/6*dx5*B|
 *
 * See Numerical Recipes in C p51
 ****************************************************************************/
int mySplineSnd(const double *x, const double *y, size_t n, double A, // NOLINT
                double B, double *y2) // NOLINT
{
   size_t i;            /* loop index for matrix computation. */
   double *gam;
   double dx2;          /* Change in x on left */
   double dx3;          /* Change in x on right */
   double bet;          /* 2 * (dx2 + dx3) (center of matrix) */
   double r_i;          /* Right side of equation */

   if (n < 1) {
      return 0;
   }
   y2[0] = A;
   if (n == 1) {
      return 0;
   }
   if (n == 2) {
      y2[1] = B;
      return 0;
   }

   n -= 2;
   /* Allocate a vector of 0..n-1 inclusive. Note: a[i] = c[i-1] = dx */
   if ((gam = (double *)malloc(n * sizeof(double))) == NULL) { // NOLINT
      return -1;
   }

   dx2 = x[1] - x[0];
   dx3 = x[2] - x[1];
   r_i = 6 * ((y[2] - y[1]) / dx3 - (y[1] - y[0]) / dx2) - dx2 * A;
   if (n == 1) {
      r_i += -dx3 * B;
   }
   bet = 2 * (x[2] - x[0]);
   if (bet == 0.0) {
      free(gam);
      return -2;
   }
   y2[1] = r_i / bet;
   for (i = 1; i < n; ++i) {
      dx2 = x[i + 1] - x[i];
      dx3 = x[i + 2] - x[i + 1];
      r_i = 6 * ((y[i + 2] - y[i + 1]) / dx3 - (y[i + 1] - y[i]) / dx2);
      if (i == n - 1) {
         r_i += -dx3 * B;
      }
      gam[i] = dx2 / bet;
      /* I think: 1 > gam > 0 */
      bet = 2 * (x[i + 2] - x[i]) - dx2 * gam[i];
      /* I think: bet > 0 */
      if (bet == 0) {
         /* If x is monitonic increasing, then bet > dx2 => gam < 0 */
         /* => dx2 * gam < x[i + 2] - x[i] => bet > 0 */
         free(gam);
         return -2;
      }
      y2[i + 1] = (r_i - dx2 * y2[i]) / bet;
   }
   for (i = (n - 1); i >= 1; --i) {
      y2[i] -= gam[i] * y2[i + 1];
   }
   free(gam);
   /* I've subtracted 2 from n, so instead of y2[n - 1] we have y2[n + 1] */
   y2[n + 1] = B;
   return 0;
}

/*****************************************************************************
 * mySplintCube() -- Arthur Taylor / TDL
 *
 * PURPOSE
 *    Given arrays xa[0..n-1], ya[0..n-1] which tabulate a function with
 * xa[i] < xa[i+1], and given the array y2a[0..n-1] which is the second
 * second derivatives computed from "mySplineSnd()", and given a value of x,
 * this routine returns the cubic spline interpolated value y.
 *
 * ARGUMENTS
 *  xa = The monitonic increasing x values (xa[i] < xa[i+1] not equal) (Input)
 *  ya = The y-axis (y[i] = f(x[i])) (Input)
 * y2a = The second derivatives of f(x[i]) (computed using mySplineSnd()) (In)
 *  n = The number of elements in xa, ya, y2a (Input)
 *  x = The desired x-value to compute f(x) of (Input)
 *  y = The interpolated value at x (Output)
 *
 * RETURNS: int
 * -1 = Array length is 0
 * -2 = X is not monitonic increasing
 *  0 = Ok.
 *
 * HISTORY
 * 11/1997 AAT Commented.
 * 10/2003 AAT Revisited.
 *  4/2007 AAT Adjusted indexing [0..n-1] instead of [1..n].
 *
 * NOTES
 *   See Numerical Recipes in C p116
 ****************************************************************************/
int mySplintCube(const double *xa, const double *ya, const double *y2a, // NOLINT
                 size_t n, double x, double *y)
{
   int lo = 0;          /* Index just below x, or 0. */
   int hi = n - 1;      /* Index just above x, or n - 1. */
   int k;               /* Current midpoint in binary search. */
   double dx;           /* Change in x (xa[hi] - xa[lo]). */
   double a;            /* "upper" portion temp variable. */
   double b;            /* "lower" portion temp variable. */

   if (n == 0) {
      return -1;
   }
   /* Perform a binary search through xa to find values where: */
   /* 1) xa[lo] < x < xa[hi], and 2) hi - lo = 1 */
   while (hi - lo > 1) {
      k = (hi + lo) >> 1;
      if (xa[k] > x) {
         hi = k;
      } else {
         lo = k;
      }
   }
   dx = xa[hi] - xa[lo];
   if (dx == 0.0) {
      return -2;
   }
   a = (xa[hi] - x) / dx;
   b = (x - xa[lo]) / dx;
   *y = a * ya[lo] + b * ya[hi] +
         ((a * a * a - a) * y2a[lo] +
          (b * b * b - b) * y2a[hi]) * dx * dx / 6.0;
   return 0;
}
