//Need the following as the first include for a Visual Sudio environment
#include "stdafx.h"

/*
Implementation of the Dijsktra's shortest path algorithm.
Good background article: https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
Another good source: https://brilliant.org/wiki/dijkstras-short-path-finder/

Sample output have been placed in the comments at the end of the code.  Used 
Matrix representation as it seemed easier to track edge and associated cost.  Initially 
tried to use Adjacency list, but, found representing vertex and edge relationships
quite cubmersome.

In the main(), was able to randomly generate graphs and determine shortest paths
between two vertices.  The iterations were driven using some constants.
*/


// Including the header file so as to submit only one file for the assignment
// Ideally would place class definitions, method definition, etc in a separate file
// but, not sure how to submit multiple files.

/****** DIJKSTRA.H ******/
#include <iostream>
#include <chrono>
#include <vector>
#include <random>

// Some helper classes

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

// A random generator to assist with random int and double values
// Credit: http://www.learncpp.com/cpp-tutorial/59-random-number-generation/
class Random
{
private:
	std::random_device m_rd;
	std::mt19937 m_mersenne;

public:
	Random() { 
		m_mersenne = std::mt19937(m_rd());
	};
	double getRandom(double start, double end)
	{
		std::uniform_real_distribution<double> dist{ start, end };

		double result = dist(m_mersenne);
		return result;
	}

	int getRandom(int start, int end)
	{
		std::uniform_int_distribution<int> dist{ start, end };

		int result = dist(m_mersenne);
		return result;
	}
};


// Graph ADT definition
// We are going to used a matrix representation for the graph.
// The matrix will be size v x v, where v is the number of vertices
// in the graph.  The value m_matrix[i][j] will be either a non-zero cost, 
// or zero which implies there is no edge between i and j.
class Graph
{
private:
	std::vector<std::vector<int>> m_matrix;
	int m_size;
	double m_density;

public:
	Graph(int size=10, double density=0.1)
		:m_size(size), m_density(density)
	{};
	double getDensity() { return m_density; };
	int vertices() { return m_size; };
	std::vector<int> neighbors(int v);
	int edgeCost(int s, int d) { return m_matrix[s][d]; };
	bool adjacent(int s, int d) { return m_matrix[s][d] > 0; };
	void generate();

	friend std::ostream& operator<<(std::ostream& out, const Graph& g);
};


// ShortestPath ADT
class ShortestPath
{
private:
	Graph m_graph;
	std::vector<bool> m_visited;
	std::vector<int> m_distance;
public:
	ShortestPath(Graph g) : m_graph(g)
	{
		m_visited.resize(m_graph.vertices());
		m_distance.resize(m_graph.vertices());
	}
	~ShortestPath() {};

	int vertices() { return m_graph.vertices(); };
	int minimum();
	bool path(int src, int dst);
	int pathCost(int n) { return m_distance[n]; };
	int avgCost();

};

/****** DIJKSTRA.H ******/

#include<cfloat>

// Constants to drive the monte carlo simulation
static const double minDensity = 0.2;
static const double maxDensity = 1.0;
static const int minGraphSize = 50;
static const int maxGraphSize = 100;
static const int minEdgeCost = 1;
static const int maxEdgeCost = 10;
static const int maxIterations = 5;


// Graph methods

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


