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
};

// Edge ADT definition
// An Edge is defined by two endpoints and the cost.
// The two endpoints are simply Ids of the vertices.
class Edge
{
private:
	int m_src;
	int m_dst;
	int m_cost;
public:
	Edge(int e_src, int e_dst, int e_cost)
	 :m_src(e_src), m_dst(e_dst), m_cost(e_cost) {}
	const int getSrc() { return m_src; };
	const int getDst() { return m_dst; };
	int getCost() { return m_cost; };

	// We define two edegs to be the same if the endpoints
	// are the same - note: cost is excluded
	friend bool operator== (const Edge &e1, const Edge &e2)
	{
		return ((e1.m_src == e2.m_src) &&
			(e1.m_dst == e2.m_dst));
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
	int m_minCost;
	int m_maxCost;
	double m_density;

public:
	Graph(int size=10, double density=0.1, int min=1, int max=10) 
		:m_size(size), m_density(density), m_minCost(min), m_maxCost(max)
	{};
	double getDensity() { return m_density; };
	int getMinCost() { return m_minCost; };
	int getMaxCost() { return m_maxCost; };
	int vertices() { return m_size; };
	bool adjacent(Vertex src, Vertex dst);
	std::vector<Vertex> neighbors(Vertex v);
	void generate();
	int getVertexID(Vertex v) { return v.getID(); };
};

// PriorityQueue ADT
class PriorityQueue {
private:
	std::vector<Vertex> m_list;
public:
	void chgPriority(Vertex v) { m_list[0] = v; };
	void minPriority() { m_list.erase(m_list.begin() + 0); };
	bool contains(Vertex v)
	{
		for (auto const& elem : m_list)
		{
			if (v == elem)
			{
				return true;
			}
		}
		return false;
	}
	void insert(Vertex v)
	{
		if (!contains(v))
		{
			m_list.push_back(v);
		}
	}
	Vertex top() { m_list.pop_back(); };
	int size() { m_list.size(); };
};


// ShortestPathAlgo ADT
class ShortestPathAlgo
{
private:
	Graph m_graph;
	PriorityQueue m_queue;
public:
	ShortestPathAlgo() {};
	~ShortestPathAlgo() {};

	std::vector<Vertex> vertices() { m_graph.vertices(); };

};

