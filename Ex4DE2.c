#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gsl/gsl_sf_trig.h>
#include <Accelerate/Accelerate.h>
#include "DE_trans.h"
#include "sigma.h"

#define ALPHA 1.0
#define DOMD (3.14 / 6.0)

double* AllocVec(int dim)
{
  double* vec;
  vec = (double*)calloc(dim, sizeof(double));

  /* Check memory */
  if (vec == NULL) {
    free(vec);
    fprintf(stderr,"Vector was not allocated! \n");
    exit(1);
  }

  return vec; /* Return the first address of the allocated memory */
}

void FreeVec(double* v)
{
  free(v);
}

double f(double t)
{
  return (1 - t*t) * sqrt(cos(4*atanh(t)) + cosh(M_PI));
}

double G(double x)
{
  double t = M_PI * sinh(x);
  double val = cos(2*t) + cosh(M_PI);

  return -2*(sin(2*t) + val * tanh(0.5*t))*DE_trans_div(-1, 1, x) / sqrt(val);
}

double Fapp(double* p, double t, int m)
{
  double h = log(2*DOMD*m / ALPHA) / m;

  int j;

  double sum = 0;

  for (j = -m; j <= m; j++) {
    sum += p[j+m] * gsl_sf_sinc((DE_trans_inv(-1, 1, t) - j*h) / h);
  }

  return sum;
}

double eta(double x)
{
  return 0.5*(1 + x);
}

int main()
{
  int i, j;
  int n, N;
  double t, h;
  double err, maxerr;
  double Iast;
  clock_t start, end;
  double time;
  double *J, *g, *c;

  int STEP = 1000;

  for (n = 3; n <= 150; n += 6) {
    start = clock();

    N = 2*n+1;
    J = AllocVec(N*N);
    g = AllocVec(N);
    c = AllocVec(N);

    h = log(2*DOMD*n / ALPHA) / n;

    Iast = 0;
    for (i = -n; i <= n; i++) {
      Iast += G(i*h);
    }
    Iast *= h;

    for (i = -n; i <= n; i++) {
      for (j = -n; j <= n; j++) {
        J[(i+n)*N + (j+n)] = 0.5 + (double)sigma_orig[1000 + (i - j)];
      }
      g[(i+n)] = G(i*h) - 0.5 * Iast * DE_trans_div(-1, 1, i*h);
    }

    /* c = h J g */
    cblas_dgemv(CblasRowMajor, CblasNoTrans, N, N, h, J, N, g, 1, 0.0, c, 1);

    maxerr = 0;
    for (i = - STEP + 1; i <= STEP - 1; i++) {
      t = (double)i / (double)(STEP);

      err = fabs(f(t) - Fapp(c, t, n) - Iast*eta(t));
      maxerr = fmax(err, maxerr);
    }
    end = clock();

    time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("%d\t%e\t%e\n", n, err, time);

    FreeVec(J);
    FreeVec(g);
    FreeVec(c);
  }

  return EXIT_SUCCESS;
}
