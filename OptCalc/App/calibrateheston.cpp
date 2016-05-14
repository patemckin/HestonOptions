#include <conio.h>
#include <ga/ga.h>
#define INSTANTIATE_REAL_GENOME
#include <ga/GARealGenome.h>
#include <ga/GAGenome.h>
#include "calibrateheston.h"



#pragma comment(lib, "fftw3.lib")
//#pragma comment(lib, "ga.lib")

#define MAX0(a) (a > 0.0 ? a : 0.0)

//vector<marketParams> * GASolver:: data;

optionParams * GASolver:: data;
size_t GASolver::size;

using namespace std;
/*
vector<optionParams> data;

marketParams getMarketParams(vector<optionParams> marketData, double crossProb, int popSize)
{
	::data.clear();
	::data = marketData;
	marketParams toreturn;
	GARealAlleleSetArray alleles;
	alleles.add(0, 1);
	alleles.add(0, 50);
	alleles.add(0, 50);
	alleles.add(0, 1);
	alleles.add(-1, 1);

	GARealGenome genome(alleles, objective);

	GAParameterList params;
	GASteadyStateGA::registerDefaultParameters(params
	);
	params.set(gaNnGenerations, 10);
	params.set(gaNpopulationSize, popSize);
	params.set(gaNscoreFrequency, 10);
	params.set(gaNflushFrequency, 50);
	params.set(gaNpCrossover, crossProb);
	params.set(gaNpMutation, 0.1);
	params.set(gaNminimaxi, -1);
	params.set(gaNselectScores, (int)GAStatistics::AllScores);

	GASteadyStateGA ga(genome);
	ga.parameters(params);
	ga.set(gaNscoreFilename, "bog.log");
	ga.evolve();

	GARealGenome& genomeAux = (GARealGenome&)ga.statistics().bestIndividual();

	cout << genomeAux.gene(0) << endl;
	cout << genomeAux.gene(1) << endl;
	cout << (genomeAux.gene(2) + genomeAux.gene(3) * genomeAux.gene(3)) / (2 * genomeAux.gene(1)) << endl;
	cout << genomeAux.gene(3) <<  endl;
	cout << genomeAux.gene(4) << endl;

	toreturn.v0 = genomeAux.gene(0);
	toreturn.theta = genomeAux.gene(1);
	toreturn.kappa = genomeAux.gene(2);
	toreturn.sigma = genomeAux.gene(3);
	toreturn.rho = genomeAux.gene(4);

	return toreturn;
}

float objective(GAGenome& g)
{
	GARealGenome& genome = (GARealGenome&)g;
	
	float price = 0;
	float err = 0;
	for (int i = 0; i < ::data.size(); ++i) 
	{
		price = callPriceFFT(12, ::data[i].S, ::data[i].K, ::data[i].T, ::data[i].r, genome.gene(0), genome.gene(1), (genome.gene(2) + genome.gene(3) * genome.gene(3))
			/ (2 * genome.gene(1)), genome.gene(3), genome.gene(4));
		err += (::data[i].price - price)*(::data[i].price - price);
	}

	return err;
}
*/

GASolver::GASolver(optionParams * _data, int _size) {
	size = _size;
	data = new optionParams[size];
	for (int i = 0; i < size; ++i)
		data[i] = data[i];
}

/*GASolver::GASolver(vector <optionParams> _data) {
	data = new vector<optionParams>(_data);
}*/


GASolver::GASolver(){}

GASolver::~GASolver()
{
	delete data;
}

float GASolver::objective(GAGenome& g)
{
	GARealGenome& genome = (GARealGenome&)g;

	float price = 0;
	float err = 0;
	for (size_t i = 0; i < size; ++i)
	{
		price = (float)callPriceFFT(12, data[i].S, data[i].K, data[i].T, data[i].r, genome.gene(0), genome.gene(1), (genome.gene(2) + genome.gene(3) * genome.gene(3))
			/ (2 * genome.gene(1)), genome.gene(3), genome.gene(4));
		err += pow(data[i].price - price, 2);
	}

	return err;
}

marketParams GASolver::getMarketParams(double crossProb, int popSize)
{
	//GASolver *GSI = new GASolver();
	//GSI->setData(marketData);
	
	//data.clear();
	//data = marketData;
	marketParams toreturn;
	GARealAlleleSetArray alleles;
	alleles.add(0, 1);
	alleles.add(0, 50);
	alleles.add(0, 50);
	alleles.add(0, 1);
	alleles.add(-1, 1);

	GARealGenome genome(alleles, objective);

	GAParameterList params;
	GASteadyStateGA::registerDefaultParameters(params);
	params.set(gaNnGenerations, 10);
	params.set(gaNpopulationSize, popSize);
	params.set(gaNscoreFrequency, 10);
	params.set(gaNflushFrequency, 50);
	params.set(gaNpCrossover, crossProb);
	params.set(gaNpMutation, 0.1);
	params.set(gaNminimaxi, -1);
	params.set(gaNselectScores, (int)GAStatistics::AllScores);

	GASteadyStateGA ga(genome);
	ga.parameters(params);
	ga.set(gaNscoreFilename, "bog.log");
	ga.evolve();

	GARealGenome& genomeAux = (GARealGenome&)ga.statistics().bestIndividual();

	cout << genomeAux.gene(0) << endl;
	cout << genomeAux.gene(1) << endl;
	cout << (genomeAux.gene(2) + genomeAux.gene(3) * genomeAux.gene(3)) / (2 * genomeAux.gene(1)) << endl;
	cout << genomeAux.gene(3) << endl;
	cout << genomeAux.gene(4) << endl;

	toreturn.v0 = genomeAux.gene(0);
	toreturn.theta = genomeAux.gene(1);
	toreturn.kappa = genomeAux.gene(2);
	toreturn.sigma = genomeAux.gene(3);
	toreturn.rho = genomeAux.gene(4);

	return toreturn;
}

/*
int main()
{
	optionParams marketdata[1];
	marketdata[0].r = 2.2685; marketdata[0].T = 0.126027;  marketdata[0].S = 1544.50;  marketdata[0].K = 1000.00; marketdata[0].price = 559.00; marketdata[0].bid = 553.00; marketdata[0].ask = 565.00;


	GASolver solver(marketdata,1);
	solver.getMarketParams(0.9, 100);

	_getch();
	return 0;
}
*/