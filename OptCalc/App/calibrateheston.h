#pragma once
#include <iostream>
#include <math.h>
#include <ga/ga.h>
#include <ga/GARealGenome.h>
#include <ga/GAGenome.h>
#include "PriceCalculation.h"
#include <vector>
#include "structs.h"
/*
struct optionParams {
	float S; //spot
	float K; //strike
	float T; //time to expiration
	float r; // current interest rate
	float bid; // current bid
	float ask; // current ask
	float price; // current price = (bid+ask)/2
};

struct marketParams {
	float v0;
	float theta;
	float kappa;
	float sigma;
	float rho;
};
*/
class GASolver: GAGenome
{
public:	
	GASolver(optionParams * _data, int _size);
	GASolver();
	~GASolver();
	static float objective(GAGenome& g);
	marketParams getMarketParams(double crossProb, int popSize);
	 static  optionParams * data;
	 static int size;
};

marketParams getMarketParams(vector<optionParams> marketData, double crossProb, int popsize);
float objective(GAGenome& g);