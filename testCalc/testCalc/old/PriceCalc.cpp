#include "PriceCalc.h"
#include "4FFT\include\fftw3.h"

Solver::Solver(params *parameters)
{
    this->p = parameters;
    cout << "Solver has been created" << endl;
}

Solver::~Solver()
{
    cout << "Solver has been deleted" << endl;
}

double Solver::hestonCallFft(double strike)
{
    double          x0 = log(p->s0);
    double          alpha = 1.25;
    const unsigned int    N = 4096;
    double          c = 600;
    double          eta = c / N;
    double          b = M_PI / eta;
    double          lambda = 2 * b / N;
    double          position = (log(strike) + b) / lambda + 1;
    double          u[N];
    cmplxvec        v(N);
    cmplxvec        zeta(N);
    cmplxvec        gamma(N);
    cmplxvec        PHI(N);
    cmplxvec        A(N);
    cmplxvec        B(N);
    cmplxvec        C(N);
    cmplxvec        charFunc(N);
    cmplxvec        modifiedCharFunc(N);
    cmplxvec        simpsonW(N);
    cmplxvec        fftFunc(N);


    for(int i = 0; i < N; i++)
    {
        u[i] = i * eta;

        v[i] = cmplx(u[i], -(alpha + 1));

        zeta[i] = -0.5 * (pow(v[i], 2) + v[i] * I);

        gamma[i] = p->kappa - p->rho * p->sigma * v[i] * I;

        PHI[i] = sqrt(pow(gamma[i], 2) - 2. * pow(p->sigma, 2) * zeta[i]);

        A[i] = I * v[i] *(x0 + p->r * p->T);

        cmplx tmp = 1. - exp(-PHI[i] * p->T);
        B[i] = p->v0 * ((2. * zeta[i] * tmp) / (2. * PHI[i] - (PHI[i] - gamma[i]) * tmp));

        double tmpConst = -p->kappa * p->theta / pow(p->sigma, 2);
        C[i] = tmpConst * (2. * log((2. * PHI[i] - (PHI[i] - gamma[i]) * tmp) / (2. * PHI[i])) + (PHI[i] - gamma[i]) * p->T);

        charFunc[i] = exp(A[i] + B[i] + C[i]);

        modifiedCharFunc[i] = charFunc[i] * exp(-p->r * p->T) / (pow(alpha, 2) + alpha - pow(u[i], 2) + I * (2. * alpha + 1) * u[i]);

        double kroneker;
        if (i == 0)
        {
            kroneker = 1.;
        }
        else
        {
            kroneker = 0.;
        }
        simpsonW[i] = 1. / 3. * (3. + pow(-I, i + 1) - kroneker);

        fftFunc[i] = exp(I * b * u[i]) * modifiedCharFunc[i] * eta * simpsonW[i];
    }
	fftw_complex *in, *out;
	fftw_plan plan_forward;
	in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * fftFunc.size());
	out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * fftFunc.size());
	if (in == NULL || out == NULL)
		cout << "! !Malloc trouble! !";
	else
	{
		double price = 0;
		plan_forward = fftw_plan_dft_1d(fftFunc.size(), in, out, FFTW_FORWARD, FFTW_ESTIMATE);
		for (int i = 0; i < fftFunc.size(); ++i)
		{
			in[i][0] = fftFunc[i].real();
			in[i][1] = fftFunc[i].imag();
		}
		fftw_execute(plan_forward);

		price = exp(-log(strike)*alpha)*out[(int)position][0]/M_PI;


		fftw_destroy_plan(plan_forward);
		fftw_free(in);
		fftw_free(out);

		return price;
	}
	

    return 0;
}

