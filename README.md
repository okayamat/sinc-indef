# sinc-indef
Sinc indefinite integration formulas for a finite interval
## Overview
These programs approximate the indefinite integration of the following
four functions:

(1) f(s) = 1 / (pi * sqrt(1 - s^2))
(2) f(s) = log((1 + s)/(1 - s)) / (4 log 2)
(3) f(s) = 2 / (pi * (1 + s^2))
(4) f(s) = -2 (s g(s) + sin(4 artanh s)) / sqrt(g(s)),
    g(s) = cos(4 artanh s) + cosh(pi)

For the approximation, the following six formulas are considered:

(SE1) proposed by Haber [1]
(SE2) proposed by Stenger [2]
(SE3) proposed by Stenger [2]
(DE1) proposed by Muhammad and Mori [3]
(DE2) proposed by Tanaka et al. [4]
(DE3) proposed by Okayama and Tanaka [5]

The name of the program denotes the example number and the approximation
formula. For example, the program Ex2DE1.c approximates the indefinite
integraion of the function in (1) using the formula (DE1). GNU Scientific
Library (GSL) is needed because the formulas (SE1) and (DE1) need to
compute a special function Si(x). For the formulas (SE2), (SE3), (DE2)
and (DE3), CBLAS is used for computation of matrix-vector multiplication.

Each program investigates maximum approximation error among 1000 equally
spaced points over the interval (-1, 1), increasing a truncation number n
as n = 3, 9, 15, 21, 27, ..., then outputs n, maximum error, and its
computation time.

## Results
Outputs by those programs are stored in data/ directory, with .dat extension.
Gnuplot programs for creating graphs are also stored in the directory.

computation environment:

Mac OS X 10.15.7
CPU: 2.4 GHz quad core (Intel Core i5)
Memory: 16 GB 2133 MHz LPDDR3
Compiler: Apple clang version 12.0.0

## References
1. S. Haber:
 Two formulas for numerical indefinite integration, Mathematics of
 Computation, Vol. 60 (1993), pp. 279--296.
2. F. Stenger:
 Numerical Methods Based on Sinc and Analytic Functions, Springer-Verlag,
 New York, 1993.
3. M. Muhammad and M. Mori:
 Double exponential formulas for numerical indefinite integration,
 Journal of Computational and Applied Mathematics, Vol. 161 (2003),
 pp. 431--448.
4. K. Tanaka, M. Sugihara, K. Murota:
 Numerical indefinite integration by double exponential sinc method,
 Mathematics of Computation, Vol. 74 (2005), pp. 655--679.
5. T. Okayama and K. Tanaka:
 Yet another DE-Sinc indefinite integration formula, submitted.
