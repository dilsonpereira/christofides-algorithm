#include "./Matching/Graph.h"
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <math.h>
using namespace std;

class TSPLIB_parser
{
public:
	TSPLIB_parser(string): filename(filename) { Read(); };

	const Graph & GetGraph() { return G; };
	const vector<int> & GetCosts() { return cost; };
	const vector<double> & GetXCoordinates()
	{
		if(X.size() == 0)
			throw "Error: no coordinates available";
		return X;
	};
	const vector<double> & GetYCoordinates()
	{
		if(Y.size() == 0)
			throw "Error: no coordinates available";
		return Y;
	};

private:
	Graph G;
	vector<int>  cost;
	//X and Y coordinates
	vector<double> X, Y;

	string filename, name, type, EdgeWeightType, EdgeWeightFormat, EdgeDataType, NodeCoordType, DisplayDataType; 

	void Read();
};


