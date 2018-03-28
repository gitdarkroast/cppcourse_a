/*
Class and method definitions for the Dijkstra's shortest path algorithm.
*/
#pragma once
#include <iostream>
#include <chrono>
#include <vector>
#include <random>


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



// Forward declaration of Edge ADT
class Edge;

// Vertex ADT definition
class Vertex
{
private:
	int m_id;
	std::vector<Edge> m_edges;
public:
	Vertex(int id) :m_id(id) {};
	int getID() { return m_id; };	
	void setID(int id) { m_id = id; };
	int degree() { return m_edges.size(); };
	std::vector<Edge> neighbors() { return m_edges; };
	void addEdge(Edge e);
	bool deleteEdge(Edge e);

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
	std::vector<std::vector<Vertex>> m_list;
	int m_size;
	double m_density;

public:
	Graph(int size=10, double density=0.1)
		:m_size(size), m_density(density)
	{};
	double getDensity() { return m_density; };
	int vertices() { return m_size; };
	bool adjacent(Vertex src, Vertex dst);
	std::vector<Vertex> neighbors(Vertex v);
	void generate();

	friend std::ostream& operator<<(std::ostream& out, const Graph& g)
	{
		out << "Graph size: " << g.m_size << ", density: " << g.m_density << std::endl;
		for (int i = 0; i < g.m_size; ++i)
		{
			for (auto const& elem : g.m_list[i])
			{
				 out << elem;

			}
			out << std::endl;
		}
		return out;
	}
};

// Set SDT

class Set {
private:
	int m_id;
	int m_cost;
public:
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
	std::vector<Set> m_openset;
	std::vector<Set> m_closedset;

	bool minimum(Set& min, const std::vector<Set> s);
public:
	// We start with the source in the openset
	PriorityQueue(Set s) {
		m_openset.push_back(s);
	}

	std::vector<Set> openset() { return m_openset; };
	std::vector<Set> closedset() { return m_closedset; };

	bool contains(Set e, std::vector<Set> s)
	{
		for (auto const& elem : s)
		{
			if (elem == e)
			{
				return true;
			}
		}
		return false;
	}
	void insert(Set s, std::vector<Set> set)
	{
		if (!contains(s, set))
		{
			set.push_back(e);
		}
	}
	bool moveToOpen(Edge e)
	{
		return true;
	}
};


// ShortestPath ADT
class ShortestPath
{
private:
	int m_totalCost;
	Graph m_graph;
	PriorityQueue m_queue;
public:
	ShortestPath(Graph g) : m_graph(g), m_totalCost(0) {};
	~ShortestPath() {};

	int vertices() { return m_graph.vertices(); };
	bool path(Vertex src, Vertex dst);
	int pathCost() { return m_totalCost; };

};

