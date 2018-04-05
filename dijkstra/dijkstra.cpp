//Need the following as the first include for a Visual Sudio environment
#include "stdafx.h"

/*
Implementation of the Dijsktra's shortest path algorithm.
Good background article: https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
Another good source: https://brilliant.org/wiki/dijkstras-short-path-finder/
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



// Vertex ADT definition
class Vertex
{
private:
	int m_id;
	std::vector<int> m_neighbors;
public:
	Vertex(int id=0) :m_id(id) {};
	int getID() { return m_id; };	
	int degree() { return m_neighbors.size(); };
	void addNeighbor(int id)
	{
		m_neighbors.push_back(id);
	}
	std::vector<int> neighbors() const { return m_neighbors; };

	friend bool operator==(const Vertex &v1, const Vertex &v2)
	{
		return v1.m_id == v2.m_id;
	};
	friend std::ostream& operator<<(std::ostream& out, const Vertex& v);
};

// Edge ADT definition
// An Edge is defined by two endpoints and the cost.
// For this implementation, the two endpoints are simply Ids 
// of the vertices.
class Edge
{
private:
	int m_src;
	int m_dst;
	int m_cost;
public:
	Edge(int e_src, int e_dst, int e_cost)
	 :m_src(e_src), m_dst(e_dst), m_cost(e_cost) {}
	int getSrc() { return m_src; };
	int getDst() { return m_dst; };
	int getCost() { return m_cost; };

	// We define two edges to be the same if the endpoints
	// are the same - note: cost is excluded
	friend bool operator== (const Edge &e1, const Edge &e2)
	{
		return ((e1.m_src == e2.m_src) &&
			(e1.m_dst == e2.m_dst));
	}

	friend std::ostream& operator<<(std::ostream& out, const Edge& e)
	{
		out << "{ " << e.m_src << ", " << e.m_dst << ", " << e.m_cost << " }";
		return out;
	}
};


// Graph ADT definition
// A set of vertices with associated edges.  Each edge has a cost
// value attached to it.
class Graph
{
private:
	std::vector<Vertex> m_nodes;
	std::vector<Edge> m_edges;
	int m_size;
	double m_density;

public:
	Graph(int size=10, double density=0.1)
		:m_size(size), m_density(density)
	{};
	double getDensity() { return m_density; };
	int vertices() { return m_size; };
	std::vector<int> neighbors(Vertex src) const;
	int edgeCost(int s_id, int d_id);
	bool adjacent(Vertex src, Vertex dst);
	void generate();

	friend std::ostream& operator<<(std::ostream& out, const Graph& g);
};

// Set SDT

// Want to be able to manage open and closed sets.
// Each containing vertex id as costs./
class Set {
private:
	int m_id;
	int m_cost;
public:
	Set(int id=0 int cost=0) : m_id(id), m_cost(cost) {};
	int id() { return m_id; };
	int cost() { return m_cost; };
	friend bool operator==(const Set& s1, const Set& s2)
	{
		// We do not equate the cost as that fluctuates
		// depending which set is defined, i.e. open or closed.
		return (s1.m_id == s2.m_id);
	};
	friend std::ostream& operator<<(std::ostream& out, const Set& s)
	{
		out << "{ " << s.m_id << ", " << s.m_cost << "}" << std::endl;
		return out;
	}
};

// PriorityQueue ADT
// Implement to maintain the closed and open sets
class PriorityQueue {
private:
	std::vector<Set> m_set;

public:
	bool contains(int id)
	{
		for (auto& elem : m_set)
		{
			if (id == elem.id())
			{
				return true;
			}
		}
		return false;
	}
	void insert(Set s)
	{
		if (!contains(s.id()))
		{
			m_set.push_back(s);
		}
	}
	void remove(Set s);
	void update(int id, int cost) { m_set[id].cost = cost; };
	bool isEmpty() { return 0 == m_set.size(); };
	bool minimum(Set& min);
};


// ShortestPath ADT
class ShortestPath
{
private:
	int m_totalCost;
	Graph m_graph;
	PriorityQueue m_openset;
	PriorityQueue m_closedset;
	PriorityQueue m_distance;
public:
	ShortestPath(Graph g) : m_graph(g), m_totalCost(0) {};
	~ShortestPath() {};

	int vertices() { return m_graph.vertices(); };
	bool path(Vertex src, Vertex dst);
	int pathCost() { return m_totalCost; };

};




/****** DIJKSTRA.H ******/


// Constants to drive the monte carlo simulation
static const double minDensity = 0.0;
static const double maxDensity = 1.0;
static const int minGraphSize = 2;
static const int maxGraphSize = 5;
static const int minEdgeCost = 1;
static const int maxEdgeCost = 10;
static const int maxIterations = 1;

// Helper functions

// Generic method to check if an edge or vertex exists
// in the provided vector container.
template <class T>
bool exists(const T elem, const std::vector<T> list)
{
	bool bResult = false;
	for (auto const& it : list)
	{
		if (bResult = (elem == it)) break;
	}
	return bResult;
}

// Vertex methods

// Ostream overload.  Output [id]: {vertices...}
std::ostream& operator<<(std::ostream& out, const Vertex& v)
{
	out << "[" << v.m_id << "]: ";
	for (auto const& n : v.m_neighbors)
		out << n;
	return out;
}



// Graph methods

