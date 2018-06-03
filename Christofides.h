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
		pair<int, int> p = O.GetEdge(*it);
		int u = odd[p.first], v = odd[p.second];

		A[u].push_back(v);
		A[v].push_back(u);
	}

	//Find an Eulerian cycle in the graph implied by A
	list<int> cycle;
	//This is to keep track of how many times we can traverse an edge
	vector<int> traversed(G.GetNumEdges(), 0);
	for(int u = 0; u < G.GetNumVertices(); u++)
	{
		for(list<int>::iterator it = A[u].begin(); it != A[u].end(); it++)
		{
			int v = *it;
			
			//we do this so that the edge is not counted twice
			if(v < u) continue;

			traversed[G.GetEdgeIndex(u, v)]++;
		}
	}

	cycle.push_back(0);
	list<int>::iterator itp = cycle.begin();
	while(itp != cycle.end())
	{
		//Let u be the current vertex in the cycle, starting at the first
		int u = *itp;
		list<int>::iterator jtp = itp;
		jtp++;

		//if there are non-traversed edges incident to u, find a subcycle starting at u
		//replace u in the cycle by the subcycle
		while(not A[u].empty())
		{
			while(not A[u].empty() and traversed[ G.GetEdgeIndex(u, A[u].back()) ] == 0)
				A[u].pop_back();

			if(not A[u].empty())
			{
				int v = A[u].back();
				A[u].pop_back();
				cycle.insert(jtp, v);
				traversed[G.GetEdgeIndex(u, v)]--;
				u = v;
			}
		}

		//go to the next vertex in the cycle and do the same
		itp++;
	}

	//Shortcut the cycle
	vector<bool> visited(G.GetNumVertices(), false);
	vector<int> solution;
	double obj = 0;

	int u = 0;
	visited[u] = true;
	list<int>::iterator it = ++(cycle.begin());
	for(; it != cycle.end(); it++)
	{
		int v = *it;
		if(visited[v]) 
			continue;

		visited[v] = true;
		obj += cost[ G.GetEdgeIndex(u, v) ];
		solution.push_back( G.GetEdgeIndex(u, v) );
		u = v;
	}
	obj += cost[ G.GetEdgeIndex(u, 0) ];
	solution.push_back( G.GetEdgeIndex(u, 0) );

	return pair< vector<int>, double >(solution, obj);
}
