#include "calibrateheston.h"


#define MAX0(a) (a > 0.0 ? a : 0.0)

using namespace std;



//const gsl_rng_type* rng_type;
//gsl_rng* rng;

//void simulate(double, double, double, double, double);

/* Поправить тут ввод функции objective в solve() и будет работать
class GASolver //мать его за ногу
{
	vector <optionParams> data;
	double crossProb = 0.9;
	int popSize = 10;
	public:
		void set_data(vector <optionParams> data, double crossProb, int popSize)
		{
			this->data = data;
			this->crossProb = crossProb;
			this->popSize = popSize;
		}
		float objective(GAGenome& g)
		{
			GARealGenome& genome = (GARealGenome&)g;
			float price = 0;
			float err = 0;
						for (int i = 0; i < data.size(); ++i) {
				price = callPriceFFT(12, data[i].S, data[i].K, data[i].T, data[i].r, genome.gene(0), genome.gene(1), (genome.gene(2) + genome.gene(3) * genome.gene(3))
					/ (2 * genome.gene(1)), genome.gene(3), genome.gene(4));
				err += (data[i].price - price)*(data[i].price - price);// / fabs(marketData[i].ask - marketData[i].bid);
			}
			//err = -err;
			return err;
		}
		marketParams solve()
		{
			marketParams toreturn;
			GARealAlleleSetArray alleles;
			alleles.add(0, 1);
			alleles.add(0, 50);
			alleles.add(0, 50);
			alleles.add(0, 1);
			alleles.add(-1, 1);

			GARealGenome genome(alleles, &(GASolver::objective));
			
			GAParameterList params;
			GASteadyStateGA::registerDefaultParameters(params);
			params.set(gaNnGenerations, 10);
			params.set(gaNpopulationSize, popSize);
			params.set(gaNscoreFrequency, 10);
			params.set(gaNflushFrequency, 50);
			params.set(gaNpCrossover, crossProb);
			params.set(gaNpMutation, 0.1);
			params.set(gaNminimaxi, -1);
			params.set(gaNselectScores, (int)GAStatistics::AllScores);
	//		params.parse(argc, argv, gaFalse);	//Ух ты, это вообще что такое?

			GASteadyStateGA ga(genome);
			ga.parameters(params);
			ga.set(gaNscoreFilename, "bog.log");
			ga.evolve();
			//cout << "fit (" << gsl_rng_default_seed << ") " << ga.statistics().minEver() << ", " << sqrt(ga.statistics().minEver()) << endl;
			//	cout << "the best individual: " << ga.statistics().bestIndividual() << endl;

			GARealGenome& genomeAux = (GARealGenome&)ga.statistics().bestIndividual();

			cout << genomeAux.gene(0) << " lol " << endl;
			cout << genomeAux.gene(1) << " lol " << endl;
			cout << (genomeAux.gene(2) + genomeAux.gene(3) * genomeAux.gene(3)) / (2 * genomeAux.gene(1)) << " lol " << endl;
			cout << genomeAux.gene(3) << " lol " << endl;
			cout << genomeAux.gene(4) << " lol " << endl;

			toreturn.v0 = genomeAux.gene(0);
			toreturn.theta = genomeAux.gene(1);
			toreturn.kappa = genomeAux.gene(2);
			toreturn.sigma = genomeAux.gene(3);
			toreturn.rho = genomeAux.gene(4);

			return toreturn;
		}
};*/

vector<optionParams> data;