// Return neighbors for a given vertex
std::vector<int> Graph::neighbors(Vertex v) const
{
	// Declare an empty vector
	std::vector<int> empty;
	for (int i = 0; i < m_nodes.size(); ++i)
	{
		if (v == m_nodes[i])
		{
			return m_nodes[i].neighbors();
		}
	}
	return empty;
}
// Checks to see if there is an edge between
// two vertices.  See keep a vector of edges,
// just search the containers for the edge.
bool Graph::adjacent(Vertex src, Vertex dst)
{
	Edge e{ src.getID(), dst.getID(), 0 };
	for (auto const& elem: m_edges)
	{
		if (e == elem)
		{
			return true;
		}
	}
	return false;
}


// Get the cost for the edge
// If we fail to find the edge, then 
// return -1.
int Graph::edgeCost(int s, int d)
{
	Edge e{ s, d, 0 };
	for (auto&  edge : m_edges)
	{
		if (e == edge)
		{
			return edge.getCost();
		}
	}
	return -1;
}

// Generate a random graph based on the density.
void Graph::generate()
{
	// Initialize the vector to the number of vertices
	m_nodes.resize(m_size);

	Random r;

	// For each vertex we need to determine the neighbors.
	// We will use the m_density variable to dictate the how dense the graph
	// is.
	for (int i = 0; i < m_size; ++i)
	{
		Vertex src{ i };
		for (int j = 0; j < m_size; ++j)
		{
			// We do not support a loop
			if (i == j)
				continue;
			if (m_density > r.getRandom(0.0, 1.0))
			{
				// Node i has a neighbor j
				src.addNeighbor(j);
				// We now create a an edge: i - > j with cost
				int e_src = i;
				int e_dst = j;
				int e_cost = r.getRandom(minEdgeCost, maxEdgeCost);
				Edge e{ i, j, e_cost};
				// Add this to the list of edges for the graph
				m_edges.push_back(e);
			}

		}
		m_nodes[i] = src;
	}
}

// Print a Graph
std::ostream& operator<<(std::ostream& out, const Graph& g)
{
	out << "Graph size: " << g.m_size << ", density: " << g.m_density << std::endl;
	for (int i = 0; i < g.m_size; ++i)
	{
		// First dump the vertices and the neighbors
		out << "[" << i << "]: ";
		std::vector<int> n = g.m_nodes[i].neighbors();
		if (n.size() > 0)
		{
			out << "{ ";
			for (int j = 0; j < n.size(); ++j)
			{
				out << n[j] << (((j+1) == n.size())? "" : ", ");
				
			}
			out << " }" << std::endl;
		}
	}
	// Now dump all the defined edges;
	for (auto const& e : g.m_edges)
	{
		out << e << std::endl;
	}
	return out;
}

// PriorityQueue methods

// Find the minimum edge in the set
bool PriorityQueue::minimum(Set & min)
{
	if (0 == m_set.size())
	{
		return false;
	}
	min = m_set[0];
	for (auto& elem : m_set)
	{
		if (min.cost() > elem.cost())
		{
			min = elem;
		}
	}
	return false;
}

void PriorityQueue::remove(Set s)
{
	for (int i = 0; i < m_set.size(); ++i)
	{
		if (s == m_set[i])
		{
			m_set.erase(m_set.begin() + i);
		}
	}
}

// ShortestPath methods

// Determines the minimum cost path between src and dst.
// Returns true if a path was found, false otherwise.
// Algorithm:
//  1. While closed set is not empty, get vertex v so that
//		i. v is not already in the open set and
//		ii. the cost associated with is the smallest in the closed set
//  2. Add v to the open set

bool ShortestPath::path(Vertex src, Vertex dst)
{
	bool bResult{ false };

	std::vector<int> neighbors = m_graph.neighbors(src);

	if (0 == neighbors.size())
	{
		// Our starting vertex has no neighbors
		return false;
	}
	// Initialise the distance array for each node
	for (int i = 0; i < m_graph.vertices(); ++i)
	{
		m_distance.insert(Set{ i, -1 });
	}
	// Set the source distance to 0
	m_distance.update(src.getID(), 0);

	// Start with our source, zero cost
	Set s{ src.getID(), 0 };
	// And place it in the openset
	m_openset.insert(s);
	for (int i = 0; i < neighbors.size(); ++i)
	{
		// Get the cost
		int cost = m_graph.edgeCost(src.getID(), neighbors[i]);
		// Go through the neighbors and build the closed set
		Set c{ neighbors[i], cost };
		m_closedset.insert(c);
	}

	// So now we have the open set with the start vertex
	// And the closed set with the start vertex's neighbors

	int dst_id = dst.getID();
	while (!(m_openset.contains(dst_id)) &&
		(!(m_closedset.isEmpty())))
	{
		// Get the vertex v from the closed set that has minimum cost 
		// and is not in the open set
		Set min;
		if (!m_closedset.minimum(min))
		{
			// Something went wrong here... bail
			return false;
		}


	}
		



	return bResult;
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
		std::cout << "Graph size: " << size << ", density: " << density << std::endl;
		Graph g{ size, density };
		g.generate();
	
		ShortestPath sp(g);
		std::cout << "Size of graph: " << sp.vertices() << std::endl;

		int src = r.getRandom(0, size);
		int dst = r.getRandom(src + 1, size);

		sp.path(Vertex(src), Vertex(dst));
		std::cout << g;
	}
	std::cout << "Average cost of the graphs: " << (avg_cost/maxIterations) << std::endl;
	std::cout << "Processing time: " << t.elapsed() << " seconds" << std::endl;
	return 0;
}

