/*
Class and method definitions for the Dijkstra's shortest path algorithm.
*/
#pragma once
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
// Each containing vertex id as costs.
class Set {
private:
	int m_id;
	int m_cost;
public:
	Set(int id, int cost) : m_id(id), m_cost(cost) {};
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

	bool minimum(Set& min);
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
	bool isEmpty() { return 0 == m_set.size(); };
};


// ShortestPath ADT
class ShortestPath
{
private:
	int m_totalCost;
	Graph m_graph;
	PriorityQueue m_openset;
	PriorityQueue m_closedset;
public:
	ShortestPath(Graph g) : m_graph(g), m_totalCost(0) {};
	~ShortestPath() {};

	int vertices() { return m_graph.vertices(); };
	bool path(Vertex src, Vertex dst);
	int pathCost() { return m_totalCost; };

};

