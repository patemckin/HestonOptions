#include <conio.h>
#include <ga/ga.h>
#define INSTANTIATE_REAL_GENOME
#include <ga/GARealGenome.h>
#include <ga/GAGenome.h>
#include "calibrateheston.h"

#pragma comment(lib, "fftw3.lib")
#define MAX0(a) (a > 0.0 ? a : 0.0)

optionParams*	GASolver::data;
size_t			GASolver::size;
unsigned int	GASolver::N;
double			GASolver::marketSpread;
bool			GASolver::stop;

using namespace std;

GASolver::GASolver(AlgoParams p, unsigned int _N, optionParams * _data, int _size, void *_ptr): ptr(_ptr)
{
	stop = false;
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
	delete[] data;
}

double GASolver::currentPrice(GAGenome& g , optionParams params)
{
	GARealGenome& genome = (GARealGenome&)g;
	assert(genome.gene(1) != 0);
	return callPriceFFT(N, params.S, params.K, params.T, params.r, genome.gene(4), genome.gene(1),
		(genome.gene(0) + genome.gene(2) * genome.gene(2)) / (2 * genome.gene(1)), genome.gene(2), genome.gene(3));
}

float GASolver::objective(GAGenome& g)
{
	GARealGenome& genome = (GARealGenome&)g;

	float price = 0;
	float err = 0;
	for (size_t i = 0; i < size; ++i)
	{
		price = (float)currentPrice(g, data[i]);
		err += mypow((double)(data[i].price - price), 2);
	}

	return err;
}

GABoolean GASolver::terminateProcess(GAGeneticAlgorithm & ga)
{
	if (!stop)
	{
		if (ga.statistics().minEver() < marketSpread || ga.generation() >= ga.nGenerations())
		{
			return gaTrue;
		}
		else
		{
			return gaFalse;
		}
	}
	else
	{
		return gaTrue;
	}
}

void GAGeneticAlgorithm::step()
{

}

void PBClass::step()
{
	int i, mut, c1, c2;
	GAGenome *mom, *dad;          // tmp holders for selected genomes

	GAPopulation *tmppop;		// Swap the old population with the new pop.
	tmppop = oldPop;		// When we finish the ++ we want the newly 
	oldPop = pop;			// generated population to be current (for
	pop = tmppop;			// references to it from member functions).

							// Generate the individuals in the temporary population from individuals in 
							// the main population.

	for (i = 0; i<pop->size() - 1; i += 2) {	// takes care of odd population
		mom = &(oldPop->select());
		dad = &(oldPop->select());
		stats.numsel += 2;		// keep track of number of selections

		c1 = c2 = 0;
		if (GAFlipCoin(pCrossover())) {
			stats.numcro += (*scross)(*mom, *dad,
				&pop->individual(i), &pop->individual(i + 1));
			c1 = c2 = 1;
		}
		else {
			pop->individual(i).copy(*mom);
			pop->individual(i + 1).copy(*dad);
		}
		stats.nummut += (mut = pop->individual(i).mutate(pMutation()));
		if (mut > 0) c1 = 1;
		stats.nummut += (mut = pop->individual(i + 1).mutate(pMutation()));
		if (mut > 0) c2 = 1;

		stats.numeval += c1 + c2;
	}
	if (pop->size() % 2 != 0) {	// do the remaining population member
		mom = &(oldPop->select());
		dad = &(oldPop->select());
		stats.numsel += 2;		// keep track of number of selections

		c1 = 0;
		if (GAFlipCoin(pCrossover())) {
			stats.numcro += (*scross)(*mom, *dad, &pop->individual(i), (GAGenome*)0);
			c1 = 1;
		}
		else {
			if (GARandomBit())
				pop->individual(i).copy(*mom);
			else
				pop->individual(i).copy(*dad);
		}
		stats.nummut += (mut = pop->individual(i).mutate(pMutation()));
		if (mut > 0) c1 = 1;

		stats.numeval += c1;
	}

	stats.numrep += pop->size();
	pop->evaluate(gaTrue);	// get info about current pop for next time

							// If we are supposed to be elitist, carry the best individual from the old
							// population into the current population.  Be sure to check whether we are
							// supposed to minimize or maximize.

	if (minimaxi() == GAGeneticAlgorithm::MAXIMIZE) {
		if (el && oldPop->best().score() > pop->best().score())
			oldPop->replace(pop->replace(&(oldPop->best()), GAPopulation::WORST),
				GAPopulation::BEST);
	}
	else {
		if (el && oldPop->best().score() < pop->best().score())
			oldPop->replace(pop->replace(&(oldPop->best()), GAPopulation::WORST),
				GAPopulation::BEST);
	}

	stats.update(*pop);		// update the statistics by one generation

    p->setValue((int)floor((double)generation() / nGenerations() * 100));
	if (p->wasCanceled())
	{
		GASolver::stop = true;
		delete p;
	}
}

marketParams GASolver::getMarketParams()
{
	marketParams toreturn;
	GARealAlleleSetArray alleles;
	alleles.add(0, 20);				// 2 * kappa * theta - sigma ^ 2
	alleles.add(CALC_ACCURACY, 1);		// theta
	alleles.add(0+CALC_ACCURACY, 1- CALC_ACCURACY);				// sigma
	alleles.add(-1,1);				// rho
	alleles.add(0+ CALC_ACCURACY, 1- CALC_ACCURACY);				// v0

	GARealGenome genome(alleles, objective);

	GAParameterList params;
	GASimpleGA::registerDefaultParameters(params);
	
	params.set(gaNnGenerations, alparam.genCount);
	params.set(gaNpopulationSize, alparam.popSize);
	params.set(gaNpCrossover, alparam.crosProb);
	params.set(gaNpMutation, alparam.mutProb);
	params.set(gaNminimaxi, -1); // Parameter respresents that we are minimize our function


	// we need only minimum value
	params.set(gaNselectScores, (int)GAStatistics::Minimum);

	PBClass ga(genome, ptr);
	ga.parameters(params);
	ga.terminator(terminateProcess);
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