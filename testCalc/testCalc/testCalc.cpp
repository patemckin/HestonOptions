#include "PriceCalculation.h"
#include "DEforHestonOLE.h"

using namespace std;




int main()
{

	double min[N_DIM] = { 0,0,0,0,-1 };
	double max[N_DIM] = { 1,50,1,1,1 };
	int i;

	vector<optionParams> data(3);

	data[0].K = 85; data[0].S = 100; data[0].r = 0; ; data[0].T = 1; data[0].price = 15.2707;
	data[1].K = 90; data[1].S = 90; data[1].r = 0; ; data[1].T = 2; data[1].price = 20.2411;
	data[2].K = 110; data[2].S = 200; data[2].r = 0; ; data[2].T = 4; data[2].price = 90.0500;


	DEforHestonOLE solver(min,max,data);

	printf("Calculating...\n\n");
	solver.Solve(MAX_GENERATIONS);
	double *solution = solver.Solution();


	for (int i = 0; i < 5; ++i)
		cout << "Param " << i << "\t"<< solution[i] << endl;

	cout <<"OLE Error:" << sqrt(solver.Energy())/data.size();

    return 0;
}
