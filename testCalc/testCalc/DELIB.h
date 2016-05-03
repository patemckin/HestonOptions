# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <fstream>
# include <iomanip>
# include <cmath>
# include <ctime>
# include <cstring>
# include <vector>

using namespace std;
// 
//  Change any of these parameters to match your needs 
//
# define POPSIZE 5
# define MAXGENS 5000
# define NVARS 5
# define PXOVER 0.8
# define PMUTATION 0.15

struct optionParams {
	double S; //spot
	double K; //strike
	double T; //time to expiration
	double r; // current interest rate
	double bid; // current bid
	double ask; // current ask
	double price; // current price = (bid-ask)/2
};


//
//  Each GENOTYPE is a member of the population, with
//  gene: a string of variables,
//  fitness: the fitness
//  upper: the variable upper bounds,
//  lower: the variable lower bounds,
//  rfitness: the relative fitness,
//  cfitness: the cumulative fitness.
//
struct genotype
{
	double gene[NVARS];
	double fitness;
	double upper[NVARS];
	double lower[NVARS];
	double rfitness;
	double cfitness;
};

void crossover(int &seed, genotype *population);
void elitist(genotype *population);
void evaluate(genotype *population, vector <optionParams> marketData);
int i4_uniform_ab(int a, int b, int &seed);
void initialize(double* lbounds, double* ubounds, int &seed, genotype *population);
void keep_the_best(genotype *population);
void mutate(int &seed, genotype *population);
double r8_uniform_ab(double a, double b, int &seed);
void report(int generation, genotype *population);
void selector(int &seed, genotype *population, genotype *newpopulation);
void timestamp();
void Xover(int one, int two, int &seed, genotype *population);