#define N_DIM 5
#define N_POP 100
#define MAX_GENERATIONS	100
#include "DESolver.h"
#include <vector>

struct optionParams{
	double S; //spot
	double K; //strike
	double T; //time to expiration
	double r; // current interest rate
	double price; // current price
};


class DEforHestonOLE : public DESolver
{
public:
	DEforHestonOLE( double _min[], double _max[], vector <optionParams> data);
	double EnergyFunction(double trial[], bool &bAtSolution);

private:
	int count;
	double min[N_DIM];
	double max[N_DIM];
	double meanPrice;
	vector <optionParams> marketData;
};
