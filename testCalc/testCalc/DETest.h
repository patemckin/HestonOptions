#define N_DIM 5
#define N_POP 100
#define MAX_GENERATIONS	10
#include "DESolver.h"

// fractional error less than x.xx * 10 ^ -4.
// Algorithm 26.2.17 in Abromowitz and Stegun, Handbook of Mathematical.
static double erf2(double z);

// cumulative normal distribution
// See Gaussia.java for a better way to compute Phi(z)
static double Phi(double z);

// Polynomial fitting problem
class PolynomialSolver : public DESolver
{
public:
	PolynomialSolver(int dim, int pop) : DESolver(dim, pop), count(0) { ; }
	double EnergyFunction(double trial[], bool &bAtSolution);

private:
	int count;
};
