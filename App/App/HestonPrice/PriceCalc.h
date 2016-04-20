#ifndef SOLVER_H_INCLUDED
#define SOLVER_H_INCLUDED

#include <iostream>
#include <complex>
#include <cmath>
#include <vector>

#define _USE_MATH_DEFINES

using namespace std;

typedef complex<double> cmplx;
typedef vector<cmplx>   cmplxvec;

#define I cmplx(0, 1)
#define M_PI atan(1.0)*4
typedef struct _params
{
    /**
    kappa = rate of reversion
    theta = long run variance
    sigma = Volatility of volatility
    v0 = initial Variance
    rho = correlation
    T = Time till maturity
    r = interest rate
    s0 = initial asset price
    */

    double kappa, theta, sigma, v0, rho, T, r, s0;
} params;

class Solver
{
public:
    Solver(params *parameters);
    ~Solver();
    double hestonCallFft(double strike);
    double hestonCallFftTimeValue(double strike);

private:
    params *p;
};


#endif // SOLVER_H_INCLUDED
