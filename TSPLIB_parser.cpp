#include "TSPLIB_parser.h"

void TSPLIB_parser::Read()
{
	ifstream file;
	file.open(filename.c_str());

	if(!file.is_open())
		throw "Error: could not open instance file";

	int n = -1;//number of vertices

	while(!file.eof())
	{
		string line;
		getline(file, line);
		stringstream stream(line);
		
		string keyword;

		getline(stream, keyword, ':');
		size_t pos = keyword.find(" ");
		while(pos != string::npos)
		{
			keyword.erase(pos);
			pos = keyword.find(" ");
		}

		if(keyword == "NAME")
			stream >> name;
		else if(keyword == "TYPE")
			stream >> type;
		else if(keyword == "DIMENSION")
		{
			stream >> n;

			//Create a complete graph with n vertices
			G = Graph(n);
			for(int i = 0; i < n; i++)
				for(int j= i+1; j < n; j++)
					G.AddEdge(i, j);
			cost.resize(n*(n-1)/2);
		}
		else if(keyword == "EDGE_WEIGHT_TYPE")
			stream >> EdgeWeightType;
		else if(keyword == "EDGE_WEIGHT_FORMAT")
			stream >> EdgeWeightFormat;
		else if(keyword == "EDGE_DATA_TYPE")
			stream >> EdgeDataType;
		else if(keyword == "NODE_COORD_TYPE")
			stream >> NodeCoordType;
		else if(keyword == "DISPLAY_DATA_TYPE")
			stream >> DisplayDataType;
		else if(keyword == "NODE_COORD_SECTION")
		{
			if(NodeCoordType == "TWOD_COORDS" ||
				EdgeWeightType == "EUC_2D" ||
				EdgeWeightType == "MAX_2D" ||
				EdgeWeightType == "CEIL_2D" ||
				EdgeWeightType == "ATT" ||
				EdgeWeightType == "GEO")
			{
				if(n == -1)
					throw "Error: there was a problem while reading the input instance";

				X.resize(n);
				Y.resize(n);
				for(int i = 0; i < n; i++)
				{
					int id;
					file >> id >> X[i] >> Y[i];
					if(EdgeWeightType == "GEO")
					{
						double PI = 3.141592;
						double deg = round(X[i]);
						double min = X[i] - deg;
						X[i] = PI * ( deg + 5.0 * min / 3.0 ) / 180.0;
						deg = round(Y[i]);
						min = Y[i] - deg;
						Y[i] = PI * ( deg + 5.0 * min / 3.0 ) / 180.0;
					}
				}
				for(int i = 0; i < n; i++)
				{
					for(int j = i+1; j < n; j++)
					{
						if(EdgeWeightType == "EUC_2D")
						{
							double xd = X[i] - X[j];
							double yd = Y[i] - Y[j];
							cost[G.GetEdgeIndex(i, j)] = round( sqrt( xd*xd + yd*yd ) );
						}
						else if(EdgeWeightType == "MAX_2D")
						{
							double xd = fabs( X[i] - X[j] );
							double yd = fabs( Y[i] - Y[j] );
							cost[G.GetEdgeIndex(i, j)] = max( round(xd), round(yd) );
						}
						else if(EdgeWeightType == "GEO")
						{
							double RRR = 6378.388;
							double q1 = cos( Y[i] - Y[j] );
							double q2 = cos( X[i] - X[j] );
							double q3 = cos( X[i] + X[j] );

							cost[G.GetEdgeIndex(i, j)] = (int)( RRR * acos( 0.5 * ( ( 1.0 + q1 ) * q2 - (1.0 - q1) * q3 ) ) + 1.0 );
						}
						else if(EdgeWeightType == "ATT")
						{
							double xd = X[i] - X[j];
							double yd = Y[i] - Y[j];
							double rij = sqrt( ( xd * xd + yd * yd ) / 10.0);
							double tij = round( rij );

							if(tij < rij ) 
								cost[G.GetEdgeIndex(i, j)] = tij + 1.0;
							else 
								cost[G.GetEdgeIndex(i, j)] = tij;
						}
						else if(EdgeWeightType == "CEIL_2D")
						{
							double xd = X[i] - X[j];
							double yd = Y[i] - Y[j];

							cost[G.GetEdgeIndex(i, j)] = (int)( sqrt( xd*xd + yd*yd ) + 1.0);
						}
					}
				}
			}
			else
			{
				throw "Error: instance format not supported";
			}
		}
		else if(keyword == "EDGE_WEIGHT_SECTION")
		{
			if(EdgeWeightFormat == "FULL_MATRIX")
			{
				for(int i = 0; i < n; i++)
					for(int j = 0; j < n; j++)
						file >> cost[G.GetEdgeIndex(i, j)];
			}
			else if(EdgeWeightFormat == "UPPER_ROW")
			{
				for(int i = 0; i < n; i++)
					for(int j = i +1; j < n; j++)
						file >> cost[G.GetEdgeIndex(i, j)];
			}
			else if(EdgeWeightFormat == "LOWER_ROW")
			{
				for(int i = 0; i < n; i++)
					for(int j = 0; j < i; j++)
						file >> cost[G.GetEdgeIndex(i, j)];
			}
			else if(EdgeWeightFormat == "UPPER_DIAG_ROW")
			{
				for(int i = 0; i < n; i++)
				{
					int t;
					file >> t;
					for(int j = i+1; j < n; j++)
						file >> cost[G.GetEdgeIndex(i, j)];
				}
			}
			else if(EdgeWeightFormat == "LOWER_DIAG_ROW")
			{
				for(int i = 0; i < n; i++)
				{
					for(int j = 0; j < i; j++)
						file >> cost[G.GetEdgeIndex(i, j)];
					int t;
					file >> t;
				}
			}
			else if( (EdgeWeightFormat == "UPPER_COL") or 
			(EdgeWeightFormat == "LOWER_COL") or 
			(EdgeWeightFormat == "UPPER_DIAG_COL") or 
			(EdgeWeightFormat == "LOWER_DIAG_COL") )
			{
				throw "Error: instance format not supported";
			}
		}
		else if(keyword == "DISPLAY_DATA_SECTION")
		{
			if(DisplayDataType != "TWOD_COORDS") continue;

			X.resize(n);
			Y.resize(n);
			for(int i = 0; i < n; i++)
			{
				int id;
				file >> id >> X[i] >> Y[i];
			}
		}
	}
	
	file.close();
}

