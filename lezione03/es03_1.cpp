#include <iostream>
#include <fstream>

#include "random.h"
#include "pricing.h"

using namespace std;
 
int main (){
	
	int M=1E5;
	int N=100;

	European eu(M, N, 100, 1, 100, 0.1, 0.25);	//simulations, blocks, asset price, delivery time, strike price, risk-free interest rate, volatility
	ofstream out;

	//Call direct
	cout << "Call direct..." << endl;
	eu.call(1);
	out.open ("data_callDirect.out"); 
	for( int i=0; i<N; i++)
		out << (i+1) << " " << eu.avg[i] << " " << eu.inc[i] << endl;
	out.close();

	//Put direct
	cout << "Put direct..." << endl;
	eu.put(1);
	out.open ("data_putDirect.out"); 
	for( int i=0; i<N; i++)
		out << (i+1) << " " << eu.avg[i] << " " << eu.inc[i] << endl;
	out.close();

	//Call discretized (100 steps)
	cout << "Call discretized (100 steps)..." << endl;
	eu.call(100);
	out.open ("data_callDiscretized.out"); 
	for( int i=0; i<N; i++)
		out << (i+1) << " " << eu.avg[i] << " " << eu.inc[i] << endl;
	out.close();

	//Put discretized (100 steps)
	cout << "Put discretized (100 steps)..." << endl;
	eu.put(100);
	out.open ("data_putDiscretized.out"); 
	for( int i=0; i<N; i++)
		out << (i+1) << " " << eu.avg[i] << " " << eu.inc[i] << endl;
	out.close();



	return 0;
}



