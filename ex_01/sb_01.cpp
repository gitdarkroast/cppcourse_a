// ex_01.cpp : Defines the entry point for the console application.
//

// Using VS so need the following include.
#include "stdafx.h"
// Convert this programe to C++
// Change to C++ IO
// Change to one line comments
// Change defines of constants to const
// Change array to vector<>
// Inline any short function

#include <iostream>
#include <vector>
#include <chrono>

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

const int N = 40;

template <class T>
void sum(T *p,  std::vector<T> d)
{
	for (auto const &element : d)
	{
		*p += element;
	}
}

template <class T>
T subtract(const std::vector<T> array, int size, T s = 0)
{
	for (auto const &element : array)
	{
		s -= element;
	}
	return s;
}


int main()
{
	int accum = 0;
	double daccum = 0.0;
	std::vector<int> ndata;
	std::vector<double> ddata;

	Timer t;

	for (int i = 0; i < N; ++i)
	{
		ndata.push_back(i);
		ddata.push_back(static_cast<double>(i));
	}

	sum(&accum, ndata);
	sum(&daccum, ddata);
	std::cout << "Int Sum is " << accum << std::endl;
	std::cout << "Double Sum is " << daccum << std::endl;
	std::cout << "Time taken: " << t.elapsed() << "\n";
	std::cout << "Int subtraction is " << subtract(ndata, N) << std::endl;
	std::cout << "Double subtraction is " << subtract(ddata, N) << std::endl;
	std::cout << "Time taken: " << t.elapsed() << "\n";


	int size;
	std::cout << "Enter matrix size: ";
	std::cin >> size;

	std::vector<std::vector<int>> v_int;

	v_int.resize(size);

	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			v_int[i].push_back(j);
		}
	}

	for (int i = 0; i < size; ++i)
	{
		std::cout << i;
		for (int j = 0; j < v_int[i].size(); ++j)
		{
			std::cout << "->" << v_int[i][j];
		}
		std::cout << std::endl;
	}

}
