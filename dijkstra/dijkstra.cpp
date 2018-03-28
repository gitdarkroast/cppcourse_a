/*
Implementation of the Dijsktra's shortest path algorithm.
Good background article: https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
*/

//Need the following as the first include for a Visual Sudio environment
#include "stdafx.h"
#include <random>

#include "dijkstra.h"

// Constants to drive the monte carlo simulation
static const double minDensity = 0.0;
static const double maxDensity = 1.0;
static const int minGraphSize = 50;
static const int maxGraphSize = 1000;
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

std::ostream& operator<<(std::ostream& out, const Vertex& v)
{
	out << "[" << v.m_id << "]: ";
	for (auto const& e : v.m_edges)
		out << e;
	return out;
}

//  Add a new edge to the list
void Vertex::addEdge(Edge e) 
{ 
	// We only add the edge if it does not already exists
	if (!exists(e, m_edges))
	{
		m_edges.push_back(e);
	}
}

// Delete an edge - provided it exists
bool Vertex::deleteEdge(Edge e)
{
	bool bResult = false;
	if (exists(e, m_edges))
	{
		for (int i = 0; i < m_edges.size(); ++i)
		{
			if (bResult = (e == m_edges[i]))
			{
				m_edges.erase(m_edges.begin() + i);
			}
		}
	}
	return bResult;
}


// Graph methods

// Checks to see if there is an edge between
// two vertices
bool Graph::adjacent(Vertex src, Vertex dst)
{
	int v_id = src.getID();
	try
	{
		for (Vertex& v : m_list.at(v_id))
		{
			if (v_id == v.getID())
			{
				return true;
			}
		}
	}
	catch (std::out_of_range)
	{
		std::cerr << "Attempting to access out of range vertex.";
	}
	return false;
}

// Return all the vertices that are neighbors to the given vertex
std::vector<Vertex> Graph::neighbors(Vertex v)
{
	return m_list[v.getID()];
}

// Generate a random graph based on the density.
void Graph::generate()
{
	// Initialize the vector to the number of vertices
	m_list.resize(m_size);

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
				// The vertex i has an edge
				int e_src = i;
				int e_dst = j;
				int e_cost = r.getRandom(minEdgeCost, maxEdgeCost);
				Edge e{ i, j, e_cost};
				src.addEdge(e);
			}

		}
		m_list[i].push_back(src);
	}
}

// PriorityQueue methods

// Find the minimum edge in the set
bool PriorityQueue::minimum(Set& min, const std::vector<Set> set)
{
	if (0 == set.size())
	{
		return false;
	}
	Edge min = set[0];
	for (auto const& elem : set)
	{
		if (min.cost() > elem.cost())
		{
			min = elem;
		}
	}
}

// ShortestPath methods

// Determines the minimum cost path between src and dst.
// Returns true if a path was found, false otherwise.
bool ShortestPath::path(Vertex src, Vertex dst)
{
	bool bResult{ false };
	Edge e{ src.getID(), src.getID(), 0 };
	PriorityQueue queue{ e };
	if (0 == src.neighbors().size)
	{
		// Our starting point has no neighbors
		return false;
	}

	while(!queue.contains(dst.getID(), queue.openset()))
	{
		// Get the neighbours
		for (auto const& elem : src.neighbors())
		{
			queue.insert(elem, queue.closeset());
		}
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

		// Instantiate the ShortestPath with the newly generated graph
		ShortestPath sp(g);
		std::cout << "Size of graph: " << sp.vertices() << std::endl;

		//std::cout << g;
	}
	std::cout << "Average cost of the graphs: " << avg_cost << std::endl;
	std::cout << "Processing time: " << t.elapsed() << " seconds" << std::endl;
	return 0;
}