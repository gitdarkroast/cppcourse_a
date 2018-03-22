#include "pch.h" 
#include <random>
#include "..\dijkstra\dijkstra.h"

double getRandom(double start, double end)
{
	std::random_device rd;
	std::mt19937 mersenne(rd());
	std::uniform_real_distribution<double> dist{ start, end };

	double result = dist(mersenne);
	return result;
}

int getRandom(int start, int end)
{
	std::random_device rd;
	std::mt19937 mersenne(rd());
	std::uniform_int_distribution<int> dist{ start, end };

	int result = dist(mersenne);
	return result;
}

namespace {

	// Fixture class for the Vertex ADT
	class VertexTest : public ::testing::Test
	{

	};

	// Test cased for the Vertex ADT implementation
	TEST(VertexTest, VertexObjExists)
	{
		ASSERT_TRUE(new Vertex(1));
	}
	
	// Test the default Vertex Id
	TEST(VertexTest, DefaultVertexId)
	{
		Vertex v{ 1 };
		EXPECT_EQ(1, v.getID());
	}

	// Test the setting of the vertex id value
	TEST(VertexTest, SetVertexIDValue)
	{
		int id = 10;
		Vertex v{ id };
		EXPECT_EQ(id, v.getID());
	}
	// Test default edge list
	TEST(VertexTest, NeighborsEmpty)
	{
		int id = 10;
		Vertex v{ id };
		std::vector<Edge> list = v.neighbors();
		EXPECT_EQ(0, list.size());
	}

	// Test default degree of vertex
	TEST(VertexTest, VertexDegreeZero)
	{
		int id = 10;
		Vertex v{ id };
		EXPECT_EQ(0, v.degree());
	}

	// Test the default Edge constructor
	TEST(EdgeTest, DefaultEdgeObject)
	{
		int id = 10;
		Vertex v1{ id };
		id = 11;
		Vertex v2{ id };
		Edge e{ v1, v2 };

		EXPECT_EQ(-1, e.getCost());
	}
	// Test set edge weight
	TEST(EdgeTest, SetEdgeWeight)
	{
		int id = 10;
		Vertex v1{ id };
		id = 11;
		Vertex v2{ id };
		Edge e{ v1, v2 };

		e.setCost(11);
		EXPECT_EQ(11, e.getCost());
	}

	// Fixture class for the Edge ADT
	class EdgeTest : public ::testing::Test
	{

	};



	// Fixture class for the Graph ADT
	class GraphTest : public ::testing::Test
	{
	protected:
	};
	// Test cased for the Graph ADT implementation
	TEST(GraphTest, GraphObjExists)
	{
		ASSERT_TRUE(new Graph());
	}

	// Test default values
	TEST(GraphTest, GraphDefaultValues)
	{
		Graph g;
		EXPECT_EQ(10, g.getSize());
		EXPECT_EQ(0.1, g.getDensity());
		EXPECT_EQ(1, g.getMinCost());
		EXPECT_EQ(10, g.getMaxCost());
	}

	// Test initial verticies
	TEST(GraphTest, GraphEmptyVertices)
	{
		Graph g;
		EXPECT_EQ(0, g.vertices());
	}

	// Test addition of 2 vertices
	TEST(GraphTest, GraphAddTwoVertices)
	{
		Graph g;
		Vertex v1{ 1 };
		Vertex v2{ 2 };
		g.addVertex(v1);
		EXPECT_EQ(1, g.vertices());
		g.addVertex(v2);
		EXPECT_EQ(2, g.vertices());
	}

	// Test removal of vertices
	TEST(GraphTest, GraphDeleteVertices)
	{
		Graph g;
		Vertex v1{ 1 };
		Vertex v2{ 2 };
		g.addVertex(v1);
		g.addVertex(v2);
		EXPECT_EQ(2, g.vertices());
		g.deleteVertex(v2);
		EXPECT_EQ(1, g.vertices());
		g.deleteVertex(v1);
		EXPECT_EQ(0, g.vertices());
	}

	// Test initiating Graph with random generated size
	TEST(GraphTest, GraphTestRandomSize)
	{
		int size = getRandom(50, 100);
		Graph g{ size };
		g.generate();
		EXPECT_EQ(size, g.vertices());
	}

	// Test initiating Graph with random generated density
	TEST(GraphTest, GraphTestRandomDensity)
	{
		int size = getRandom(50, 100);
		double density = getRandom(0.1, 1.0);
		Graph g{ size, density };
		g.generate();
		EXPECT_EQ(size, g.vertices());
		EXPECT_EQ(density, g.getDensity());
	}
} // namespace

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}