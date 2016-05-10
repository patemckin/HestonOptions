#pragma once
#include <iostream>
#include <math.h>
#include <ga/ga.h>
#define INSTANTIATE_REAL_GENOME
#include <ga/GARealGenome.h>
#include <ga/GAGenome.h>
#include "PriceCalculation.h"
#include <vector>
#include "structs.h"

marketParams getMarketParams(vector<optionParams> marketData, double crossProb, int popsize);
float objective(GAGenome& g);