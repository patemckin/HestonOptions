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
	//GASolver::GASolver(vector <optionParams> _data);
	~GASolver();
	marketParams getMarketParams();
	//static  vector<optionParams> * data;
private:
	AlgoParams alparam;
	static float objective(GAGenome& g);
	GABoolean done();
	static double currentPrice(GAGenome& g, optionParams params);
	GABoolean GASolver::terminateProcess(GAGeneticAlgorithm & ga);
	static optionParams * data;
	static size_t size;
	static unsigned int N;

	double marketSpread;

};

//marketParams getMarketParams(vector<optionParams> marketData, double crossProb, int popsize);
//float objective(GAGenome& g);