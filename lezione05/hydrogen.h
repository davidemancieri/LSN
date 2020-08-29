#include <vector>
#include <string>

using namespace std;

#ifndef __Hydrogen__
#define __Hydrogen__

class Hydrogen {

private:
	int M;			//n of simulations
	int N;			//n of blocks
	int L;			//M/L
	double delta;
	vector<double> x, y, z;	//positions
	vector<double> ave, av2, sum_prog, su2_prog, err_prog;
	int acc;		//keeps count for acceptance rate

protected:

	
public:
	// constructors
	Hydrogen(int, int, double, double, double);
	// destructor
	~Hydrogen();
	// methods
	void setDelta( double d ) {delta = d ;};
	void setX( double X ) {x[0] = X ;};
	void setY( double Y ) {y[0] = Y ;};
	void setZ( double Z ) {z[0] = Z ;};
	int getAcc() {return acc;};
	void equilibrate(int, int, char);
	void Metropolis(int, char);
	double Prob0( double, double, double);
	double Prob1( double, double, double);
	double err( vector<double>, vector<double>, int);
	void writeBlocks( string );
	void writePoints( string, int );
	void writeInstantValues( string, int );
	void writeInstantPoints( string, int );
};

#endif // __Hydrogen__
