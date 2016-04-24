#ifndef PRICECALCULATION_H_INCLUDED
#define PRICECALCULATION_H_INCLUDED

#define _USE_MATH_DEFINES
#include <iostream>
#include <complex>
#include <cmath>
#include <vector>
#include "4FFT\include\fftw3.h"



using namespace std;

typedef complex<double> cmplx;

#define I cmplx(0, 1)

/*typedef struct _params
{
    /**
    S       = Spot prices
    K       = Strike price
    T       = Maturities
    r       = Discount factors
        % Specify the model
  //  v0      = Instantanuous variance of base parameter set
 //   theta   = Long term variance of base parameter set //theta
//    kappa   = Mean reversion speed of variance of base parameter set
//   sigma   = Volatility of variance of base parameter set
//    rho     = Correlation of base parameter set

    double S, K, T, r, v0, theta, kappa, sigma, rho;
} 
params;*/

double callPriceFFT(int N, double S,double K,double T,double r,double v0,double theta,double kappa,double sigma,double rho);

#endif // PRICECALCULATION_H_INCLUDED
