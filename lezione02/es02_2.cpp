#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "random.h"

using namespace std;

double err ( double AV, double AV2, int n);

int main (){

	Random rnd;

	int M=1E4;				//n of throws
	int N=100;				//n of blocks
	int L=M/N;
	int steps=100;

	double dist[steps+1], sum[steps+1];
	double ave[N][steps+1], av2[N][steps+1];		// first index: BLOCK, second index: STEP
	double sum_prog[steps+1], su2_prog[steps+1], err_prog[steps+1];

	//1 Discrete

	for (int k=0; k<N; k++){
		for (int i=0; i<steps+1; i++){
			sum[i] = 0;
		}
		for ( int j=0; j<L; j++){
			int x=0;
			int y=0;
			int z=0;
			for(int i=1; i<steps+1; i++){		//generating random motion
				int ran = rnd.Rannyu(1,7);
				if(ran==1)
					x++;
				else if(ran==2)
					x--;
				else if(ran==3)
					y++;
				else if(ran==4)
					y--;
				else if(ran==5)
					z++;
				else
					z--;
				dist[i] = (x*x+y*y+z*z);	//r^2
			}
			for (int i=0; i<steps+1; i++){
				sum[i] += dist[i];
			}
		}
		for( int i=0; i<steps+1; i++){
			ave[k][i] = sum[i]/L;
			av2[k][i] = (ave[k][i])*(ave[k][i]);
		}
	}

	for ( int j=0; j<N; j++){				//only the last block in computed,
		for( int i=0; i<steps+1; i++){			//we don't need the rest of them
			sum_prog[i] += ave[j][i];		//i is still index of the steps,
			su2_prog[i] += av2[j][i];		//not of the blocks
		}
	}
	for( int i=0; i<steps+1; i++){
		sum_prog[i] /= N;				//cumulative average
		su2_prog[i] /= N;				//cumulative square average
		err_prog[i] = err(sum_prog[i], su2_prog[i], N);	//statistical uncertainty
	}
	
	ofstream out;						//output data
	out.open ("data_discreteRW.out"); 
	for( int i=0; i<steps+1; i++)
		out << i << " " << sqrt(sum_prog[i]) << " " << 0.5/sqrt(sum_prog[i])*err_prog[i] <<  endl;	//err[sqrt(x)]=0.5*x^(-1/2)*err[x]
	out.close();

	//2 Continuum

	for( int i=0; i<steps+1; i++){				//resetting vectors
		for( int k=0; k<N; k++){
			ave[k][i]=0.;
			av2[k][i]=0.;
		}
		sum_prog[i]=0.;
		su2_prog[i]=0.;
		err_prog[i]=0.;
	}

	for (int k=0; k<N; k++){
		for (int i=0; i<steps+1; i++){
			sum[i] = 0;
		}
		for ( int j=0; j<L; j++){
			double x=0.;
			double y=0.;
			double z=0.;
			for(int i=1; i<steps+1; i++){		//to generate the direction we generate points inside
				double x_rnd=0;			//a square until one falls inside the inside sphere
				double y_rnd=0;			//of radius 1
				double z_rnd=0;

				double norm=0;
				do{
					x_rnd=rnd.Rannyu(-1,1);
					y_rnd=rnd.Rannyu(-1,1);
					z_rnd=rnd.Rannyu(-1,1);
					norm=sqrt(x_rnd*x_rnd+y_rnd*y_rnd+z_rnd*z_rnd);
				}while(norm>=1);

				x_rnd=x_rnd/norm;		//the point is now normalized so that
				y_rnd=y_rnd/norm;		//it falls on the sphere and not inside it
				z_rnd=z_rnd/norm;
				x += x_rnd;
				y += y_rnd;
				z += z_rnd;
				dist[i] = (x*x+y*y+z*z);
			}
			for (int i=0; i<steps+1; i++){
				sum[i] += dist[i];
			}
		}
		for( int i=0; i<steps+1; i++){
			ave[k][i] = sum[i]/L;
			av2[k][i] = (ave[k][i])*(ave[k][i]);
		}
	}

	for ( int j=0; j<N; j++){				//again only the last block in computed
		for( int i=0; i<steps+1; i++){
			sum_prog[i] += ave[j][i];
			su2_prog[i] += av2[j][i];
		}
	}
	for( int i=0; i<steps+1; i++){
		sum_prog[i] /= N;				//cumulative average
		su2_prog[i] /= N;				//cumulative square average
		err_prog[i] = err(sum_prog[i], su2_prog[i], N);	//statistical uncertainty
	}
	
						//output data
	out.open ("data_continuumRW.out"); 
	for( int i=0; i<steps+1; i++)
		out << i << " " << sqrt(sum_prog[i]) << " " << 0.5/sqrt(sum_prog[i])*err_prog[i] <<  endl;	//err[sqrt(x)]=0.5*x^(-1/2)*err[x]
	out.close();
	

	return 0;
}

double err ( double AV, double AV2, int n){
	if (n==0)
		return 0;
	else
		return sqrt((AV2-AV*AV)/n);
}
