#include "MST.h"
#include "./Matching/Matching.h"
#include "./Matching/Graph.h"
#include "Christofides.h"
#include "TSPLIB_parser.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>
using namespace std;

void PrintNeato(const Graph & G, const vector<double> & X, const vector<double> & Y, const vector<int> & s, const string & filename, double scale = 1.0)
{
	ofstream F(filename.c_str());

	F << "graph G{" << endl;
	for(int i = 0; i < (int)X.size(); i++)
		F << "n" << i << "[pos = \"" << X[i]/scale << "," << Y[i]/scale << "!\", height=0.1, width=0.1, label=\"\"];" << endl;

	for(int i = 0; i < (int)s.size(); i++)
		F << "n" << G.GetEdge(s[i]).first << "--n" << G.GetEdge(s[i]).second << "[color = red];" << endl;

	F << "}" << endl;

	F.close();
}

void ReadEuclideanGraph(string filename, Graph & G, vector<double> & X, vector<double> & Y, vector<double> & cost)
{
	ifstream file;
	file.open(filename.c_str());

	string s;
	getline(file, s);
	stringstream ss(s);
	int n;
	ss >> n;

    G = Graph(n);
    X.clear(); 
    Y.clear();
    cost.clear();
	for(int i = 0; i < n; i++)
	{
		getline(file, s);
		ss.str(s);
		ss.clear();
		double x, y;
		ss >> x >> y;

        X.push_back(x);
        Y.push_back(y);
	}
	
	for(int i = 0; i < n; i++)
	    for(int j = i+1; j < n; j++)
	        G.AddEdge(i, j);
	        
	for(int i = 0; i < G.GetNumEdges(); i++)
	{
	    pair<int, int> p = G.GetEdge(i);
	    int u = p.first, v = p.second;
	    cost.push_back( sqrt( pow(X[u]-X[v], 2) + pow(Y[u]-Y[v], 2) ) );
	}

	file.close();
}

int main(int argc, char * argv[])
{
    string fileformat = "TSPLIB";
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
		else if(a == "--coord")
			fileformat = "coord";
		i++;
	}

	if(filename == "")
	{
		cout << endl << "usage: ./example -f <filename> [--coord] [-p] [-g <filename2>]" << endl << endl;
		cout << "-f followed by file name to specify the input file." << endl << endl;
		
		cout << "The input file will be assumed to be in a TSPLIB format unless option --coord is provided." << endl;
		cout << "In that case, the first line of the file should give the number of vertices n followed by n lines giving the X and Y coordinates of the corresponding vertices." << endl << endl;
	   	
		cout << "-p to print edges in the solution." << endl;
		cout << "-g to print the solution in graphviz neato format." << endl;
		return 1;
	}

	try
	{
	    Graph G;
	    vector<double> cost, X, Y;
	    if(fileformat == "TSPLIB")
	    {
	        try
	        {
		        //Read the TSPLIB file
		        TSPLIB_parser T(filename);
		        G = T.GetGraph();

		        //TSPLIB costs are integers, convert them to double
		        cost.assign(T.GetCosts().begin(), T.GetCosts().end());
		        
		        X = T.GetXCoordinates();
		        Y = T.GetYCoordinates();
            }
            catch(const char * msg)
            {
                if(string(msg) == "Error: no coordinates available")
                {
                    X.clear();
                    Y.clear();
                }
                else
                {
                    throw msg;
                }
            }
		}
		else
		{
		    ReadEuclideanGraph(filename, G, X, Y, cost);
		}

		pair< vector<int> , double > p = Christofides(G, cost);

		cout << "Solution cost: " << p.second << endl;

		vector<int> s = p.first;

		if(printSequence)
		{
			cout << "Edges:" << endl;
			for(int i = 0; i < (int)s.size(); i++)
				cout << G.GetEdge(s[i]).first << " " << G.GetEdge(s[i]).second << endl;
			cout << endl;
		}
		if(fileneato != "")
		{
		    if(X.size() == 0)
			{
		        cout << "Unable to print GraphViz output. Coordinates not available" << endl;
			}
		    else
			{
				double scale = 1.0;
				if(fileformat=="TSPLIB")
					scale = 10.0;
    			PrintNeato(G, X, Y, s, fileneato, scale);
			}
		}
	}
	catch(const char * msg)
	{
		cout << msg << endl;
		return 1;
	}

	return 0;
}


