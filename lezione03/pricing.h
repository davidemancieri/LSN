#include <vector>

using namespace std;

#ifndef __European__
#define __European__

class European {

private:
	int m_M;			//n of simulations
	int m_N;			//n of blocks
	double m_S;			//asset price
	double m_T;			//delivery time
	double m_K;			//strike price
	double m_r;			//risk-free interest rate
	double m_sigma;			//volatility
	

protected:

	
public:
	vector<double> avg;
	vector<double> inc;
	// constructors
	European(int, int, double, double, double, double, double);
	// destructor
	~European();
	// methods
	void call( int n_step );
	void put( int n_step );
	double err( vector<double> AV, vector<double> AV2, int n);
};

#endif // __European__
