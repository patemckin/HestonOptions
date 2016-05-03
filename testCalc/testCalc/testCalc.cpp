#include "PriceCalculation.h"
//#include "DEforHestonOLS.h"

#include "DELIB.h"

#pragma comment(lib, "fftw3.lib")

using namespace std;

struct genotype population[POPSIZE + 1];
struct genotype newpopulation[POPSIZE + 1];



int main()
{
	vector<optionParams> data(1);
	
	data[0].r = 2.2685; data[0].T = 0.126027;  data[0].S = 1544.50;  data[0].K = 1000.00; data[0].price = 559.00; data[0].bid = 553.00; data[0].ask = 565.00;
	//data[1].r = 2.2685; data[1].T = 0.126027;  data[1].S = 1544.50;  data[1].K = 1050.00; data[1].price = 509.50; data[1].bid = 503.50; data[1].ask = 515.50;
	//data[2].r = 2.2685; data[2].T = 0.126027;  data[2].S = 1544.50;  data[2].K = 1100.00; data[2].price = 460.00; data[2].bid = 454.00; data[2].ask = 466.00;
	
	//cout << data[0].price - callPriceFFT(12, data[0].S, data[0].K, data[0].T, data[0].r, 0.218418, 3.30594, 0.0617272, 0.00183837, 0.718194);


	int generation;
	int i;
	int seed;

	double min[NVARS] = { 0,0,0,0,-1 };
	double max[NVARS] = { 1,50,50,1,1 }; //здесь у нас не каппа, а 2каппа*тета-сигмаквадрат

	timestamp();
	cout << "\n";
	cout << "SIMPLE_GA:\n";
	cout << "  C++ version\n";
	cout << "  A simple example of a genetic algorithm.\n";

	if (NVARS < 2)
	{
		cout << "\n";
		cout << "  The crossover modification will not be available,\n";
		cout << "  since it requires 2 <= NVARS.\n";
	}

	seed = 123456789;

	initialize(min, max, seed, population);

	evaluate(population, data);

	keep_the_best(population);

	for (generation = 0; generation < MAXGENS; generation++)
	{
		selector(seed, population, newpopulation);
		crossover(seed, newpopulation);
		mutate(seed, population);
		report(generation, population);
		evaluate(population, data);
		elitist(population);
	}

	cout << "\n";
	cout << "  Best member after " << MAXGENS << " generations:\n";
	cout << "\n";

	for (i = 0; i < NVARS; i++)
	{
		if (i != 3) cout << "  var(" << i << ") = " << population[POPSIZE].gene[i] << "\n";
		else cout << "  var(" << i << ") = " << population[POPSIZE].gene[2]+ population[POPSIZE].gene[3]* population[POPSIZE].gene[3]/(2* population[POPSIZE].gene[1]) << "\n";
	}

	cout << "\n";
	double err = 0;
	double price = 0;
	for (int i = 0; i < data.size(); ++i) {
		price = callPriceFFT(12, data[i].S, data[i].K, data[i].T, data[i].r, population[POPSIZE].gene[0], population[POPSIZE].gene[1],
			(population[POPSIZE].gene[2] + population[POPSIZE].gene[3] * population[POPSIZE].gene[3]) / (2 * population[POPSIZE].gene[1]), population[POPSIZE].gene[3], 
			population[POPSIZE].gene[4]);
		err += (data[i].price - price)*(data[i].price - price);// / fabs(marketData[i].ask - marketData[i].bid);
	}
	cout << "  Best fitness = " << err << "\n";
	//
	//  Terminate.
	//
	cout << "\n";
	cout << "SIMPLE_GA:\n";
	cout << "  Normal end of execution.\n";
	cout << "\n";
	timestamp();


/*
	double min[N_DIM] = { 0,0,0,0,-1 };
	double max[N_DIM] = { 1,50,1,1,1 };
	//int i;

	vector<optionParams> data(3);
/*
	data[0].K = 85; data[0].S = 100; data[0].r = 0; ; data[0].T = 1; data[0].ask = 15.5; data[0].bid = 15; data[0].price = 15.2707;
	data[1].K = 90; data[1].S = 90; data[1].r = 0; ; data[1].T = 2; data[1].ask = 20.5; data[1].bid = 20; data[1].price = 20.2411;
	data[2].K = 110; data[2].S = 200; data[2].r = 0; ; data[2].T = 3; data[2].ask = 91; data[2].bid = 90;  data[2].price = 90.0500;
	*/
/*
	data[0].r = 2.2685; data[0].T = 0.126027;  data[0].S = 1544.50;  data[0].K = 1000.00; data[0].price = 559.00; data[0].bid = 553.00; data[0].ask = 565.00;
	data[1].r = 2.2685; data[1].T = 0.126027;  data[1].S = 1544.50;  data[1].K = 1050.00; data[1].price = 509.50; data[1].bid = 503.50; data[1].ask = 515.50;
	data[2].r = 2.2685; data[2].T = 0.126027;  data[2].S = 1544.50;  data[2].K = 1100.00; data[2].price = 460.00; data[2].bid = 454.00; data[2].ask = 466.00;

	DEforHestonOLE solver(min,max,data);

	printf("Calculating...\n\n");
	solver.Solve(MAX_GENERATIONS);
	double *solution = solver.Solution();


	for (int i = 0; i < 5; ++i)
		cout << "Param " << i << "\t"<< solution[i] << endl;

	cout <<"OLE Error:" << sqrt(solver.Energy())/data.size();
	*/
    return 0;
}
