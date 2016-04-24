#include "PriceCalculation.h"
#include "DETest.h"

using namespace std;

int main()
{
    params *p = new params;



    /*p->kappa = 2;
    p->theta = 0.01;
    p->sigma = 0.225;
    p->v0 = 0.01;
    p->rho = 0;
    p->T = 0.00001;
    p->r = 0;
    p->s0 = 100;

    Solver *sol = new Solver(p);
   double kek = sol->hestonCallFft(90.);
   */

	double min[N_DIM];
	double max[N_DIM];
	int i;
	//double c = erf(0.5);
	//printf("%lf", c);
	double S = 100; //spot price
	double K = 85; //Strike price
	double r = 0.00; //Risk-free interest rate
	double delta = 0; //Dividend yield
	double V0 = 0.01; //Initial volatility//trial[0]
	double tau = 5; //Time to maturity (years)
	double thet = 0.01; //Long-run volatility//trial[1]
	double kappa = 2; //Mean-reversion rate for volatility//trial[2]
	double SigmaV = 0.225; //Volatility of volatility//trial[3]
	double rho = 0;//; //Price-volatility correlation//trial[4]
	double gamma = 0; //Risk-aversion parameter
	PolynomialSolver solver(N_DIM,N_POP);

	for (i=0;i < N_DIM;i++)
	{
		max[i] =  1.0;
		min[i] = 0.0;
	}

	solver.Setup(min,max,stBest1Exp,0.9,1.0);

	printf("Calculating...\n\n");
	solver.Solve(MAX_GENERATIONS);

	double *solution = solver.Solution();

	/*for (i=0;i < N_DIM;i++)
		printf("[%d]: %lf\n",i,solution[i]);
		*/

	p->kappa = solution[2];
	p->theta = solution[1];
	p->sigma = solution[3];
	p->v0 = solution[0];
	p->rho = solution[4];
	p->r = r;
	p->K = K;
	p->T = tau;
	p->S = S;
	printf("%lf\n",(callPriceFFT(18, p)));





  // cout << kek << " !!";
    //double kek = sol->hestonCallFftTimeValue(85.);

    delete p;
    //delete sol;

    return 0;
}
