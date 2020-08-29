#include "random.h"
#include "class.h"

#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

int main(){

	Random rnd;
	int n_cities = 32;
	int n_individuals = 300;
	int n_generations = 2000;
	vector<double> xC_cities, yC_cities;
	vector<double> xS_cities, yS_cities;

	cout << "The traveling salesman problem" <<endl <<endl;
	cout << "Number of cities to visit: " << n_cities <<endl;
	cout << "Number of generations: " << n_generations <<endl;
	cout << "Number of individuals in each generation: " << n_individuals << endl <<endl <<endl;

	//Generate cities on a circle of radius 1
	cout << "Arranging the cities on a circle of radius 1..." << endl;
	for(int i=0; i<n_cities; i++){
		double theta = rnd.Rannyu(0, 2*M_PI);
		xC_cities.push_back( cos(theta) );
		yC_cities.push_back( sin(theta) );
	}

	//Generate cities inside a square of side 2
	cout << "Arranging the cities in a square of side 2..." << endl;
	for(int i=0; i<n_cities; i++){
		xS_cities.push_back( rnd.Rannyu(-1, 1) );
		yS_cities.push_back( rnd.Rannyu(-1, 1) );
	}
	
	cout << endl << endl;

	cout <<"Circle - Generating the first generation..." <<endl;
	Generation circle( n_individuals, xC_cities, yC_cities, rnd );
	
	//Evolution
	cout <<"Let's begin the evolution process" <<endl;
	for( int i=1; i<=n_generations; i++){
		circle.stats( "circle_fitness.dat" );
		circle.new_gen();
		if(i%100==0) cout<<"Generation "<<i<<endl;
	}

	cout << "Done. Printing results..." <<endl;

	circle.print_best_path( "circle_bestpath.dat" );

	cout << endl << endl;

	cout <<"Square - Generating the first generation..." <<endl;
	Generation square( n_individuals, xS_cities, yS_cities, rnd );

	//Evolution
	cout <<"Let's begin the evolution process" <<endl;
	for( int i=1; i<=n_generations; i++){
		square.stats( "square_fitness.dat" );
		square.new_gen();
		if(i%100==0) cout<<"Generation "<<i<<endl;
	}
	
	cout << "Done. Printing results..." <<endl;
	square.print_best_path( "square_bestpath.dat" );


	return 0;
}
