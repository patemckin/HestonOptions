#include <stdio.h>
#include <math.h>
#include <String>
#include "DESolver.h"
#include "PriceCalculation.h"
#include "DEforHestonOLS.h"

DEforHestonOLE::DEforHestonOLE(double _min[], double _max[], vector <optionParams> data) : DESolver(N_DIM, N_POP), count(0), marketData(data) {
	for (int i = 0; i < N_DIM; ++i)
	{
		min[i] = _min[i];
		max[i] = _max[i];
		this->Setup(min, max, stBest1Exp, 0.9, 1.0);
	}

	absErr = 0;
	for each (optionParams option in data)
	{
		absErr += fabs((option.bid - option.ask)); // for OLS with weights
	}
	cout << absErr<<endl;
}




double DEforHestonOLE::EnergyFunction(double *trial, bool &bAtSolution)
{

	// check limits, if search is going out, break it
	for (int i = 0; i < N_DIM; ++i)
	{
		if (trial[i] > max[i] || trial[i] < min[i])
			return 1e21;
		
	}
	if (2 * trial[1] * trial[2] <= trial[3]) // Fellers condition
		return 1e21;

	// Caclulate OLS error
	double err = 0;
	double price;


	// OLS with weights 
	for (int i = 0; i < marketData.size(); ++i) {
		price = callPriceFFT(12, marketData[i].S, marketData[i].K, marketData[i].T, marketData[i].r, trial[0], trial[1], trial[2], trial[3], trial[4]);
		err += pow((marketData[i].price - price),(unsigned int)2) / fabs(marketData[i].ask - marketData[i].bid); 
	}

	cout << endl << "Current OLE error:  " << err << endl;

	// check if it is nice
	if (err < absErr)
		bAtSolution = true;

	return err;
}
