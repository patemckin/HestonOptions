// Differential Evolution Test Program
// Based on algorithms developed by Dr. Rainer Storn & Kenneth Price
// Written By: Lester E. Godwin
//             PushCorp, Inc.
//             Dallas, Texas
//             972-840-0208 x102
//             godwin@pushcorp.com
// Created: 6/8/98
// Last Modified: 6/8/98
// Revision: 1.0

#include <stdio.h>
#include <math.h>
#include <String>
#include "DESolver.h"
#include "PriceCalculation.h"
#include "DETest.h"

double erf(double z) {
        double t = 1.0 / (1.0 + 0.5 * fabs(z));

        // use Horner's method
        double ans = 1 - t * exp( -z*z   -   1.26551223 +
                                            t * ( 1.00002368 +
                                            t * ( 0.37409196 + 
                                            t * ( 0.09678418 + 
                                            t * (-0.18628806 + 
                                            t * ( 0.27886807 + 
                                            t * (-1.13520398 + 
                                            t * ( 1.48851587 + 
                                            t * (-0.82215223 + 
                                            t * ( 0.17087277))))))))));
        if (z >= 0) return  ans;
        else        return -ans;
    }

    // fractional error less than x.xx * 10 ^ -4.
    // Algorithm 26.2.17 in Abromowitz and Stegun, Handbook of Mathematical.
    static double erf2(double z) {
        double t = 1.0 / (1.0 + 0.47047 * abs(z));
        double poly = t * (0.3480242 + t * (-0.0958798 + t * (0.7478556)));
        double ans = 1.0 - poly * exp(-z*z);
        if (z >= 0) return  ans;
        else        return -ans;
    }

    // cumulative normal distribution
    // See Gaussia.java for a better way to compute Phi(z)
    static double Phi(double z) {
        return 0.5 * (1.0 + erf(z / (sqrt(2.0))));
    }

double PolynomialSolver::EnergyFunction(double *trial,bool &bAtSolution)
{
    //return erf(4);
/*    double S = 100; //spot price
double K = 85; //Strike price
double r = 0.00; //Risk-free interest rate
double delta = 0; //Dividend yield
double V0;// = 0.01; //Initial volatility//trial[0]
double tau = 0.00001; //Time to maturity (years)
double thet;// = 0.01; //Long-run volatility//trial[1]
double kappa;// = 2; //Mean-reversion rate for volatility//trial[2]
double SigmaV;// = 0.225; //Volatility of volatility//trial[3]
double rho;// = 0;//; //Price-volatility correlation//trial[4]
double gamma = 0; //Risk-aversion parameter
*/
	double S = 100; //spot price
	double K = 85; //Strike price
	double r = 0.00; //Risk-free interest rate
	double delta = 0; //Dividend yield
	double tau = 1; //Time to maturity (years)

params *p = new params;
p->kappa = trial[2];
p->theta = trial[1];
p->sigma = trial[3];
p->v0 = trial[0];
p->rho = trial[4];
p->r = r;
p->K = K;
p->T = tau;
p->S = S;
//string optType = "call"; //Option type (call or put)


double price = callPriceFFT(14, p);


double err = price - 15.270668799937340;

if (price != price)
{
	perror("\n Warning:!!!");
	cout << endl;
}


std::cout <<"Price:" << price <<"Error:" << err << endl;

	
return err*err;


    
}


/*void main(void)
{
	double min[N_DIM];
	double max[N_DIM];
	int i;
    //double c = erf(0.5);
	//printf("%lf", c);
       double S = 100; //spot price
double K = 85; //Strike price
double r = 0.00; //Risk-free interest rate
double delta = 0; //Dividend yield
double V0 = 0.01; //Initial volatility//trial[0]
double tau = 5; //Time to maturity (years)
double thet= 0.01; //Long-run volatility//trial[1]
double kappa = 2; //Mean-reversion rate for volatility//trial[2]
double SigmaV = 0.225; //Volatility of volatility//trial[3]
double rho = 0;//; //Price-volatility correlation//trial[4]
double gamma = 0; //Risk-aversion parameter
/*	PolynomialSolver solver(N_DIM,N_POP);

	for (i=0;i<N_DIM;i++)
	{
		max[i] =  1.0;
		min[i] = 0.0;
	}

	solver.Setup(min,max,stBest1Exp,0.9,1.0);
	
	printf("Calculating...\n\n");
	solver.Solve(MAX_GENERATIONS);

	double *solution = solver.Solution();
    
	for (i=0;i<N_DIM;i++)
		printf("[%d]: %lf\n",i,solution[i]);

    printf("%lf\n",(HestonPrice(S,K,r,delta,V0,tau,thet,kappa,SigmaV,rho,gamma)));

	return;
}
*/