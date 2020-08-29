#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>

#include "class.h"
#include "random.h"

using namespace std;

Individual :: Individual( vector<double> x_cities, vector<double> y_cities, vector<int> path ){
	
	m_Ncities = x_cities.size();
	m_citiesX = x_cities;
	m_citiesY = y_cities;
	m_path = path;
	m_L1 = L1();
	
}

Individual :: ~Individual(){

}

double Individual :: L1(){

	double L1 = 0.;
	double dx = 0.;
	double dy = 0.;
	for( int i=0; i<m_Ncities-1; i++ ){
		dx = m_citiesX[m_path[i]] - m_citiesX[m_path[i+1]];
		dy = m_citiesY[m_path[i]] - m_citiesY[m_path[i+1]];
		L1 += sqrt( dx*dx + dy*dy );
	}
	dx = m_citiesX[m_path[m_Ncities-1]] - m_citiesX[m_path[0]];	//adding the distance from the last city to the first one
	dy = m_citiesY[m_path[m_Ncities-1]] - m_citiesY[m_path[0]];	//because it is not computed in the for cicle
	L1 += sqrt( dx*dx + dy*dy );

	return L1;

}

void Individual :: check(){

	if( m_path[0]!=0 ){
		cerr << "The first city is not the first to be visited!" << endl;
		exit(1);
	}

	for( int i=0; i<m_Ncities; i++){
		for( int j=i+1; j<m_Ncities; j++){
			if( m_path[i] == m_path[j] ){
				cerr << "A city is visited more than once!" << endl;
				exit(1);
			}
		}
	}

}







Generation :: Generation( vector<double> x_cities, vector<double> y_cities, double initial_beta, Random rnd ){

	m_Ncities = x_cities.size();
	m_citiesX = x_cities;
	m_citiesY = y_cities;
	m_rnd = rnd;
	m_beta = initial_beta;

	vector<int> rnd_path = random_path();				//Generating a random individual
	Individual new_i( m_citiesX, m_citiesY, rnd_path );		//
	new_i.check();							//Checking that the individual is legal
	m_individual = new_i;

}

Generation :: ~Generation(){

}

vector<int> Generation :: random_path(){

	vector<int> path;
	for( int i=0; i<m_Ncities; i++){
		path.push_back(i);
	}
	for( int i=0; i<1E3; i++){
		path = pair_permutation( path );				//shuffling the path
	}

	return path;

}

void Generation :: new_gen(){

	Individual mutated = mutate( m_individual );
	double oldBolt = Boltzmann( m_individual.getL1() );
	double newBolt = Boltzmann( mutated.getL1() );
	double p = newBolt/oldBolt;
	double r = m_rnd.Rannyu();
	if( r < p ){
		m_individual = mutated;
	}

}


Individual Generation :: mutate( Individual original ){

	vector<int> mutated_path;					//doing all the mutations
	mutated_path = original.getPath();
	if( m_rnd.Rannyu() < 0.09 ){
		mutated_path = pair_permutation( mutated_path );
	}
	if( m_rnd.Rannyu() < 0.09 ){
		mutated_path = shift( mutated_path );
	}
	if( m_rnd.Rannyu() < 0.09 ){
		mutated_path = block_permutation( mutated_path );
	}
	if( m_rnd.Rannyu() < 0.09 ){
		mutated_path = inversion( mutated_path );
	}

	Individual mutated( m_citiesX, m_citiesY, mutated_path);
	mutated.check();						//checking after the mutations that the path is still legal

	return mutated;

}

vector<int> Generation :: pair_permutation( vector<int> path ){

	int pos1 = m_rnd.Rannyu( 1, m_Ncities-1 );		//the two pairs are chosen making sure that the first city isn't moved
	int pos2 = m_rnd.Rannyu( pos1+1, m_Ncities );		//and that they are different

	double appo = path[pos1];
	path[pos1] = path[pos2];
	path[pos2] = appo;

	return path;

}

vector<int> Generation :: shift( vector<int> path ){

	int pos1 = m_rnd.Rannyu( 1, m_Ncities-1 );		//we can't move the first city and moving the last one doesn't make sense
	int m = m_rnd.Rannyu( 1, m_Ncities-pos1);		//how many cities are moved together
	int n = m_rnd.Rannyu( 1, m_Ncities-pos1-m+1);		//how many steps are they moved

	vector<int> appo;
	for( int i=pos1; i<pos1+m; i++){
		appo.push_back( path[i] );			//first we store the m cities away
	}
	for( int i=0; i<n; i++){
		path[pos1+i] = path[pos1+i+m];			//then we move n steps of the path to the left
	}
	for( int i=0; i<m; i++){
		path[pos1+n+i] = appo[i];			//finally we add back the m cities on the right
	}

	return path;

}

vector<int> Generation :: block_permutation( vector<int> path ){

	int max=0;
	if( m_Ncities%2 == 0 ){					//even	
		max = (m_Ncities-2)/2;				//-1 because we can't move the first city, then it becomes odd so we do -1 again to have it even
	}
	else{							//odd
		max = (m_Ncities-1)/2;				//-1 because we can't move the first city
	}
	int m = m_rnd.Rannyu( 1, max+1 );			//m can't be higher than max because we need two different blocks of cities of lenght m to move
	int pos1 = m_rnd.Rannyu( 1, m_Ncities-(2*m)+1 );	//choosing the position of the first city of the first block to move
	int pos2 = m_rnd.Rannyu( pos1+m, m_Ncities-m+1 );	//position of the first city of the second one
	for( int i=0; i<m; i++){
		double appo = path[pos1+i];
		path[pos1+i] = path[pos2+i];
		path[pos2+i] = appo;
	}

	return path;

}

vector<int> Generation :: inversion( vector<int> path ){

	int pos1 = m_rnd.Rannyu( 1, m_Ncities-1 );		//pos1 is the start of the block of cities that we want to invert, pos2 the end
	int pos2 = m_rnd.Rannyu( pos1+1, m_Ncities );		//pos1 can't be the first city and it doesn't make sense for it to be the last one either
	int diff = pos2-pos1;
	int i=0;
	do{
		double appo = path[pos1+i];
		path[pos1+i] = path[pos2-i];
		path[pos2-i] = appo;
		i++;
	}
	while( i<diff-1);

	return path;

}

void Generation :: stats( string output ){

	ofstream fitnessL1;
	fitnessL1.open( output, ios::app );
	fitnessL1 << m_individual.getL1() << endl;	//printing the lenght of the best path and the averaged one
	fitnessL1.close();

}

void Generation :: print_best_path( string output ){

	ofstream bestPath;						//printing the best path
	bestPath.open( output );
	vector<int> path = m_individual.getPath();
	for( int i=0; i<m_Ncities; i++){
		bestPath << m_citiesX[path[i]] << "   " << m_citiesY[path[i]] << endl;
	}
	bestPath << m_citiesX[path[0]] << "   " << m_citiesY[path[0]] << endl; //adding again the first city to return to hometown at the end
	bestPath.close();

}

void Generation :: setBeta( double beta ){
	
	m_beta = beta;

}

double Generation :: Boltzmann( double L1 ){

	return exp( -m_beta * L1 );

}
