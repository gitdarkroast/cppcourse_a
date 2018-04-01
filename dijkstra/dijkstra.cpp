/*
Implementation of the Dijsktra's shortest path algorithm.
Good background article: https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
Another good source: https://brilliant.org/wiki/dijkstras-short-path-finder/
*/

//Need the following as the first include for a Visual Sudio environment
#include "stdafx.h"
#include <random>

#include "dijkstra.h"

// Constants to drive the monte carlo simulation
static const double minDensity = 0.0;
static const double maxDensity = 1.0;
static const int minGraphSize = 50;
static const int maxGraphSize = 100;
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

// ShortestPath methods

// Determines the minimum cost path between src and dst.
// Returns true if a path was found, false otherwise.
bool ShortestPath::path(Vertex src, Vertex dst)
{
	bool bResult{ false };

	std::vector<int> neighbors = m_graph.neighbors(src);

	if (0 == neighbors.size())
	{
		// Our starting vertex has no neighbors
		return false;
	}

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

	// While the dst in not in the open set 
	// And we have not exhausted the closed set entries
	// Leep looking
	int dst_id = dst.getID();
	while (!(m_openset.contains(dst_id)) &&
		(!(m_closedset.isEmpty())))
	{

	}
		



	return bResult;
}


// Here we implement the Monte Carlo simulation:
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

