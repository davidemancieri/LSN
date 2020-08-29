#include <vector>
#include <string>

#include "random.h"

using namespace std;

#ifndef __class_h__
#define __class_h__

class Individual{

private:
	int m_Ncities;			
	vector<double> m_citiesX;	
	vector<double> m_citiesY;	
	vector<int> m_path;
	double m_L1;

public:
	Individual( vector<double>, vector<double>, vector<int> );
	~Individual();
	
	double L1();
	void check();
	double getL1() {return m_L1;};
	vector<int> getPath() {return m_path;};

};
class Generation{
	
private:
	int m_Ncities;
	int m_Nindividuals;
	vector<double> m_citiesX;
	vector<double> m_citiesY;
	vector<Individual> m_individuals;
	Random m_rnd;

public:
	Generation( int, vector<double>, vector<double>, Random );
	~Generation();

	vector<int> random_path();
	void new_gen();
	void Sort();
	int selectParent();
	vector<Individual> crossover( int, int );
	Individual mutate ( Individual );
	vector<int> pair_permutation( vector<int> );
	vector<int> shift( vector<int> );
	vector<int> block_permutation( vector<int> );
	vector<int> inversion( vector<int> );
	void stats( string );
	void print_best_path( string );

};



#endif //__class_h__
