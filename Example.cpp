#include "MST.h"
#include "./Matching/Matching.h"
#include "./Matching/Graph.h"
#include "Christofides.h"
#include "TSPLIB_parser.h"
#include <iostream>
using namespace std;

int main(int argc, char * argv[])
{
	if(argc < 2)
	{
		cout << "File name should be passed as a parameter" << endl;
		cout << "Use option --p after file name to print the solution in graphviz format" << endl;
		return 1;
	}

	string filename = argv[1];

	TSPLIB_parser T(filename);
	vector<double> cost(T.GetCosts().begin(), T.GetCosts().end());

	pair< vector<int> , double > p = Christofides(T.GetGraph(), cost);

	cout << "Solution cost: " << p.second;

	if(argc == 3 and string(argv[2]) == "--p")
	{
		//PrintGraphViz();
	}

	return 0;
}


