#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "random.h"

using namespace std;

double err (vector<double>, vector<double>, int);

int main (){

	Random rnd;

	double L=5;						//lenght of the needle
	double d=10;						//space between the lines
	int M=1E5;						//n of throws
	int N=100;						//n of blocks
	int Q=M/N;
	vector<double> ave(N,0), av2(N,0), sum_prog(N,0), su2_prog(N,0), err_prog(N,0);
	vector<double> x;					//position of the centre of the needle
	vector<double> theta;					//angle of the inclination of the needle
	vector<int> P;	
	for(int j=0; j<M; j++)					//x is random uniform (0,d)
		x.push_back(rnd.Rannyu(0, d));
	for(int j=0; j<M; j++){
		double rndX = 1;
		double rndY = 1;
		while( rndX*rndX+rndY*rndY >= 1 ){		//theta is random uniform (0,pi)
			rndX = rnd.Rannyu(-1, 1);		//obtained without knowing the value of pi
			rndY = rnd.Rannyu(0, 1);		//generating rndX and rndY uniform
		}
		theta.push_back( acos(rndX/(sqrt(rndX*rndX+rndY*rndY))) );
	}
	for(int i=0; i<N; i++)
		P.push_back(i);

	//Monte Carlo
	for (int i=0; i<N; i++){
		int nHit = 0;
		for ( int j=0; j<Q; j++){
			int k = j+i*Q;
			if( x[k]+L/2*sin(theta[k]) >= d or x[k]-L/2*sin(theta[k]) <= 0 )
				nHit++;				//checks if the needle crosses a line
		}
		ave[i] = 2*L*Q/(nHit*d);			// pi=2L/Pd
		av2[i] = (ave[i])*(ave[i]);
	}

	for (int i=0; i<N; i++){
		for ( int j=0; j<i+1; j++){
			sum_prog[i] += ave[j];
			su2_prog[i] += av2[j];
		}
		sum_prog[i] /= (i+1);				//cumulative average
		su2_prog[i] /= (i+1);				//cumulative square average
		err_prog[i] = err(sum_prog, su2_prog, i);	//statistical uncertainty
	}

	ofstream out;						//output data
	out.open ("data_buffon.out"); 
	for( int i=0; i<N; i++)
		out << (P[i]+1) << " " << sum_prog[i] << " " << err_prog[i] << endl;
	out.close();

	return 0;
}	

double err ( vector<double> AV, vector<double> AV2, int n){
	if (n==0)
		return 0;
	else
		return sqrt((AV2[n]-AV[n]*AV[n])/n);
}	
