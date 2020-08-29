#include <vector>
#include <string>

using namespace std;

#ifndef __QMparticle__
#define __QMparticle__

class QMparticle {

private:
	int M;			//n of simulations
	int N;			//n of blocks
	int L;			//M/L
	double delta;
	vector<double> x;	//positions
	vector<double> ave, av2, sum_prog, su2_prog, err_prog;
	int acc;		//keeps count for acceptance rate

protected:


public:
	// constructors
  	QMparticle(int, int, double);
	// destructor
        ~QMparticle();
	// methods				
 	void setDelta( double d ) {delta = d ;};
	void setX( double X ) {x[0] = X ;};
	int GetAcc() { return acc; }
	double GetH() { return sum_prog[N-1]; };
	void Optimization( double, double, double, double, double );
	void Metropolis( double, double );
	double Psi(double, double, double);
	double d2Psi(double, double, double);
	double Potential(double);
	double err( vector<double>, vector<double>, int);
	void writeBlocks( string );
	void writeInstantPoints( string, int );
};	

#endif // __QMparticle__