/*Эта функция практически дублирует int main. Она будет принимать на вход историю рынка, 
параметры для генетического алгоритма - вероятность кроссовера (а в школе его называли кроссинговером?)
и размер популяции; на выходе, соответственно, будет структурка из пяти параметров рынка. Надеюсь, она будет норм.
Неясный момент, что делать с массивом параметров рынка, который тут будет передаваться в глобальную переменную - вот этот момент
самый косяковероятный*/
marketParams getMarketParams(vector<optionParams> marketData, double crossProb, int popSize)
{
	::data.clear();
	::data = marketData;
	marketParams toreturn;
	GARealAlleleSetArray alleles;
	alleles.add(0, 1);
	alleles.add(0, 50);
	alleles.add(0, 50);
	alleles.add(0, 1);
	alleles.add(-1, 1);

	GARealGenome genome(alleles, objective);

	GAParameterList params;
	GASteadyStateGA::registerDefaultParameters(params);
	params.set(gaNnGenerations, 10);
	params.set(gaNpopulationSize, popSize);
	params.set(gaNscoreFrequency, 10);
	params.set(gaNflushFrequency, 50);
	params.set(gaNpCrossover, crossProb);
	params.set(gaNpMutation, 0.1);
	params.set(gaNminimaxi, -1);
	params.set(gaNselectScores, (int)GAStatistics::AllScores);
	//		params.parse(argc, argv, gaFalse);	//Ух ты, это вообще что такое?

	GASteadyStateGA ga(genome);
	ga.parameters(params);
	ga.set(gaNscoreFilename, "bog.log");
	ga.evolve();
	//cout << "fit (" << gsl_rng_default_seed << ") " << ga.statistics().minEver() << ", " << sqrt(ga.statistics().minEver()) << endl;
	//	cout << "the best individual: " << ga.statistics().bestIndividual() << endl;

	GARealGenome& genomeAux = (GARealGenome&)ga.statistics().bestIndividual();

	cout << genomeAux.gene(0) << " lol " << endl;
	cout << genomeAux.gene(1) << " lol " << endl;
	cout << (genomeAux.gene(2) + genomeAux.gene(3) * genomeAux.gene(3)) / (2 * genomeAux.gene(1)) << " lol " << endl;
	cout << genomeAux.gene(3) << " lol " << endl;
	cout << genomeAux.gene(4) << " lol " << endl;

	toreturn.v0 = genomeAux.gene(0);
	toreturn.theta = genomeAux.gene(1);
	toreturn.kappa = (genomeAux.gene(2) + genomeAux.gene(3) * genomeAux.gene(3)) / (2 * genomeAux.gene(1));
	toreturn.sigma = genomeAux.gene(3);
	toreturn.rho = genomeAux.gene(4);

	return toreturn;
}





int main(int argc, char * argv[]) {

	//gsl_rng_env_setup();
	//rng_type = gsl_rng_default;
	//rng = gsl_rng_alloc (rng_type);

	vector<optionParams> marketdata(1);
	//Типа нам эти данные пришли извне
	marketdata[0].r = 2.2685; marketdata[0].T = 0.126027;  marketdata[0].S = 1544.50;  marketdata[0].K = 1000.00; marketdata[0].price = 559.00; marketdata[0].bid = 553.00; marketdata[0].ask = 565.00;
	//data[1].r = 2.2685; data[1].T = 0.126027;  data[1].S = 1544.50;  data[1].K = 1050.00; data[1].price = 509.50; data[1].bid = 503.50; data[1].ask = 515.50;
	//data[2].r = 2.2685; data[2].T = 0.126027;  data[2].S = 1544.50;  data[2].K = 1100.00; data[2].price = 460.00; data[2].bid = 454.00; data[2].ask = 466.00;

	//теперь юзаем функцию

	getMarketParams(marketdata, 0.9, 10);


	return 0;
}
	/*GARealAlleleSetArray alleles;
	alleles.add(0, 1);
	alleles.add(0, 50);
	alleles.add(0, 50);
	alleles.add(0, 1);
	alleles.add(-1, 1);
	

	//GASolver solver;
	//solver.set_data(data, 0.9, 10);
	//solver.solve();
	//float(*my_func_ptr)(GAGenome& g) = objective_cheat.objective;

	/*GARealGenome genome(alleles, objective);
	
	GAParameterList params;
	GASteadyStateGA::registerDefaultParameters(params);
	params.set(gaNnGenerations, 10);
	params.set(gaNpopulationSize, 10);
	params.set(gaNscoreFrequency, 10);
	params.set(gaNflushFrequency, 50);
	params.set(gaNpCrossover, 0.9);
	params.set(gaNpMutation, 0.1);
	params.set(gaNminimaxi, -1);
	params.set(gaNselectScores, (int)GAStatistics::AllScores);
	params.parse(argc, argv, gaFalse);		//ух ты, а что же это такое? нипанятна

	GASteadyStateGA ga(genome);
	ga.parameters(params);
	ga.set(gaNscoreFilename, "bog.log");
	ga.evolve();
	//cout << "fit (" << gsl_rng_default_seed << ") " << ga.statistics().minEver() << ", " << sqrt(ga.statistics().minEver()) << endl;
//	cout << "the best individual: " << ga.statistics().bestIndividual() << endl;

	GARealGenome& genomeAux = (GARealGenome&)ga.statistics().bestIndividual();
//	printf("par (%lu) lambda = %.10f\n", gsl_rng_default_seed, genomeAux.gene(0));
//	printf("par (%lu) eta    = %.10f\n", gsl_rng_default_seed, genomeAux.gene(1));
//	printf("par (%lu) rho    = %.10f\n", gsl_rng_default_seed, genomeAux.gene(2));
//	printf("par (%lu) vbar   = %.10f\n", gsl_rng_default_seed, genomeAux.gene(3));
//	printf("par (%lu) v0     = %.10f\n", gsl_rng_default_seed, genomeAux.gene(4));
	// simulate(genomeAux.gene(0), genomeAux.gene(1), genomeAux.gene(2), genomeAux.gene(3), genomeAux.gene(4));
	
	cout << genomeAux.gene(0) << " lol " << endl;
	cout << genomeAux.gene(1) << " lol " << endl;
	cout << (genomeAux.gene(2) + genomeAux.gene(3) * genomeAux.gene(3))/(2 * genomeAux.gene(1)) << " lol " << endl;
	cout << genomeAux.gene(3) << " lol " << endl;
	cout << genomeAux.gene(4) << " lol " << endl;
	*/
 //   return 0;
