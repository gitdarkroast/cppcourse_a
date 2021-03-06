/*
Minimum Spanning Tree assignment.
- using the Prim algorithm
- used some code from the Dijkstra's assignment.


*/
#include "stdafx.h"

#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <cassert>


// Choosen to submit one file, so here are the
// definitions that would go in a header file.
/**** MST.H ****/

// Though not part of the assignment, the following class implements a Timer,
// allowing you to check the runtime execution of the implementation.
// Credit: http://www.learncpp.com/cpp-tutorial/8-16-timing-your-code/
class Timer
{
private:
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1>>;

	std::chrono::time_point<clock_t> m_beg;

public:
	Timer() : m_beg(clock_t::now())
	{}

	void reset()
	{
		m_beg = clock_t::now();
	}

	double elapsed() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}
};

// Graph ADT definition
// Use a matrix representation for the graph.
// For a graph of size s, the matrix will be of
// dimention s x s.  The element (i,j) will have 
// a non-zero edge cost value.  If the edge cost value 
// is 0 then it implies that there is no edge between
// vertex i and vertex j.
class Graph
{
private:
	std::vector<std::vector<int>> m_matrix;
	int m_size;

public:
	Graph() {};
	int vertices() const { return m_size; };
	std::vector<int> neighbors(int v);
	int cost(int s, int d) const { return m_matrix[s][d]; };
	bool populate(std::string fname);

	friend std::ostream& operator<<(std::ostream& out, const Graph& g);
};

// MST ADT
// Manage the minimum spanning tree
// We need to keep track of the vertices that are part of the MST
// We need to keep track of the cost  of the vertices visited
// Also need to keep track of the spanning tree.

class MST
{
private:
	Graph m_graph;
	int m_mstcost;
	std::vector<bool> m_visited;
	std::vector<int> m_cost;
	std::vector<int> m_mst;

public:
	MST(Graph g) : m_graph(g), m_mstcost(0) {
		m_visited.resize(m_graph.vertices());
		m_cost.resize(m_graph.vertices());
		m_mst.resize(m_graph.vertices());
	};
	int minimum();
	void prim();
	int cost();

	friend std::ostream& operator<<(std::ostream& out, const MST& mst);
};
/**** MST.H ****/

// Graph methods

// Read the tree data from the provided file and
// populate the matrix.
// The data file is of the following format:
// First line consists of a number repesenting the graph size
// Rest of the file is an edge definition (i, j, c); each on a separate
// line.
// e.g.:
// size
// i j c
// ...
bool Graph::populate(std::string fname)
{
	bool bResult{ true };
	std::ifstream infile;

	infile.open(fname);
	if (!infile)
	{
		// unable to open the given file.
		return false;
	}

	// Lets read the first line - size of graph
	std::string strSize;
	std::getline(infile, strSize);
	try
	{
		m_size = std::stoi(strSize);
		// Initialise the graph matrix
		m_matrix.resize(m_size);
		for (int i = 0; i < m_size; ++i)
		{
			m_matrix[i].resize(m_size);
		}
	}
	catch (std::invalid_argument)
	{
		assert(true && "Unable to convert graph size to int.");
	}


	std::string delimiter = " ";
	while (infile)
	{
		std::string strEdge;
		std::getline(infile, strEdge);

		// Assume format: "i j c"
		// Extract "i"
		std::string strSrc = strEdge.substr(0, strEdge.find(delimiter));
		// Remove "i " - note also removing the space, left with "j c"
		strEdge.erase(0, strEdge.find(delimiter) + delimiter.length());
		// Extract j
		std::string strDst = strEdge.substr(0, strEdge.find(delimiter));
		// Remove "j ", left with "c"
		strEdge.erase(0, strEdge.find(delimiter) + delimiter.length());
		// Extract "c"
		std::string strCost = strEdge.substr(0, strEdge.find(delimiter));

		try
		{
			int src = std::stoi(strSrc);
			int dst = std::stoi(strDst);
			int cost = std::stoi(strCost);

			bool valid = (src < m_size) && (src >= 0);
			valid = valid && (dst < m_size) && (dst >= 0);

			assert(valid && "Vertex in data file is out range.");

			m_matrix[src][dst] = cost;
		}
		catch (std::invalid_argument)
		{
			assert(true && "Error in converting edge data to int.");
		}

	}
	infile.close();
	return bResult;
}

// Return a container of vertex Ids.
std::vector<int> Graph::neighbors(int v)
{
	// Declare an empty vector
	std::vector<int> neighbor;
	for (int i = 0; i < m_matrix.size(); ++i)
	{
		if (m_matrix[v][i])
		{
			neighbor.push_back(i);
		}
	}
	return neighbor;
}

// Print a Graph
std::ostream& operator<<(std::ostream& out, const Graph& g)
{
	for (int i = 0; i < g.m_size; ++i)
	{
		for (int j = 0; j < g.m_size; ++j)
		{
			out << g.m_matrix[i][j] << " ";
		}
		out << std::endl;
	}
	return out;
}

