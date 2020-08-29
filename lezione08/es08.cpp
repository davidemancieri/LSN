#include <iostream>
#include "random.h"
#include "QMparticle.h"

using namespace std;

int main (){

	int M=1E6;
	int N=100;
   	QMparticle P(M, N, 0.5);		//M, N, initialX
	P.setDelta( 2.65 );
	
	//P.Optimization(0.75, 0.85, 0.55, 0.65, 0.005); //startMu, endMu, startSigma, endSigma, var

	P.Metropolis( 0.805, 0.61 );
	P.writeBlocks( "energy.dat" );
	P.writeInstantPoints( "points.dat", M );

	return 0;

}
