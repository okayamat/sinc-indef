#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gsl/gsl_sf_trig.h>
#include <Accelerate/Accelerate.h>
#include "SE_trans.h"
#include "sigma.h"

#define ALPHA 0.99
#define DOMD 3.14

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
  return 0.25 * ((1+t) * log1p(t) + (1-t) * log1p(-t) - 2 * M_LN2) / M_LN2;
}

double G(double x)
{
  return 0.25 * x * SE_trans_div(-1, 1, x) / M_LN2;
}

double Fapp(double* p, double t, int m)
{
  double h = sqrt(M_PI*DOMD / (ALPHA*m));

  int j;

  double sum = 0;

  for (j = -m; j <= m; j++) {
    sum += p[j+m] * gsl_sf_sinc((SE_trans_inv(-1, 1, t) - j*h) / h);
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

  for (n = 3; n <= 180; n += 6) {
    start = clock();

    N = 2*n+1;
    J = AllocVec(N*N);
    g = AllocVec(N);
    c = AllocVec(N);

    h = sqrt(M_PI*DOMD / (ALPHA*n));

    Iast = 0;
    for (i = -n; i <= n; i++) {
      Iast += G(i*h);
    }
    Iast *= h;

    for (i = -n; i <= n; i++) {
      for (j = -n; j <= n; j++) {
        J[(i+n)*N + (j+n)] = 0.5 + (double)sigma_orig[1000 + (i - j)];
      }
      g[(i+n)] = G(i*h) - 0.5 * Iast * SE_trans_div(-1, 1, i*h);
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
