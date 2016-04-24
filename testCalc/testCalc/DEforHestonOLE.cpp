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
#include "DEforHestonOLE.h"

DEforHestonOLE::DEforHestonOLE(double _min[], double _max[], vector <optionParams> data) : DESolver(N_DIM, N_POP), count(0), marketData(data) {
	for (int i = 0; i < N_DIM; ++i)
	{
		min[i] = _min[i];
		max[i] = _max[i];
		this->Setup(min, max, stBest1Exp, 0.9, 1.0);
	}

	double sumPrice = 0;
	for each (optionParams option in data)
	{
		sumPrice += option.price;
	}
	meanPrice = sumPrice / data.size();
}




double DEforHestonOLE::EnergyFunction(double *trial,bool &bAtSolution)
{


/*p->kappa = trial[2];
p->theta = trial[1];
p->sigma = trial[3];
p->v0 = trial[0];
p->rho = trial[4];
p->r = r;
p->K = K;
p->T = tau;
p->S = S;*/
for (int i = 0; i < N_DIM; ++i)
{
	if (trial[i] > max[i] || trial[i] < min[i])
		return 1e20;
}


// Caclulate OLE error
double err = 0;
double price;


cout << "Prices:\t";
for (int i = 0; i < marketData.size();++i){ 
	price = callPriceFFT(14, marketData[i].S, marketData[i].K, marketData[i].T, marketData[i].r, trial[0], trial[1], trial[2], trial[3], trial[4]);
	cout << price << " | ";
	err += (marketData[i].price - price)*(marketData[i].price - price);
}

cout <<endl<< "--Error-- :  " << sqrt(err)<<endl;

if (sqrt(err)/marketData.size()/meanPrice < 0.10)
	bAtSolution = true;

return err*err;

    
}
