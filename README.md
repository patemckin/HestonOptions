# HestonOptions
This repository contains implementation of the price calculation and market parameters calibration from Heston Model for pricing European call options with stochastic volatility. 

Price calculation implemetation inspired by Carr & Madan 1999 paper "Option valuation using the fast Fourier transform". It uses fast fourier transform, which were taken from fftw library (http://www.fftw.org/).

Ideas for market parameters calibration were taken from Nimalin Moodley's 2005 paper "The Heston Model : A Practical Approach" , improved and implemented using genetic algorithm from GaLib (http://lancet.mit.edu/ga/)

