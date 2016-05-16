#include <conio.h>
#include <ga/ga.h>
#define INSTANTIATE_REAL_GENOME
#include <ga/GARealGenome.h>
#include <ga/GAGenome.h>
#include "calibrateheston.h"

static int mycount = 0;

#pragma comment(lib, "fftw3.lib")

#define MAX0(a) (a > 0.0 ? a : 0.0)

//vector<marketParams> * GASolver:: data;
/*GASolver::GASolver(vector <optionParams> _data) {
data = new vector<optionParams>(_data);
}*/

optionParams * GASolver:: data;
size_t GASolver::size;
unsigned int GASolver::N;

using namespace std;


GASolver::GASolver(AlgoParams p, unsigned int _N, optionParams * _data, int _size)
{
	alparam = p;
	N = _N;
	size = _size;
	data = new optionParams[size];
	marketSpread = 0;
	for (int i = 0; i < size; ++i)
	{ 
		data[i] = _data[i];
		marketSpread += mypow((double)(data[i].ask - data[i].bid), 2);
	}
}

GASolver::~GASolver()
{
	delete data;
}

double GASolver::currentPrice(GAGenome& g , optionParams params)
{
	GARealGenome& genome = (GARealGenome&)g;
	assert(genome.gene(1) != 0);
	return callPriceFFT(N, params.S, params.K, params.T, params.r, genome.gene(4), genome.gene(1),
		(genome.gene(0) + genome.gene(2) * genome.gene(2)) / (2 * genome.gene(1)), genome.gene(2), genome.gene(3));
		//genome.gene(0), genome.gene(1), (genome.gene(2) + genome.gene(3) * genome.gene(3))
		/// (2 * genome.gene(1)), genome.gene(3), genome.gene(4));
}

float GASolver::objective(GAGenome& g)
{
	GARealGenome& genome = (GARealGenome&)g;

	float price = 0;
	float err = 0;
	for (size_t i = 0; i < size; ++i)
	{
		price = (float)currentPrice(g, data[i]);
		err += mypow((double)(data[i].price - price),2);
	}

	mycount++;
	return err;
}

GABoolean GASolver::terminateProcess(GAGeneticAlgorithm & ga)
{
	if (ga.statistics().minEver() < marketSpread || ga.generation() >= ga.nGenerations())
		return gaTrue;
	else
		return gaFalse;
}

void GAGeneticAlgorithm::step() {
	//  отправить в интрефейс прогресса, например: generation() / nGenerations();
}

marketParams GASolver::getMarketParams()
{
	marketParams toreturn;
	GARealAlleleSetArray alleles;
	alleles.add(0, 20); // 2*kappa*theta - sigma^2
	alleles.add(0, 1); //theta
	alleles.add(0, 1); //sigma
	alleles.add(-1,1); // rho
	alleles.add(0, 1); // v0

	GARealGenome genome(alleles, objective);

	GAParameterList params;
	GASteadyStateGA::registerDefaultParameters(params);
	
	params.set(gaNnGenerations, alparam.genCount);
	params.set(gaNpopulationSize, alparam.popSize);
	//params.set(gaNscoreFrequency, 10);
	//params.set(gaNflushFrequency, 50);
	params.set(gaNpCrossover, alparam.crosProb);
	params.set(gaNpMutation, alparam.mutProb);
	params.set(gaNminimaxi, -1);
	params.set(gaNselectScores, (int)GAStatistics::AllScores);


	GASteadyStateGA ga(genome);
	ga.parameters(params);
	//ga.terminator((GAGeneticAlgorithm::Terminator)terminateProcess)// ЭТО ГОВНО НЕ ВСТАВЛЯЕТСЯ
	//ga.set(gaNscoreFilename, "bog.log");
	ga.evolve();

	GARealGenome& genomeAux = (GARealGenome&)ga.statistics().bestIndividual();

	assert(genomeAux.gene(1) != 0);	
	toreturn.kappa = (genomeAux.gene(0) + genomeAux.gene(2)*genomeAux.gene(2)) / (2 * genomeAux.gene(1));
	toreturn.v0 = genomeAux.gene(4);
	toreturn.theta = genomeAux.gene(1);
	toreturn.sigma = genomeAux.gene(2);
	toreturn.rho = genomeAux.gene(3);

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