// Generate a random graph based on the density.
void Graph::generate()
{
	Random r;
	// Initialize the vector to the number of vertices
	m_matrix.resize(m_size);

	for (int i = 0; i < m_size; ++i)
	{
		// Initialize the columns for row i
		m_matrix[i].resize(m_size);
		for (int j = 0; j < m_size; ++j)
		{
			// We do not support a loop
			if (i == j)
				continue;
			if (m_density > r.getRandom(0.0, 1.0))
			{
				// We have an edge between i and j
				int cost = r.getRandom(minEdgeCost, maxEdgeCost);
				m_matrix[i][j] = cost;
			}
			else
			{
				m_matrix[i][j] = 0;
			}
		}
	}

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


// ShortestPath methods

// Get the minimum cost vertex which has not beein visited
int ShortestPath::minimum()
{
	int min_value = INT_MAX;
	int min_index{ 0 };

	for (int i = 0; i < vertices(); ++i)
	{
		if ((m_visited[i] == false) &&
			(m_distance[i] < min_value))
		{
			min_value = m_distance[i];
			min_index = i;
		}
	}
	return min_index;
}

int ShortestPath::avgCost()
{
	int cost{ 0 };
	for (int i = 0; i < m_graph.vertices(); ++i)
	{
		cost += m_graph.edgeCost(0, i);
	}
	return cost / m_graph.vertices();
}

// Determine the miimum cost between src and dst:
// Mantain two containers:
//	- one contains the distances of the vertices visisted
//	- the other a list of unvisisted vertices - use bool to toggle
// The algorithm is:
// - while we still have vertices in unvisited state
//		- pick minimum cost vertex m which is not in the visited state
//		- update the shortest distance if:
//			- for all neighbors n of m:
//				dist(m) + cost(n,m) < dist(n)
bool ShortestPath::path(int src, int dst)
{
	bool bResult{ false };

	// First lets check if the starting point has any neighbors.
	// If not then no point carrying as there is no route.
	std::vector<int> neighbors = m_graph.neighbors(src);
	if (0 == neighbors.size())
	{
		return false;
	}

	int vertices = m_graph.vertices();
	for (int i = 0; i < vertices; ++i)
	{
		m_distance[i] = INT_MAX;
		m_visited[i] = false;
	}

	// Our start point has cost of zero
	m_distance[src] = 0;

	for (int i = 0; i < vertices; ++i)
	{
		// Get the min cost from the openset
		int m = minimum();

		m_visited[m] = true;

		// We get the neighbors of vertex m:
		std::vector<int> neighbors = m_graph.neighbors(m);

		// We go through the neighbors and see if the
		// the distance needs to be updated.
		for (auto& elem: neighbors)
		{
			bool inOpenSet = m_visited[elem];
			int edge_val = m_graph.edgeCost(m, elem);
			bool visited = m_distance[m] != INT_MAX;
			bool update = m_distance[m] + edge_val < m_distance[elem];

			if (!inOpenSet &&
				edge_val &&
				visited &&
				update)
			{
				m_distance[elem] = m_distance[m] + edge_val;
			}
		}

	}
	return m_distance[dst] != INT_MAX;
}


// Here is our simulator
// - randomly generate graphs
// - find the shortest path
// - aggregate the average
int main(int argc, char **argv)
{
	Timer t;
	Random r;
	int avg_cost{ 0 };

	for (int i = 0; i < maxIterations; ++i)
	{
		std::cout << "Iteration #: " << i << std::endl;
		// Randomly determine graph density and size.
		double density = r.getRandom(minDensity, maxDensity);
		int size = r.getRandom(minGraphSize, maxGraphSize);
		Graph g{ size, density };
		g.generate();
	
		ShortestPath sp(g);

		int src = r.getRandom(0, size/2);
		int dst = r.getRandom(size/2, size);

		std::cout << "Graph size: " << g.vertices() << ", density: " << g.getDensity();
		std::cout << " Avg cost: " << sp.avgCost() << std::endl;
		// Dump the graph
		std::cout << g;
		std::cout << "Minimum cost between " << src << " and " << dst;
		if (sp.path(src, dst))
		{
			std::cout << " is: " << sp.pathCost(dst) << std::endl;
		}
		else
		{
			std::cout << " not possible." << std::endl;
		}
	}
	std::cout << "Average cost of the graphs: " << (avg_cost/maxIterations) << std::endl;
	std::cout << "Processing time: " << t.elapsed() << " seconds" << std::endl;
	return 0;
}
/*
Execution run output for graph size 50 and densities of 20% and 40% respectively:
Graph size: 50, density: 0.2
Minimum cost between 8 and 46 is: 8

Graph size: 50, density: 0.4
Minimum cost between 25 and 40 is: 4

Iteration #: 0
Graph size: 99, density: 0.756063 Avg cost: 4
Minimum cost between 36 and 82 is: 3
Iteration #: 1
Graph size: 56, density: 0.923827 Avg cost: 5
Minimum cost between 24 and 51 is: 2
Iteration #: 2
Graph size: 78, density: 0.84256 Avg cost: 4
Minimum cost between 19 and 39 is: 2
Iteration #: 3
Graph size: 68, density: 0.446179 Avg cost: 2
Minimum cost between 20 and 47 is: 4
Iteration #: 4
Graph size: 75, density: 0.96831 Avg cost: 4
Minimum cost between 37 and 72 is: 2
*/

