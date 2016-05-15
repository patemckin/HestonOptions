#include "calibrateheston.h"
#include <conio.h>

#pragma comment(lib, "fftw3.lib")
#pragma comment(lib, "ga.lib")

#define MAX0(a) (a > 0.0 ? a : 0.0)

using namespace std;

int mcount = 0;

  optionParams *GASolver:: data;
 int GASolver::size;

//vector<optionParams> data;
/*
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

/*GASolver::GASolver(vector<optionParams> _data)
{
	data = _data;
}*/
GASolver::GASolver(optionParams * _data, int _size) {
	size = _size;
	data = new optionParams[size];
	for (int i = 0; i < size; ++i)
		data[i] = _data[i];
}

GASolver::GASolver(){}

GASolver::~GASolver()
{
	delete data;
}

/*void GASolver::setData(vector<optionParams> _data) : data(_data)
{
}*/

float GASolver::objective(GAGenome& g)
{
	GARealGenome& genome = (GARealGenome&)g;

	float price = 0;
	float err = 0;
	for (size_t i = 0; i < size; ++i)
	{
	/*	cout << "-----" << endl;
		cout << genome.gene(4) << endl;
		cout << genome.gene(1) << endl;
		cout << (genome.gene(0) + genome.gene(2)*genome.gene(2)) / (2 * genome.gene(1)) << endl;
		cout << genome.gene(2) << endl;
		cout << genome.gene(3) << endl;
		cout << "-----" << endl;*/
		price = (float)callPriceFFT(12, data[i].S, data[i].K, data[i].T, data[i].r, genome.gene(4), genome.gene(1),
			(genome.gene(0) + genome.gene(2)*genome.gene(2)) / (2 * genome.gene(1)), genome.gene(2), genome.gene(3));
		
		//cout << price;
		err += (data[i].price - price)*(data[i].price - price);
	}
	//cout << err << endl;
	mcount++;
	
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
	alleles.add(0, 20); // 2*kappa*theta - sigma^2
	alleles.add(0.0001, 1); //theta
	alleles.add(0, 1); //sigma
	alleles.add(-1, 1); // rho
	alleles.add(0, 1); // v0

	GARealGenome genome(alleles, objective);

	GAParameterList params;
	GASteadyStateGA::registerDefaultParameters(params);
	params.set(gaNnGenerations, 20);
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

	toreturn.kappa = (genomeAux.gene(0) + genomeAux.gene(2)*genomeAux.gene(2)) / (2 * genomeAux.gene(1));
	toreturn.v0 = genomeAux.gene(4);
	toreturn.theta = genomeAux.gene(1);
	toreturn.sigma = genomeAux.gene(2);
	toreturn.rho = genomeAux.gene(3);

	cout << toreturn.v0 << endl;
	cout << toreturn.theta << endl;
	cout << toreturn.sigma<< endl;
	cout << toreturn.rho << endl;
	cout << toreturn.kappa << endl;

	return toreturn;
}


int main()
{
	optionParams marketdata[1];
	marketdata[0].r = 2.2685; marketdata[0].T = 0.126027;  marketdata[0].S = 1544.50;  marketdata[0].K = 1000.00; marketdata[0].price = 559.00; marketdata[0].bid = 553.00; marketdata[0].ask = 565.00;


	GASolver solver(marketdata,1);
	solver.getMarketParams(0.9, 10);

	cout << mcount << endl;

	_getch();
	return 0;
}
