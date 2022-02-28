#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gsl/gsl_sf_expint.h>
#include "DE_trans.h"

#define ALPHA 0.99
#define DOMD 1.57

double f(double t)
{
  return 0.25 * ((1+t) * log1p(t) + (1-t) * log1p(-t) - 2 * M_LN2) / M_LN2;
}

double G(double x)
{
  return 0.25 * M_PI * sinh(x) * DE_trans_div(-1, 1, x) / M_LN2;
}

double J(int j, double h, double x)
{
  return h * (0.5 + M_1_PI * gsl_sf_Si(M_PI * (x / h - j)));
}

double Fapp(double t, int m)
{
  double h = log(2*DOMD*m / ALPHA) / m;

  int j;
  double sum1 = 0;
  double sum2 = 0;

  for (j = -m; j <  0; j++) {
    sum1 += G(j*h) * J(j, h, DE_trans_inv(-1, 1, t));
  }
  for (j =  m; j >= 0; j--) {
    sum2 += G(j*h) * J(j, h, DE_trans_inv(-1, 1, t));
  }

  return (sum1 + sum2);
}

int main()
{
  int i;
  int n;
  double t;
  double err, maxerr;
  clock_t start, end;
  double time;

  int STEP = 1000;

  for (n = 3; n <= 50; n += 6) {
    start = clock();

    maxerr = 0;
    for (i = - STEP + 1; i <= STEP - 1; i++) {
      t = (double)i / (double)(STEP);

      err = fabs(f(t) - Fapp(t, n));
      maxerr = fmax(err, maxerr);
    }
    end = clock();

    time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("%d\t%e\t%e\n", n, err, time);
  }

  return EXIT_SUCCESS;
}
