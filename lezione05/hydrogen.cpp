#include "hydrogen.h"
#include "random.h"
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>

Hydrogen :: Hydrogen(int n_sim, int n_blocks, double initialX, double initialY, double initialZ){
	M = n_sim;
	N = n_blocks;
	L = M/N;
	for(int i=0; i< M; i++){
		x.push_back(initialX);		//sets initial position
		y.push_back(initialY);	
		z.push_back(initialZ);
	}
	for(int i=0; i< N; i++){
		ave.push_back(0);
		av2.push_back(0);
		sum_prog.push_back(0);
		su2_prog.push_back(0);
		err_prog.push_back(0);
	}
}

Hydrogen :: ~Hydrogen(){}

void Hydrogen :: equilibrate( int cicle, int state, char distr ){
//used to equilibrate the state with "cicle" steps and to check
//acceptance rate if needed using acc
	acc = 0;
	double xnew = 0;
	double ynew = 0;
	double znew = 0;
	Random rnd;
	for(int i=0; i<cicle; i++){
		if( distr == 'u' ){
			//uniform distribution
			double xdelta = rnd.Rannyu(-delta, delta);
			double ydelta = rnd.Rannyu(-delta, delta);
			double zdelta = rnd.Rannyu(-delta, delta);
			xnew = x[i] + xdelta;
			ynew = y[i] + ydelta;
			znew = z[i] + zdelta;
		}
		if( distr == 'g' ){
			//gaussian distribution
			xnew = rnd.Gauss(x[i], delta);
			ynew = rnd.Gauss(y[i], delta);
			znew = rnd.Gauss(z[i], delta);
		}
		double A=0;				//Metropolis
		if( state == 0 )
			A = min( 1., Prob0(xnew, ynew, znew)/Prob0(x[i], y[i], z[i]) );
		if( state == 1 )
			A = min( 1., Prob1(xnew, ynew, znew)/Prob1(x[i], y[i], z[i]) );
		double r = rnd.Rannyu();
		if ( r <= A ){
			acc++;
			x[i+1] = xnew;
			y[i+1] = ynew;
			z[i+1] = znew;
		}
		else{
				x[i+1] = x[i];
				y[i+1] = y[i];
				z[i+1] = z[i];
			}
	}
	setX( x[cicle-1] );		//replace starting position with
	setY( y[cicle-1] );		//last position of equilibration
	setZ( z[cicle-1] );
}

void Hydrogen :: Metropolis( int state, char distr ){
	fill(ave.begin(), ave.end(),0);
	fill(av2.begin(), av2.end(),0);
	fill(sum_prog.begin(), sum_prog.end(),0);
	fill(su2_prog.begin(), su2_prog.end(),0);
	fill(err_prog.begin(), err_prog.end(),0);
	Random rnd;
	double xnew = 0;
	double ynew = 0;
	double znew = 0;
	for (int i=0; i<N; i++){
		double sum = 0;
		for ( int j=0; j<L; j++){
			int k = i*L+j;
			if( distr == 'u' ){	
				//uniform distribution
				double xdelta = rnd.Rannyu(-delta, delta);
				double ydelta = rnd.Rannyu(-delta, delta);
				double zdelta = rnd.Rannyu(-delta, delta);
				xnew = x[k] + xdelta; 
				ynew = y[k] + ydelta; 
				znew = z[k] + zdelta;
			}
			if( distr == 'g' ){
				//gaussian distribution
				xnew = rnd.Gauss(x[k], delta);
				ynew = rnd.Gauss(y[k], delta);
				znew = rnd.Gauss(z[k], delta);
			}
 			double A=0;				//Metropolis
			if( state == 0 )
 				A = min( 1., Prob0(xnew, ynew, znew)/Prob0(x[k], y[k], z[k]) );
			if( state == 1 )
 				A = min( 1., Prob1(xnew, ynew, znew)/Prob1(x[k], y[k], z[k]) );
			double r = rnd.Rannyu();
			if ( r <= A ){
				x[k+1] = xnew;
				y[k+1] = ynew;
				z[k+1] = znew;
			}
			else{
				x[k+1] = x[k];
				y[k+1] = y[k];
				z[k+1] = z[k];
			}
			sum += sqrt( x[k]*x[k]+y[k]*y[k]+z[k]*z[k]);
		}
		ave[i] = sum/L;
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
}

void Hydrogen :: writeBlocks( string name ){
	ofstream out;
	out.open ( name ); 
	for( int i=0; i<N; i++)
		out << (i+1) << " " << sum_prog[i] << " " << err_prog[i] << endl;
	out.close();
}

void Hydrogen :: writePoints( string name, int num ){
	ofstream out;
	out.open ( name );
	for( int i=0; i<M; i++){
		if( i%num == 0 ){			//only writes a point every "num"
			out << x[i] << " " << y[i] << " " << z[i] << endl;
		}
	}
	out.close();
}

void Hydrogen :: writeInstantValues( string name, int lenght ){
	ofstream out;
	out.open ( name ); 
	for( int i=1; i<lenght; i++)
		out << i+1 << " " << sqrt(x[i]*x[i]+y[i]*y[i]+z[i]*z[i]) << endl;
	out.close();
}

void Hydrogen :: writeInstantPoints( string name, int lenght ){
	ofstream out;
	out.open ( name );
	for( int i=1; i<lenght; i++)
		out << x[i] << " " << y[i] << " " << z[i] << endl;
	out.close();
}
double Hydrogen :: Prob0( double x, double y, double z){
	return exp(-2*sqrt(x*x+y*y+z*z)) / M_PI;
}

double Hydrogen :: Prob1( double x, double y, double z){
	return z*z*exp(-sqrt(x*x+y*y+z*z)) / (32*M_PI);
}

double Hydrogen :: err(vector<double> AV, vector<double> AV2, int n){
	if (n==0)
		return 0;
	else
		return sqrt((AV2[n]-AV[n]*AV[n])/n);
}
