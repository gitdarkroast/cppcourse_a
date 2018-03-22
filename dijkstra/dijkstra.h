#pragma once
#include <iostream>
#include <chrono>
#include <vector>


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


// Forward declaration of Edge ADT
class Edge;

// Vertex ADT definition
// The Vertex ADT consists of:
//		- a vertex ID
//		- a vector of edges - the destination end point being another vertex.
class Vertex
{
private:
	int m_id;											// ID to define the vertex
	std::vector<Edge> m_edges;								// List of edges for this vertex
public:
	Vertex(int id) :m_id(id) {};						// Constructor, default ID is 0.
	int getID() { return m_id; };	
	void setID(int id) { m_id = id; };
	int degree() { return m_edges.size(); };				// The number of neighbors this vertex has
	std::vector<Edge> neighbors() { return m_edges; };		// The list edges from this vertex.
	void addEdge(Edge e);
	bool deleteEdge(Edge e);

	friend bool operator==(const Vertex &v1, const Vertex &v2)
	{
		return v1.m_id == v2.m_id;
	};
};

// Edge ADT definition
// The Edge ADT consists of:
//		- The endpoint pair (src,dst) of vertices
//		- The cost of going from src to dst
class Edge
{
private:
	Vertex m_src;
	Vertex m_dst;
	int m_cost{ -1 };
public:
	Edge(Vertex v_src, Vertex v_dst) :m_src(v_src), m_dst(v_dst) {}
	Vertex getSrc() { return m_src; };
	Vertex getDst() { return m_dst; };
	void setCost(int c) { m_cost = c; };
	int getCost() { return m_cost; };

	friend bool operator== (const Edge &e1, const Edge &e2)
	{
		return ((e1.m_src == e2.m_src) &&
			(e1.m_dst == e2.m_dst));
	}
};

// Adjacent List ADT definition
/*
Example:
1->2->3->4
2->4
3->1->4
4
*/
class AdjacentList
{
private:
	std::vector<std::vector<Vertex>> m_vlist;
public:
	AdjacentList() {};
	~AdjacentList() {};

	//friend std::ostream& operator<<(std::ostream& out, const AdjacentList& list);

};

// Connectivity Matrix ADT definition
/*
Example:
	1	2	3	4
1	0	1	1	1
2	0	0	0	1
3	0	0	0	1
4	0	0	0	0
*/
class ConnectivityMatrix
{
private:
	std::vector< std::vector<int> > m_representation;	// A two-dimensional array
public:
	ConnectivityMatrix() {};
	~ConnectivityMatrix() {};

	//friend std::ostream& operator<<(std::ostream& out, const ConnectivityMatrix& matrix);
};

// Graph ADT definition
/*
The Graph ADT manages a set of nodes with a specific representation
*/
class Graph
{
private:
	std::vector<Vertex> m_vertices;
	std::vector<std::vector<Vertex>> m_list;
	int m_size;
	int m_minCost;
	int m_maxCost;
	double m_density;

public:
	Graph(int size=10, double density=0.1, int min=1, int max=10) 
		:m_size(size), m_density(density), m_minCost(min), m_maxCost(max)
	{};
	int getSize() { return m_size; };
	double getDensity() { return m_density; };
	int getMinCost() { return m_minCost; };
	int getMaxCost() { return m_maxCost; };
	int vertices() { return m_vertices.size(); };
	bool adjacent(Vertex src, Vertex dst);
	std::vector<Vertex> neighbors(Vertex v);
	void generate();
	int getVertexID(Vertex v) { return v.getID(); };
	void addVertex(Vertex v);
	bool deleteVertex(Vertex v);
};

// openSet
// a list of nodes with minimum cost


// closedSet
// a list of neighbors with associated cost

