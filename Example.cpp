#include "MST.h"
#include "./Matching/Matching.h"
#include "./Matching/Graph.h"
#include "Christofides.h"
#include "TSPLIB_parser.h"
#include <fstream>
#include <iostream>
using namespace std;

void PrintNeato(const TSPLIB_parser & T, const vector<int> & s, const string & filename)
{
	try
	{
		const vector<double> & X = T.GetXCoordinates();
		const vector<double> & Y = T.GetYCoordinates();

		ofstream F(filename);

		F << "graph G{" << endl;
		for(int i = 0; i < X.size(); i++)
			F << "n" << i << "[pos = \"" << X[i]/100.0 << "," << Y[i]/100.0 << "!\", height=0.1, width=0.1, label=\"\"];" << endl;

		for(int i = 0; i < s.size(); i++)
			F << "n" << T.GetGraph().GetEdge(s[i]).first << "--n" << T.GetGraph().GetEdge(s[i]).second << "[color = red];" << endl;

		F << "}" << endl;

		F.close();
	}
	catch(const char * msg)
	{
		cout << msg << endl;
		exit(1);
	}
}

int main(int argc, char * argv[])
{
	string filename = "";
	string fileneato = "";
	bool printSequence = false;

	int i = 1;
	while(i < argc)
	{
		string a(argv[i]);
		if(a == "-f")
			filename = argv[++i];
		else if(a == "-p")
			printSequence = true;
		else if(a == "-g")
			fileneato = argv[++i];
		i++;
	}

	if(filename == "")
	{
		cout << "usage: ./example -f <filename> [-p] [-g <filename2> ]" << endl;
		cout << "-p to print edges in the solution" << endl;
		cout << "-g to print the solution in graphviz neato format" << endl;
		return 1;
	}

	try
	{
		//Read the TSPLIB file
		TSPLIB_parser T(filename);

		//TSPLIB costs are integers, convert them to double
		vector<double> cost(T.GetCosts().begin(), T.GetCosts().end());

		pair< vector<int> , double > p = Christofides(T.GetGraph(), cost);

		cout << "Solution cost: " << p.second << endl;

		vector<int> s = p.first;

		if(printSequence)
		{
			cout << "Edges:" << endl;
			for(int i = 0; i < s.size(); i++)
				cout << T.GetGraph().GetEdge(s[i]).first << " " << T.GetGraph().GetEdge(s[i]).second << endl;
			cout << endl;
		}
		if(fileneato != "")
			PrintNeato(T, s, fileneato);
	}
	catch(const char * msg)
	{
		cout << msg << endl;
		return 1;
	}

	return 0;
}


