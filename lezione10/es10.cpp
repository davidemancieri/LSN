#include "random.h"
#include "class.h"

#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

int main(){

	Random rnd;
	int n_cities = 32;
	int n_steps = 600000;
	double initial_beta = 0.08;
	double cooling_parameter = 1.005;
	int cool_rate = 300;
	vector<double> xC_cities, yC_cities;
	vector<double> xS_cities, yS_cities;

	cout << "The traveling salesman problem" <<endl <<endl;
	cout << "Number of cities to visit: " << n_cities <<endl;
	cout << "Number of steps: " << n_steps <<endl;
	cout <<  endl <<endl <<endl;

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

	cout <<"Circle - Generating the first path..." <<endl;
	double beta = initial_beta;
	Generation circle( xC_cities, yC_cities, beta, rnd );
	//Evolution
	cout <<"Let's begin the process" <<endl;
	for( int i=1; i<=n_steps; i++){
		circle.stats( "circle_fitness.dat" );
		circle.new_gen();
		if(i%10000==0) cout<<"Step "<<i<<endl;
		if(i%cool_rate==0){
			beta *= cooling_parameter;
			cout <<"Temperature: " << 1./beta << endl;
			circle.setBeta( beta );
		}
	}

	cout << "Done. Printing results..." <<endl<<endl<<endl;

	circle.print_best_path( "circle_bestpath.dat" );

	cout <<"Square - Generating the first path..." <<endl;
	beta = initial_beta;
	Generation square( xS_cities, yS_cities, beta, rnd );

	//Evolution
	cout <<"Let's begin the process" <<endl;
	for( int i=1; i<=n_steps; i++){
		square.stats( "square_fitness.dat" );
		square.new_gen();
		if(i%10000==0) cout<<"Step "<<i<<endl;
		if(i%cool_rate==0){
			beta *= cooling_parameter;
			cout <<"Temperature: " << 1./beta << endl;
			square.setBeta( beta );
		}
	}
	
	cout << "Done. Printing results..." <<endl;
	square.print_best_path( "square_bestpath.dat" );


	return 0;
}
