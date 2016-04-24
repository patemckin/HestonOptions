#include "PriceCalculation.h"

double callPriceFFT(int N, params *p)
{
    double          lnS = log(p->S);
    double          lnK = log(p->K);
    double          optAlpha = .75;
    double          discountFactor = exp(-p->r * p->T);
    uint64_t        FFT_N = pow(2, N);
    double          FFT_eta = 0.05;
    double          FFT_lambda = (2 * M_PI) / (FFT_N * FFT_eta);
    double          FFT_b = (FFT_N * FFT_lambda) / 2;
    double          jvec, vj, omega2, gamma, *ku, *cpvec;
    cmplx           u, alpha, beta, D, bD, eDt, G, B, psi, A, phi, tmp;
    fftw_complex    *fftFunc, *out;
    fftw_plan       plan_forward;
    uint64_t        indexOfStrike;
    double          x[3], y[3], price;

    fftFunc = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * FFT_N);
    out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * FFT_N);
    if (fftFunc == NULL || out == NULL)
    {
        cout << "Malloc trouble for fft";
        return -1;
    }
    ku = new double[FFT_N];
    cpvec = new double[FFT_N];

    for(int i = 0; i < FFT_N; i++)
    {
        jvec = i + 1;
        vj = (jvec - 1) * FFT_eta;

        /** Heston_characteristicFn - begin */
        u = cmplx(vj, - (optAlpha + 1));
        alpha = -0.5 * (u * u + u * I);
        beta = p->kappa - p->rho * p->sigma * u * I;
        omega2 = p->sigma * p->sigma;
        gamma = 0.5 * omega2;
        D = sqrt(beta * beta - 4.0 * alpha * gamma);
        bD = beta - D;
        eDt = exp(-D * p->T);
        G = bD / (beta + D);
        B = (bD / omega2) * ((1.0 - eDt) / (1.0 - G * eDt));
        psi = (G * eDt - 1.0) / (G - 1.0);
        A = ((p->kappa * p->theta) / (omega2)) * (bD * p->T - 2.0 * log(psi));
        phi = A + B * p->v0 + I * u * (lnS + p->r * p->T);
        /** Heston_characteristicFn - end */

        tmp = discountFactor * exp(phi) / (optAlpha * optAlpha + optAlpha - vj * vj +
                            I * (2.0 * optAlpha + 1) * vj) * exp(I * vj * (FFT_b)) * FFT_eta;

        fftFunc[i][0] = real((tmp / 3.0) * (3.0 + pow((-1), jvec) - ((jvec - 1) == 0)));
        fftFunc[i][1] = imag((tmp / 3.0) * (3.0 + pow((-1), jvec) - ((jvec - 1) == 0)));
    }

    plan_forward = fftw_plan_dft_1d(FFT_N, fftFunc, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan_forward);

    for(int i = 0; i < FFT_N; i++)
    {
        ku[i] = - FFT_b + FFT_lambda * i;
        cpvec[i] = real(exp(-optAlpha * ku[i]) * cmplx(out[i][0], out[i][1]) / M_PI);
    }

    indexOfStrike = floor((lnK + FFT_b) / FFT_lambda + 1);

    /** Interpolation - begin */
    x[0] = ku[indexOfStrike + 1];
    y[0] = cpvec[indexOfStrike + 1];
    x[1] = ku[indexOfStrike];
    y[1] = cpvec[indexOfStrike];
    x[2] = ku[indexOfStrike - 1];
    y[2] = cpvec[indexOfStrike - 1];

    if ((lnK >= x[0]) && (lnK < x[1]))
        price = y[0] + (y[1] - y[0]) / (x[1] - x[0]) * (lnK - x[0]);
    else
        price = y[1] + (y[2] - y[1]) / (x[2] - x[1]) * (lnK - x[1]);
   /** Interpolation - end */


    fftw_destroy_plan(plan_forward);
    fftw_free(fftFunc);
    fftw_free(out);

    return price;
}
