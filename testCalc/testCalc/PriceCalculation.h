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

double callPriceFFT(int N, double S, double K, double T, double r, double v0, double theta, double kappa, double sigma, double rho);

#endif // PRICECALCULATION_H_INCLUDED
