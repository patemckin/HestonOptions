#include "PriceCalculation.h"
#include "DEforHestonOLS.h"

#pragma comment(lib, "fftw3.lib")

using namespace std;

int main()
{

	double min[N_DIM] = { 0,0,0,0,-1 };
	double max[N_DIM] = { 1,50,1,1,1 };
	//int i;

	vector<optionParams> data(3);
/*
	data[0].K = 85; data[0].S = 100; data[0].r = 0; ; data[0].T = 1; data[0].ask = 15.5; data[0].bid = 15; data[0].price = 15.2707;
	data[1].K = 90; data[1].S = 90; data[1].r = 0; ; data[1].T = 2; data[1].ask = 20.5; data[1].bid = 20; data[1].price = 20.2411;
	data[2].K = 110; data[2].S = 200; data[2].r = 0; ; data[2].T = 3; data[2].ask = 91; data[2].bid = 90;  data[2].price = 90.0500;
	*/

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

    return 0;
}
