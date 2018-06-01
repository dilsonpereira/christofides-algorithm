#include "MST.h"
#include "./Matching/Matching.h"
#include "./Matching/Graph.h"

pair< vector<int>, double > Christofides(const Graph & G, const vector<double> & cost)
{
	//Solve minimum spanning tree problem
	pair< list<int>, double > p = Prim(G, cost);
	list<int> mst = p.first;

	//Build adjacency lists using edges in the tree
	vector< list<int> > A(G.GetNumVertices(), list<int>());
	for(list<int>::iterator it = mst.begin(); it != mst.end(); it++)
	{
		pair<int, int> p = G.GetEdge(*it);
		int u = p.first, v = p.second;

		A[u].push_back(v);
		A[v].push_back(u);
	}

	//Find vertices with odd degree
	vector<int> odd;
	for(int u = 0; u < G.GetNumVertices(); u++)
		if(A[u].size() % 2)
			odd.push_back(u);

	//Create a graph with the odd degree vertices
	Graph O(odd.size());
	vector<double> costO;
	for(int i = 0; i < odd.size(); i++)
	{
		for(int j = i+1; j < odd.size(); j++)
		{
			if(G.AdjMat()[odd[i]][odd[j]])
			{
				O.AddEdge(i, j);
				costO.push_back( cost[G.GetEdgeIndex(odd[i], odd[j])] );
			}
		}
	}

	//Find the minimum cost perfect matching of the graph of the odd degree vertices
	Matching M(O);
	p = M.SolveMinimumCostPerfectMatching(costO);
	list<int> matching = p.first;

	//Add the edges in the matching the the adjacency list
	for(list<int>::iterator it = matching.begin(); it != matching.end(); it++)
	{
		pair<int, int> p = G.GetEdge(*it);
		int u = odd[p.first], v = odd[p.second];

		A[u].push_back(v);
		A[v].push_back(u);
	}

	//Find an Eulerian path in the graph implied by A
	list<int> path;

	int u = 0;
	path.push_back(0);
	while(not A[u].empty())
	{
		int v = A[u].back();
		path.push_back(v);
		A[u].pop_back();
		u = v;
	}

	//Shortcut the path
	vector<bool> visited(G.GetNumVertices(), false);
	vector<int> solution;
	double obj = 0;

	u = 0;
	visited[u] = true;
	list<int>::iterator it = ++(path.begin());
	for(; it != path.end(); it++)
	{
		int v = *it;
		if(visited[v]) 
			continue;

		visited[v] = true;
		obj += cost[ G.GetEdgeIndex(u, v) ];
		solution.push_back( G.GetEdgeIndex(u, v) );
		u = v;
	}

	return pair< vector<int>, double >(solution, obj);
}
