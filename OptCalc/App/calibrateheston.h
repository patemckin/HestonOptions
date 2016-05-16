#pragma once
#include <iostream>
#include <math.h>
#include <ga/ga.h>
#include <ga/GARealGenome.h>
#include <ga/GAGenome.h>
#include "PriceCalculation.h"
#include <vector>
#include "structs.h"

class GASolver: GAGenome 
{
public:	
	GASolver(AlgoParams p, unsigned int _N, optionParams * _data, int _size);
	~GASolver();
	marketParams getMarketParams();

private:
	AlgoParams alparam;
	static unsigned int N;
	static optionParams * data;
	static size_t size;

	static double currentPrice(GAGenome& g, optionParams params);
	static float objective(GAGenome& g);
	GABoolean done();
	GABoolean GASolver::terminateProcess(GAGeneticAlgorithm & ga);

	double marketSpread;
};