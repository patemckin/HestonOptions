#pragma once

typedef struct _dataForFFT
{
	unsigned int	N;
	double			S;
	double			K;
	double			r;
	double			T;
	double			v0;
	double			theta;
	double			kappa;
	double			sigma;
	double			rho;
} PriceFunParams;

typedef struct _optionParams
{
	double S; //spot
	double K; //strike
	double T; //time to expiration
	double r; // current interest rate
	double price; // current price
} optionParams;