double Solver::hestonCallFftTimeValue(double strike)
{
    double          x0 = log(p->s0);
    double          alpha = 1.25;
    const unsigned int    N = 4096;
    double          c = 600;
    double          eta = c / N;
    double          b = M_PI / eta;
    double          lambda = 2 * b / N;
    double          position = (log(strike) + b) / lambda + 1;
    double          u[N];
    cmplxvec        v1(N);
    cmplxvec        v2(N);
    cmplxvec        w1(N);
    cmplxvec        w2(N);
    cmplxvec        zeta1(N);
    cmplxvec        zeta2(N);
    cmplxvec        gamma1(N);
    cmplxvec        gamma2(N);
    cmplxvec        PHI1(N);
    cmplxvec        PHI2(N);
    cmplxvec        A1(N);
    cmplxvec        A2(N);
    cmplxvec        B1(N);
    cmplxvec        B2(N);
    cmplxvec        C1(N);
    cmplxvec        C2(N);
    cmplxvec        charFunc1(N);
    cmplxvec        charFunc2(N);
    cmplxvec        modifiedCharFunc1(N);
    cmplxvec        modifiedCharFunc2(N);
    cmplxvec        modifiedCharFuncCombo(N);
    cmplxvec        simpsonW(N);
    cmplxvec        fftFunc(N);


    for(int i = 0; i < N; i++)
    {
        u[i] = i * eta;

        v1[i] = u[i] - I * alpha - I;

        w1[i] = u[i] - I * alpha;

        zeta1[i] = -0.5 * (pow(v1[i], 2) + v1[i] * I);

        gamma1[i] = p->kappa - p->rho * p->sigma * v1[i] * I;

        PHI1[i] = sqrt(pow(gamma1[i], 2) - 2. * pow(p->sigma, 2) * zeta1[i]);

        A1[i] = I * v1[i] *(x0 + p->r * p->T);

        cmplx tmp = 1. - exp(-PHI1[i] * p->T);
        B1[i] = p->v0 * ((2. * zeta1[i] * tmp) / (2. * PHI1[i] - (PHI1[i] - gamma1[i]) * tmp));

        double tmpConst = -p->kappa * p->theta / pow(p->sigma, 2);
        C1[i] = tmpConst * (2. * log((2. * PHI1[i] - (PHI1[i] - gamma1[i]) * tmp) / (2. * PHI1[i])) + (PHI1[i] - gamma1[i]) * p->T);

        charFunc1[i] = exp(A1[i] + B1[i] + C1[i]);

        modifiedCharFunc1[i] = exp(-p->r * p->T) * (1. / (1. + I * w1[i]) - exp(p->r * p->T) / (I * w1[i])
                                - charFunc1[i] / (pow(w1[i], 2) - I * w1[i]));

        v2[i] = u[i] + I * alpha - I;

        w2[i] = u[i] + I * alpha;

        zeta2[i] = -0.5 * (pow(v2[i], 2) + v2[i] * I);

        gamma2[i] = p->kappa - p->rho * p->sigma * v2[i] * I;

        PHI2[i] = sqrt(pow(gamma2[i], 2) - 2. * pow(p->sigma, 2) * zeta2[i]);

        A2[i] = I * v2[i] *(x0 + p->r * p->T);

        tmp = 1. - exp(-PHI2[i] * p->T);
        B2[i] = p->v0 * ((2. * zeta2[i] * tmp) / (2. * PHI2[i] - (PHI2[i] - gamma2[i]) * tmp));

        tmpConst = -p->kappa * p->theta / pow(p->sigma, 2);
        C2[i] = tmpConst * (2. * log((2. * PHI2[i] - (PHI2[i] - gamma2[i]) * tmp) / (2. * PHI2[i])) + (PHI2[i] - gamma2[i]) * p->T);

        charFunc2[i] = exp(A2[i] + B2[i] + C2[i]);

        modifiedCharFunc2[i] = exp(-p->r * p->T) * (1. / (1. + I * w2[i]) - exp(p->r * p->T) / (I * w2[i])
                                - charFunc2[i] / (pow(w2[i], 2) - I * w2[i]));

        modifiedCharFuncCombo[i] = (modifiedCharFunc1[i] - modifiedCharFunc2[i]) / 2.;

        double kroneker;
        if (i == 0)
        {
            kroneker = 1.;
        }
        else
        {
            kroneker = 0.;
        }
        simpsonW[i] = 1. / 3. * (3. + pow(-1, i + 1) - kroneker);

        fftFunc[i] = exp(I * b * u[i]) * modifiedCharFuncCombo[i] * eta * simpsonW[i];
    }

    return 0;
}

