#if !defined(_HESTON_H)
#define _HESTON_H
#include <iostream>
#include <cmath>
#include <complex>
#include <algorithm>
#include <String>
#include<math.h>
#include <assert.h>
#include <vector>
using namespace std;

double trapezoidalMethod(vector<double> x, vector<double> y);

double Heston_integrand(complex<double> K, double X, double V0,
double tau, double thet, double kappa, double SigmaV,
double rho, double gamma);

double HestonPrice(double S, double K, double r, double delta,
double V0, double tau,
double thet, double kappa, double SigmaV, double rho,
double gamma);
/*
double HestonDelta(double S, double K, double r, double delta,
double V0, double tau,
double thet, double kappa, double SigmaV, double rho,
double gamma, string optType);

double HestonGamma(double S, double K, double r, double delta,
double V0, double tau,
double thet, double kappa, double SigmaV, double rho,
double gamma, string optType);

double HestonRho(double S, double K, double r, double delta,
double V0, double tau,
double thet, double kappa, double SigmaV, double rho,
double gamma, string optType);

double HestonVega(double S, double K, double r, double delta,
double V0, double tau,
double thet, double kappa, double SigmaV, double rho,
double gamma, string optType);

double HestonTheta(double S, double K, double r, double delta,
double V0, double tau,
double thet, double kappa, double SigmaV, double rho,
double gamma, string optType);
*/


#endif