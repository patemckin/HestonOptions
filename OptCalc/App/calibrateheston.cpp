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

using namespace std;

GASolver::GASolver(AlgoParams p, unsigned int _N, optionParams * _data, int _size, void *_ptr): ptr(_ptr)
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
	if (ga.statistics().minEver() < marketSpread || ga.generation() >= ga.nGenerations())
	{
		return gaTrue;
	}	
	else
	{
		return gaFalse;
	}	
}

void GAGeneticAlgorithm::step()
{

}

void PBClass::step()
{
	int i, mut, c1, c2;
	GAGenome *mom, *dad;          // tmp holders for selected genomes

								  // Generate the individuals in the temporary population from individuals in 
								  // the main population.

	for (i = 0; i<tmpPop->size() - 1; i += 2) {	// takes care of odd population
		mom = &(pop->select());
		dad = &(pop->select());
		stats.numsel += 2;		// keep track of number of selections

		c1 = c2 = 0;
		if (GAFlipCoin(pCrossover())) {
			stats.numcro += (*scross)(*mom, *dad, &tmpPop->individual(i),
				&tmpPop->individual(i + 1));
			c1 = c2 = 1;
		}
		else {
			tmpPop->individual(i).copy(*mom);
			tmpPop->individual(i + 1).copy(*dad);
		}
		stats.nummut += (mut = tmpPop->individual(i).mutate(pMutation()));
		if (mut > 0) c1 = 1;
		stats.nummut += (mut = tmpPop->individual(i + 1).mutate(pMutation()));
		if (mut > 0) c2 = 1;

		stats.numeval += c1 + c2;
	}
	if (tmpPop->size() % 2 != 0) {	// do the remaining population member
		mom = &(pop->select());
		dad = &(pop->select());
		stats.numsel += 2;		// keep track of number of selections

		c1 = 0;
		if (GAFlipCoin(pCrossover())) {
			stats.numcro += (*scross)(*mom, *dad,
				&tmpPop->individual(i), (GAGenome*)0);
			c1 = 1;
		}
		else {
			if (GARandomBit())
				tmpPop->individual(i).copy(*mom);
			else
				tmpPop->individual(i).copy(*dad);
		}
		stats.nummut += (mut = tmpPop->individual(i).mutate(pMutation()));
		if (mut > 0) c1 = 1;

		stats.numeval += c1;
	}

	// Replace the worst genomes in the main population with all of the individuals
	// we just created.  Notice that we invoke the population's add member with a
	// genome pointer rather than reference.  This way we don't force a clone of
	// the genome - we just let the population take over.  Then we take it back by
	// doing a remove then a replace in the tmp population.

	for (i = 0; i<tmpPop->size(); i++)
		pop->add(&tmpPop->individual(i));
	pop->evaluate();		// get info about current pop for next time
	pop->scale();			// remind the population to do its scaling

							// the individuals in tmpPop are all owned by pop, but tmpPop does not know 
							// that.  so we use replace to take the individuals from the pop and stick 
							// them back into tmpPop
	for (i = 0; i<tmpPop->size(); i++)
		tmpPop->replace(pop->remove(GAPopulation::WORST, GAPopulation::SCALED), i);

	stats.numrep += tmpPop->size();

	stats.update(*pop);		// update the statistics by one generation

    p->setValue((int)floor((double)generation() / nGenerations() * 100));
}

marketParams GASolver::getMarketParams()
{
	marketParams toreturn;
	GARealAlleleSetArray alleles;
	alleles.add(0, 20);				// 2 * kappa * theta - sigma ^ 2
	alleles.add(0.00001, 1);		// theta
	alleles.add(0, 1);				// sigma
	alleles.add(-1,1);				// rho
	alleles.add(0, 1);				// v0

	GARealGenome genome(alleles, objective);

	GAParameterList params;
	GASteadyStateGA::registerDefaultParameters(params);
	
	params.set(gaNnGenerations, alparam.genCount);
	params.set(gaNpopulationSize, alparam.popSize);
	params.set(gaNpCrossover, alparam.crosProb);
	params.set(gaNpMutation, alparam.mutProb);
	params.set(gaNminimaxi, -1);
	params.set(gaNselectScores, (int)GAStatistics::AllScores);

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