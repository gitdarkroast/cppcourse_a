/*
Implementation of the Dijktra's shortest path algorithm.
Good background article: https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
*/

//Need the following as the first include for a Visual Sudio environment
#include "stdafx.h"

#include "dijkstra.h"

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


// ConnectivityMatrix methods
// Overload operator<< to output the matrix representation
std::ostream& operator<<(std::ostream& out, const ConnectivityMatrix& matrix)
{
	for (int i = 0; i < matrix.m_representation.size(); ++i)
	{
		std::cout << i << " ";		// The row value - source vertex
		for (int j = 0; j < matrix.m_representation[i].size(); ++j)
		{
			std::cout << j << " "; // The column value - destination vertex
		}
		std::cout << std::endl;

	}
	return out;
}

// EdgeList methods
// Overload operator<< to output the edge list representation
std::ostream& operator<<(std::ostream& out, const EdgeList& list)
{
	for (auto const& row : list.m_representation)
	{
	}
	return out;
}

// Graph methods


// Constructor
Graph::Graph(int size)
{
	for (int i = 0; i < size; ++i)
	{
		m_vertices.push_back(Vertex(i));
	}
}

// Checks to see if there is an edge between
// two vertices
bool Graph::adjacent(const Vertex src, const Vertex dst)
{
	Edge e{ src, dst };
	return true;
}

// Return all the vertices that are neighbors to the given vertex
std::vector<Vertex> Graph::neighbors(Vertex v)
{
	std::vector<Vertex> vertices;
	// Vertex::neighbour() returns edges for the specific vertex
	// We want a list of destination vertices - those are the adjacent
	// verticies.
	for (auto & edge : v.neighbors())
	{
		vertices.push_back(edge.getDst());
	}
	return vertices;
}

// Add a vertex
void Graph::addVertex(Vertex v)
{
	if (!exists(v, m_vertices))
	{
		m_vertices.push_back(v);
	}
}

// Delete a vertex
bool Graph::deleteVertex(Vertex e)
{
	bool bResult = false;
	if (exists(e, m_vertices ))
	{
		for (int i = 0; i < m_vertices.size(); ++i)
		{
			if (bResult = (e == m_vertices[i]))
			{
				m_vertices.erase(m_vertices.begin() + i);
			}
		}
	}
	return bResult;
}

/*
Close set: known shortest distances.
Open set: what is reachable
If the node that is picked is the destination node (d), stop
Otherwise compute all successors of n
Add the distance from s to n + cost of edge n to k
And if that improves the value in the open set - store the new value (in the closed set)
Pick current open set distance that is the minimum and repeate
*/
void solveDijkstra()
{

}

/*
int main()
{
	Timer t;

	Graph g;
	Vertex v1{ 1 };
	Vertex v2{ 2 };
	g.addVertex(v1);
	std::cout << "Number of vertices: " << g.vertices() << std::endl;
	g.addVertex(v2);
	std::cout << "Number of vertices: " << g.vertices() << std::endl;
	std::cout << "Vertex id : " << g.getVertexID(v2) << std::endl;
	g.deleteVertex(v2);
	std::cout << "Number of vertices: " << g.vertices() << std::endl;
	std::cout << "Vertex id : " << g.getVertexID(v2) << std::endl;

	Graph g2(10);



	std::cout << "Time elapsed: " << t.elapsed() << std::endl;
    return 0;
}
*/
