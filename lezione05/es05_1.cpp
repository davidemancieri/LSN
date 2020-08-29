#include <iostream>
#include "random.h"
#include "hydrogen.h"

using namespace std;
 
int main (){
	
	Hydrogen H(1E6, 100, 0., 0., 0.);		//M, N, initialX, initialY, initialZ

//deltas chosen to get a 50% acceptance rate from Metropolis
	int test = 1E3;

//quantum state 1,0,0 and uniform distribution (0, u)
	H.setDelta( 1.25 );
	H.equilibrate( test, 0, 'u');
	//cout << "Acceptance rate: " << H.getAcc() / (double)test << endl;
	//H.writeInstantValues( "EQvalues100.out", test );
	//H.writeInstantPoints( "EQpoints100.out", test );
	H.Metropolis( 0, 'u' );
	H.writeBlocks( "mean100.out" );
	H.writePoints( "points100.out", 100 );

//quantum state 1,0,0 and gaussian distribution (0, g)
	H.setDelta( 0.75 );
	H.equilibrate( test, 0, 'g');
	//cout << "Acceptance rate: " << H.getAcc() / (double)test << endl;
	//H.writeInstantValues( "EQvalues100Gauss.out", test );
	//H.writeInstantPoints( "EQpoints100Gauss.out", test );
	H.Metropolis( 0, 'g' );
	H.writeBlocks( "mean100Gauss.out" );
	H.writePoints( "points100Gauss.out", 100 );	

//quantum state 2,1,0 and uniform distribution (0, g)
	H.setX(1.);
	H.setY(1.);
	H.setZ(1.);
	H.setDelta( 3 );
	H.equilibrate( test, 1, 'u');
	//cout << "Acceptance rate: " << H.getAcc() / (double)test << endl;
	//H.writeInstantValues( "EQvalues210.out", test );
	//H.writeInstantPoints( "EQpoints210.out", test );
	H.Metropolis( 1, 'u' );
	H.writeBlocks( "mean210.out" );
	H.writePoints( "points210.out", 100 );
	
//quantum state 2,1,0 and gaussian distribution (1, g)
	H.setDelta( 1.85 );
	H.equilibrate( test, 1, 'g');
	//cout << "Acceptance rate: " << H.getAcc() / (double)test << endl;
	//H.writeInstantValues( "EQvalues210Gauss.out", test );
	//H.writeInstantPoints( "EQpoints210Gauss.out", test );
	H.Metropolis( 1, 'g' );
	H.writeBlocks( "mean210Gauss.out" );
	H.writePoints( "points210Gauss.out", 100 );
	
	
	return 0;
}