//}

float objective(GAGenome& g)//, vector <optionParams> data)
{
	GARealGenome& genome = (GARealGenome&)g;

	/*float lambda = genome.gene(0);
	float eta = genome.gene(1);
	float rho = genome.gene(2);
	float vbar = genome.gene(3);
	float v0 = genome.gene(4);
	
	float S = 100;
	float K = 100;
	float r = 0.1;
	float q = 0.0;
	float tau = 1;
	float sig = 0.2;*/
	
	float price = 0;
	float err = 0;
	
	/*if (lambda*vbar*2 > eta*eta) {
		return 100.0;
	} else {*/
		//double h_call = heston_call(S, K, tau, r, v0, q, lambda, rho, eta, vbar);
		//double b_call = bsm_call(S, K, tau, r, sig, q);
		
		//float fit = h_call - b_call;

		for (int i = 0; i < ::data.size(); ++i) {
			price = callPriceFFT(12, ::data[i].S, ::data[i].K, ::data[i].T, ::data[i].r, genome.gene(0), genome.gene(1), (genome.gene(2) + genome.gene(3) * genome.gene(3))
				/ (2 * genome.gene(1)), genome.gene(3), genome.gene(4));
			err += (::data[i].price - price)*(::data[i].price - price);// / fabs(marketData[i].ask - marketData[i].bid);
		}
		//err = -err;

		
		return err;//fabs(lambda*eta / rho - vbar*v0);//fit*fit;
	//}
}
/*
void load_data(const char* filename) {
	ifstream datafile(filename);
	
	if (!datafile) {
		cerr << "File could not be opened\n";
		exit(1);
	}
	
	double aux;
	
	while (datafile >> aux) {
		if (aux == 252.0) continue;
		
	}
}

void simulate (double lambda, double eta, double rho, double vbar, double v0) {
	
//	printf ("generator type: %s\n", gsl_rng_name (rng));
//	printf ("seed = %lu\n", gsl_rng_default_seed);
//	printf ("first value = %lu\n", gsl_rng_get (rng));
	
	// vanilla parameters
	double S0 = 100;
	double K = 100;
	double r = 0.1;
	double q = 0.0;
	double T = 1;
	double sig = 0.2;
	
	int scen = 100000;
	int steps = 252*T;
	double dt = 1.0/steps;

	double S;
	double bs = 0.0;
	double bs_sq = 0.0;
	double bs_std = 0.0;
	
	double hs_1 = 0.0;
	double hs_sq_1 = 0.0;
	double hs_std_1 = 0.0;
	double H_1, V_1;
	double hs_2 = 0.0;
	double hs_sq_2 = 0.0;
	double hs_std_2 = 0.0;
	double H_2, V_2;
	double hs_3 = 0.0;
	double hs_sq_3 = 0.0;
	double hs_std_3 = 0.0;
	double H_3, V_3;
	double hs_4 = 0.0;
	double hs_sq_4 = 0.0;
	double hs_std_4 = 0.0;
	double H_4, V_4;
	
	double z1, z2;
	
	for (int i=0; i<scen; i++) {
		z1 = gsl_ran_gaussian(rng, 1);
		
		S = S0*exp( (r - q - sig*sig/2.0)*T + sig*sqrt(T)*z1 );
		
		V_1 = v0; H_1 = S0;
		V_2 = v0; H_2 = S0;
		V_3 = v0; H_3 = S0;
		V_4 = v0; H_4 = S0;
		for (int j=0 ; j<steps ; j++) {
			z1 = gsl_ran_gaussian(rng, 1);
			z2 = rho*z1 + sqrt(1 - rho*rho)*gsl_ran_gaussian(rng, 1);
			
			// type 1 - reflection rule
			H_1 = H_1 + (r - q)*H_1*dt + sqrt(V_1*dt)*H_1*z1;
			V_1 = V_1 - lambda*( V_1 - vbar )*dt + eta*sqrt( V_1*dt )*z2;
			V_1 = fabs(V_1);
			// type 2 - absorbtion rule
			H_2 = H_2 + (r - q)*H_2*dt + sqrt(V_2*dt)*H_2*z1;
			V_2 = V_2 - lambda*( V_2 - vbar )*dt + eta*sqrt( V_2*dt )*z2;
			V_2 = MAX0(V_2);
			// type 3 - Partial Truncation
			H_3 = H_3 + (r - q)*H_3*dt + sqrt(V_3*dt)*H_3*z1;
			V_3 = V_3 - lambda*( V_3 - vbar )*dt + eta*sqrt( MAX0(V_3)*dt )*z2;
			// type 4 - Full Truncation
			H_4 = H_4 + (r - q)*H_4*dt + sqrt(V_4*dt)*H_4*z1;
			V_4 = V_4 - lambda*( MAX0(V_4) - vbar )*dt + eta*sqrt( MAX0(V_4)*dt )*z2;
		}
		
		double bs_payoff = exp(-r*T)*MAX0(S - K);
		bs += bs_payoff;
		bs_sq += bs_payoff*bs_payoff;

		double hs_payoff_1 = exp(-r*T)*MAX0(H_1 - K);
		hs_1 += hs_payoff_1;
		hs_sq_1 += hs_payoff_1*hs_payoff_1;
		double hs_payoff_2 = exp(-r*T)*MAX0(H_2 - K);
		hs_2 += hs_payoff_2;
		hs_sq_2 += hs_payoff_2*hs_payoff_2;
		double hs_payoff_3 = exp(-r*T)*MAX0(H_3 - K);
		hs_3 += hs_payoff_3;
		hs_sq_3 += hs_payoff_3*hs_payoff_3;
		double hs_payoff_4 = exp(-r*T)*MAX0(H_4 - K);
		hs_4 += hs_payoff_4;
		hs_sq_4 += hs_payoff_4*hs_payoff_4;
	}
	
	gsl_rng_free(rng);
	
	bs /= scen;
	bs_std = (bs_sq/scen - bs*bs)/sqrt(scen);

	hs_1 /= scen;
	hs_std_1 = (hs_sq_1/scen - hs_1*hs_1)/sqrt(scen);
	hs_2 /= scen;
	hs_std_2 = (hs_sq_2/scen - hs_2*hs_2)/sqrt(scen);
	hs_3 /= scen;
	hs_std_3 = (hs_sq_3/scen - hs_3*hs_3)/sqrt(scen);
	hs_4 /= scen;
	hs_std_4 = (hs_sq_4/scen - hs_4*hs_4)/sqrt(scen);
	
	double bs_f = bsm_call(S0, K, T, r, sig, q);
	double hs_f = heston_call(S0, K, T, r, v0, q, lambda, rho, eta, vbar);
	
	printf("sim (%lu) Black-Scholes-Merton %.10f %.10f\n", gsl_rng_default_seed, bs, bs_std);
	printf("sim (%lu) Heston Reflection    %.10f %.10f\n", gsl_rng_default_seed, hs_1, hs_std_1);
	printf("sim (%lu) Heston Absorption    %.10f %.10f\n", gsl_rng_default_seed, hs_2, hs_std_2);
	printf("sim (%lu) Heston Partial Trun  %.10f %.10f\n", gsl_rng_default_seed, hs_3, hs_std_3);
	printf("sim (%lu) Heston Full Trunc    %.10f %.10f\n", gsl_rng_default_seed, hs_4, hs_std_4);
	printf("for (%lu) Black-Scholes-Merton %.10f\n", gsl_rng_default_seed, bs_f);
	printf("for (%lu) Heston               %.10f\n", gsl_rng_default_seed, hs_f);
	printf("\n");
}




*/