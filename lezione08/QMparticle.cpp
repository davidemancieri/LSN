#include "QMparticle.h"
#include "random.h"
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>

QMparticle :: QMparticle(int n_sim, int n_blocks, double initialX){
	M = n_sim;
	N = n_blocks;
	L = M/N;
	x.push_back(initialX);
	for(int i=0; i< N; i++){
		ave.push_back(0);
		av2.push_back(0);
		sum_prog.push_back(0);
		su2_prog.push_back(0);
		err_prog.push_back(0);
	}
}	
	
QMparticle :: ~QMparticle(){};

void QMparticle :: Optimization( double startMu, double endMu, double startSigma, double endSigma, double var){
	Random rnd;
	double bestE=1000;
	double bestMu, bestSigma;
        for(double mu=startMu; mu<=endMu; mu+=var){
       		for(double sigma=startSigma; sigma<=endSigma; sigma+=var){
       			acc = 0;
			double xnew = 0.;
			double x = 0.;
			double sum = 0.;
                	for(int i=0; i<M; i++){
                  		double xdelta = rnd.Rannyu(-delta, delta);
				xnew = x + xdelta;
                    		double A=min(1., pow(Psi(xnew,mu,sigma),2.)/pow(Psi(x,mu,sigma),2.));
                         	double r = rnd.Rannyu();
                   		if( r <= A ){
					acc++;
                        		x = xnew;
                   		}
                    		sum += ( -d2Psi(x,mu,sigma)*0.5+Potential(x)*Psi(x,mu,sigma))/(Psi(x,mu,sigma) );
                	}
			sum /= M;
			cout << mu << "		" << sigma << "		" << sum << "		" << acc/(double)M << "		" << endl;
                	if(sum < bestE){
                  		bestE = sum;
                    		bestMu = mu; 
				bestSigma = sigma;
                	}
           	}
        }
	cout << endl << "Optimized parameters " << endl << "Mu: "  << bestMu << endl << "Sigma: " << bestSigma << endl <<  "Energy: " << bestE << endl;
} 	
 
void QMparticle :: Metropolis( double mu, double sigma){
	fill(ave.begin(), ave.end(),0);
	fill(av2.begin(), av2.end(),0);
	fill(sum_prog.begin(), sum_prog.end(),0);
	fill(su2_prog.begin(), su2_prog.end(),0);
	fill(err_prog.begin(), err_prog.end(),0);	
	Random rnd;
	acc = 0;
	double xnew = 0.;
	for(int i=0; i<N; i++){
		double sum=0;		
		for(int j=0; j<L; j++){
			int k = i*L+j;
			double xdelta = rnd.Rannyu(-delta, delta);
			xnew = x[k] + xdelta;
			double A = min( 1., pow(Psi(xnew,mu,sigma),2.)/pow(Psi(x[k],mu,sigma),2.) );	
			double r = rnd.Rannyu();
                   	if( r <= A ){
				acc++;
				x.push_back(xnew);
			}

			else{
				x.push_back(x[k]);
			}
			sum += ( -d2Psi(x[k+1],mu,sigma)*0.5+Potential(x[k+1])*Psi(x[k+1],mu,sigma))/(Psi(x[k+1],mu,sigma) );
		}
		ave[i] = sum/L;			
		av2[i] = (ave[i])*(ave[i]);
	}

	for(int i=0; i<N; i++){
		for( int j=0; j<i+1; j++){
			sum_prog[i] += ave[j];
			su2_prog[i] += av2[j];
		}

		sum_prog[i] /= (i+1);            		//Cumulative average				
    		su2_prog[i] /= (i+1);				//Cumulative square average		
		err_prog[i] = err(sum_prog, su2_prog, i);	//statistical uncertainty			
	}
}

void QMparticle :: writeBlocks( string name ){
	ofstream out;
	out.open ( name ); 
	for( int i=0; i<N; i++)
		out << (i+1) << " " << sum_prog[i] << " " << err_prog[i] << endl;
	out.close();
}

void QMparticle :: writeInstantPoints( string name, int lenght ){
	ofstream out;
	out.open ( name );
	for( int i=1; i<=lenght; i++)
		out << x[i] << endl;
	out.close();
}

double QMparticle :: Psi(double x, double mu, double sigma){
	return exp(-pow(x-mu,2.)/(2.*sigma*sigma)) + exp(-pow(x+mu,2.)/(2.*sigma*sigma));
}

double QMparticle :: d2Psi(double x, double mu, double sigma){
	double e_neg = exp(-pow(x-mu,2.)/(2.*sigma*sigma))*(pow((x-mu)/(sigma*sigma),2.)-(1./(sigma*sigma)));
	double e_pos = exp(-pow(x+mu,2.)/(2.*sigma*sigma))*(pow((x+mu)/(sigma*sigma),2.)-(1./(sigma*sigma)));
	
	return e_neg + e_pos;
}

double QMparticle :: Potential(double x){

	return pow(x,4)-2.5*pow(x,2);

}

double QMparticle :: err(vector<double> AV, vector<double> AV2, int n){
	if (n==0)
		return 0;
	else
		return sqrt((AV2[n]-AV[n]*AV[n])/n);
}
