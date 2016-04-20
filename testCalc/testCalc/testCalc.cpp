#include "PriceCalc.h"

using namespace std;

int main()
{
    params *p = new params;

    p->kappa = 2;
    p->theta = 0.01;
    p->sigma = 0.225;
    p->v0 = 0.01;
    p->rho = 0;
    p->T = 0.00001;
    p->r = 0;
    p->s0 = 100;

    Solver *sol = new Solver(p);
   double kek = sol->hestonCallFft(90.);

   cout << kek << " !!";
    //double kek = sol->hestonCallFftTimeValue(85.);

    delete p;
    delete sol;

    return 0;
}
