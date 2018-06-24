#pragma once

#include "./Matching/Graph.h"
#include "./Matching/BinaryHeap.h"
#include "./Matching/Globals.h"

//Prim's algorithm using binary heap
pair< list<int>, double > Prim(const Graph & G, const vector<double> & cost)
{
	BinaryHeap B;

	vector<int> father(G.GetNumVertices(), -1);
	list<int> mst;

	double obj = 0;
	
	//Put 0 in the heap
	B.Insert(0, 0);

	while(B.Size() > 0)
	{
		//Select the vertex that is closest to the growing tree
		int u = B.DeleteMin();
		int w = father[u];

		//Add {w,u} to the tree
		if(w != -1)
		{
			int i = G.GetEdgeIndex(w, u);
			mst.push_back(i);
			obj += cost[i];
		}

		//This is to indicate that u is already in the tree
		father[u] = -2;

		//Update the heap with vertices adjacent to u
		for(list<int>::const_iterator it = G.AdjList(u).begin(); it != G.AdjList(u).end(); it++)
		{
			int v = *it;
			
			//if v is already in the tree
			if(father[v] == -2)
				continue;

			double c = cost[G.GetEdgeIndex(u,v)];

			//v has not been reached by anyone else
			if(father[v] == -1)
			{
				father[v] = u;	
				B.Insert(c, v);
			}
			//we found a cheaper connection to v
			else if( LESS(c, cost[G.GetEdgeIndex(father[v], v)]) )
			{
				father[v] = u;
				B.ChangeKey(c, v);
			}
		}
	}

	if((int)mst.size() < G.GetNumVertices()-1)
		throw "Error: graph does not have a spanning tree";

	return pair< list<int>, double >(mst, obj);
}