// MST methods

// Find the vertex that has not been visisted AND has the lowest cost associated 
// with it.
int MST::minimum()
{
	int min = INT_MAX;
	int v_id{ 0 };

	for (int i = 0; i < m_graph.vertices(); ++i)
	{
		if (m_visited[i] == false && m_cost[i] < min)
		{
			min = m_cost[i];
			v_id = i;
		}
	}
	return v_id;
}

void MST::prim()
{
	int size = m_graph.vertices();
	// Initialise our containers - all costs are
	// infinite and we have not visisted any vertex.
	for (int i = 0; i < size; ++i)
	{
		m_cost[i] = INT_MAX;
		m_visited[i] = false;
	}

	// Establish the starting point.
	m_cost[0] = 0;
	m_mst[0] = 0;

	for (int i = 0; i < size; ++i)
	{
		// Get the lowest of the unvisited vertex
		int m = minimum();

		m_visited[m] = true;

		std::vector<int> neighbors = m_graph.neighbors(m);

		for(auto j: neighbors)
		{
			// If we have not visisted the adjacent vertex
			// and he edge cost is less than the current
			// update the MST and cost
			if (m_visited[j] == false && m_graph.cost(m, j) < m_cost[j])
			{
				m_mst[j] = m;
				m_cost[j] = m_graph.cost(m, j);
			}
		}

	}
}

// Calculate the total cost of the MST
int MST::cost()
{
	for (int i = 1; i < m_graph.vertices(); ++i)
	{
		m_mstcost += m_graph.cost(i, m_mst[i]);
	}
	return m_mstcost;
}

std::ostream& operator<<(std::ostream& out, const MST& mst)
{
	for (int i = 1; i < mst.m_graph.vertices(); ++i)
	{
		out << mst.m_mst[i] << " " << i << " " << mst.m_graph.cost(i, mst.m_mst[i]) << std::endl;
	}
	return out;
}

int main()
{
	Timer t;

	Graph g;

	// Construct the graph from the data file
	// For the purpose of this assignment we will
	// use a hardcoded location and filename.
	std::string sampledata = "C:\\SampleTestData_mst_data.txt";
	if (g.populate(sampledata))
	{
		// Dump the matrix representation of the graph
		std::cout << g;
		// Intialize the minimum spanning tree ADT
		MST mst{ g };
		// Apply the PRIM algorithm.
		mst.prim();
		// Output the MST, with the cost
		std::cout << mst;
		std::cout << "MST cost: " << mst.cost() << std::endl;
	}

	std::cout << "Total processing time: " << t.elapsed() << std::endl;
    return 0;
}
/*
Matrix representation of the sample data:
0 17 2 9 24 28 29 14 28 13 23 10 15 23 15 18 11 4 27 5
17 0 9 3 0 14 1 0 27 20 16 24 29 6 0 15 20 1 11 9
2 9 0 21 21 29 13 19 16 1 11 4 12 0 26 5 25 12 5 24
9 3 21 0 11 0 22 22 12 16 0 22 1 12 0 14 15 23 27 28
24 0 21 11 0 25 0 1 1 5 0 24 29 9 4 2 5 0 10 10
28 14 29 0 25 0 1 17 22 7 20 7 22 16 11 22 2 23 1 20
29 1 13 22 0 1 0 0 18 7 0 4 18 11 14 5 24 5 13 0
14 0 19 22 1 17 0 0 27 7 2 5 0 29 16 25 8 19 26 23
28 27 16 12 1 22 18 27 0 3 3 26 9 25 16 7 4 23 7 0
13 20 1 16 5 7 7 7 3 0 0 23 3 3 28 24 12 20 25 25
23 16 11 0 0 20 0 2 3 0 0 0 27 13 25 2 3 4 4 15
10 24 4 22 24 7 4 5 26 23 0 0 1 0 1 20 20 22 19 28
15 29 12 1 29 22 18 0 9 3 27 1 0 23 6 9 28 1 6 13
23 6 0 12 9 16 11 29 25 3 13 0 23 0 5 19 18 4 16 12
15 0 26 0 4 11 14 16 16 28 25 1 6 5 0 6 27 15 1 28
18 15 5 14 2 22 5 25 7 24 2 20 9 19 6 0 23 21 28 2
11 20 25 15 5 2 24 8 4 12 3 20 28 18 27 23 0 9 11 12
4 1 12 23 0 23 5 19 23 20 4 22 1 4 15 21 9 0 20 9
27 11 5 27 10 1 13 26 7 25 4 19 6 16 1 28 11 20 0 11
5 9 24 28 10 20 0 23 0 25 15 28 13 12 28 2 12 9 11 0

Output of the spanning tree edges and cost - from the
above matrix
17 1 1
0 2 2
12 3 1
8 4 1
6 5 1
1 6 1
4 7 1
9 8 3
2 9 1
7 10 2
12 11 1
9 12 3
9 13 3
11 14 1
4 15 2
5 16 2
12 17 1
14 18 1
15 19 2
MST cost: 30
*/

