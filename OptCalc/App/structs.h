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
	float S; //spot
	float K; //strike
	float T; //time to expiration
	float r; // current interest rate
	float bid; // current bid
	float ask; // current ask
	float price; // current price = (bid+ask)/2
} optionParams;

typedef struct _marketParams 
{
	float v0;
	float theta;
	float kappa;
	float sigma;
	float rho;
} marketParams;
