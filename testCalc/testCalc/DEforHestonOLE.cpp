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




double DEforHestonOLE::EnergyFunction(double *trial, bool &bAtSolution)
{

	// check limits, if search is going out, break it
	for (int i = 0; i < N_DIM; ++i)
	{
		if (trial[i] > max[i] || trial[i] < min[i])
			return 1e21;
	}


	// Caclulate OLE error
	double err = 0;
	double price;
	//cout << "Prices:\t";
	for (int i = 0; i < marketData.size(); ++i) {
		price = callPriceFFT(12, marketData[i].S, marketData[i].K, marketData[i].T, marketData[i].r, trial[0], trial[1], trial[2], trial[3], trial[4]);
	//	cout << price << " | ";
		err += (marketData[i].price - price)*(marketData[i].price - price);
	}

	cout << endl << "Current OLE error:  " << sqrt(err) << endl;

	// check if it is nice
	if (sqrt(err) / marketData.size()/meanPrice < 0.01)
		bAtSolution = true;

	return err;
}
