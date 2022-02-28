#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gsl/gsl_sf_trig.h>
#include <Accelerate/Accelerate.h>
#include "SE_trans.h"
#include "sigma.h"

#define ALPHA 1.0
#define DOMD 1.57

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
  double val = cos(2*x) + cosh(M_PI);

  return -2*(sin(2*x) + val * tanh(0.5*x))*SE_trans_div(-1, 1, x) / sqrt(val);
}

double omega_j(double t, int j, double h)
{
  return gsl_sf_sinc((SE_trans_inv(-1, 1, t) - j*h)/h);
}
/*
double omega_mN(double t, int m, double h)
{
  double val = 0;
  int k;

  for (k = m; k > -m; k--) {
    val += omega_j(t, k, h) / (1 + exp(k*h));
  }

  val = 0.5*(1 - t) - val;

  return val * (1 + exp(-m*h));
}

double omega_pN(double t, int m, double h)
{
  double val = 0;
  int k;

  for (k = -m; k < m; k++) {
    val += omega_j(t, k, h) / (1 + exp(-k*h));
  }

  val = 0.5*(1 + t) - val;

  return val * (1 + exp(-m*h));
}
*/
double Fapp(double* p, double t, int m)
{
  double h = sqrt(M_PI*DOMD / (ALPHA*m));

  double val = (1 + exp(m*h));
  double omega_m = omega_j(t, m, h) / val;
  double omega_p = omega_j(t,-m, h) / val;

  int j;

  double sum = 0;
  for (j = -m+1; j <= m-1; j++) {
    val = omega_j(t, j, h);
    sum += p[j+m] * val;
    omega_m += val / (1 + exp( j*h));
    omega_p += val / (1 + exp(-j*h));
  }

  val = (1 + exp(-m*h));
  omega_m = (0.5*(1 - t) - omega_m) * val;
  omega_p = (0.5*(1 + t) - omega_p) * val;

  sum += p[-m+m] * omega_m + p[m+m] * omega_p;

  return sum;
}

int main()
{
  int i, j;
  int n, N;
  double t, h;
  double err, maxerr;
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

    h = sqrt(M_PI*DOMD / (ALPHA*n));

    for (i = -n; i <= n; i++) {
      for (j = -n; j <= n; j++) {
        J[(i+n)*N + (j+n)] = 0.5 + (double)sigma_orig[1000 + (i - j)];
      }
      g[(i+n)] = G(i*h);
    }

    /* c = h J g */
    cblas_dgemv(CblasRowMajor, CblasNoTrans, N, N, h, J, N, g, 1, 0.0, c, 1);

    maxerr = 0;
    for (i = - STEP + 1; i <= STEP - 1; i++) {
      t = (double)i / (double)(STEP);

      err = fabs(f(t) - Fapp(c, t, n));
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
