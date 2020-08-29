#include "pricing.h"
#include "random.h"
#include <cmath>
#include <iostream>

European :: European(int n_sim, int n_blocks, double asset, double delivery, double strike, double interest, double vol){
	m_M = n_sim;
	m_N = n_blocks;
	m_S = asset;
	m_T = delivery;
	m_K = strike;
	m_r = interest;
	m_sigma = vol;
}

European :: ~European(){}


void European :: call( int n_step ){
	avg.clear();
	inc.clear();

	int L = m_M/m_N;
	vector<double> ave(m_N,0), av2(m_N,0), sum_prog(m_N,0), su2_prog(m_N,0), err_prog(m_N,0);
	Random rnd;
	for (int i=0; i<m_N; i++){
		double sum = 0;
		for ( int j=0; j<L; j++){
			double s = m_S;
			double step = (double) m_T/n_step;
			for(int l=0; l<n_step; l++){		//sampling the GBM
				s *= exp( (m_r-0.5*m_sigma*m_sigma)*step+m_sigma*rnd.Gauss(0,1)*sqrt(step) );
			}
			sum += exp(-m_r*m_T)*max(0., s-m_K);	//price of call option
		}
		ave[i] = sum/L;
		av2[i] = (ave[i])*(ave[i]);
	}

	for (int i=0; i<m_N; i++){
		for ( int j=0; j<i+1; j++){
			sum_prog[i] += ave[j];
			su2_prog[i] += av2[j];
		}
		sum_prog[i] /= (i+1);				//cumulative average
		su2_prog[i] /= (i+1);				//cumulative square average
		err_prog[i] = err(sum_prog, su2_prog, i);	//statistical uncertainty
	}

	for(int i=0; i<m_N; i++){
		avg.push_back( sum_prog[i] );
		inc.push_back( err_prog[i] );
	}
	
}

void European :: put( int n_step ){
	avg.clear();
	inc.clear();

	int L = m_M/m_N;
	vector<double> ave(m_N,0), av2(m_N,0), sum_prog(m_N,0), su2_prog(m_N,0), err_prog(m_N,0);
	Random rnd;
	for (int i=0; i<m_N; i++){
		double sum = 0;
		for ( int j=0; j<L; j++){
			double s = m_S;
			double step = (double) m_T/n_step;
			for(int l=0; l<n_step; l++){		//sampling the GBM
				s *= exp( (m_r-0.5*m_sigma*m_sigma)*step+m_sigma*rnd.Gauss(0,1)*sqrt(step) );
			}
			sum += exp(-m_r*m_T)*max(0., m_K-s);	//price of put option
		}
		ave[i] = sum/L;
		av2[i] = (ave[i])*(ave[i]);
	}

	for (int i=0; i<m_N; i++){
		for ( int j=0; j<i+1; j++){
			sum_prog[i] += ave[j];
			su2_prog[i] += av2[j];
		}
		sum_prog[i] /= (i+1);				//cumulative average
		su2_prog[i] /= (i+1);				//cumulative square average
		err_prog[i] = err(sum_prog, su2_prog, i);	//statistical uncertainty
	}

	for(int i=0; i<m_N; i++){
		avg.push_back( sum_prog[i] );
		inc.push_back( err_prog[i] );
	}
	
}

double European :: err( vector<double> AV, vector<double> AV2, int n){
	if (n==0)
		return 0;
	else
		return sqrt((AV2[n]-AV[n]*AV[n])/n);
}
	
