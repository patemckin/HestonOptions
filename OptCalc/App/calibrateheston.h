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
	GASolver(optionParams * _data, int _size);
	//GASolver::GASolver(vector <optionParams> _data);
	~GASolver();
	marketParams getMarketParams(double crossProb, int popSize);
	//static  vector<optionParams> * data;
private:
	static float objective(GAGenome& g);
	GABoolean done();
	static double currentPrice(GAGenome& g, optionParams params);
	GABoolean GASolver::terminateProcess(GAGeneticAlgorithm & ga);
	static optionParams * data;
	static size_t size;

	double marketSpread;

};

//marketParams getMarketParams(vector<optionParams> marketData, double crossProb, int popsize);
//float objective(GAGenome& g);