#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

std::ifstream fin("radius_distribution.txt");
std::ofstream fout("pressure_matrix_out.txt");

const double AMBIENT_PRESSURE_INPUT = 10; // [Pa]
const double AMBIENT_PRESSURE_OUTPUT = 2; // [Pa]
const double LEAK_RADIUS = 0.001; // [m]

const double LENGTH_TUBE = 0.1; // [m]
const double VISCOSITY_FLUID = 1e-3; // [Pa-s]
const double PI = std::acos(-1);
//const double DENSITY_FLUID = 1e3 // [kg/m3]

/*
 * Pouisell's law:
 * q: volumetric fow rate out from node_i to node_j
 * q = PI / (8 * VISCOSITY_FLUID * LENGTH_TUBE) * radius(i,j) ^ 4 * (P_i - P_j)

 * K = PI / (8 * VISCOSITY_FLUID * LENGTH_TUBE)
 */

const double K = PI / 8 / VISCOSITY_FLUID / LENGTH_TUBE;

// radius(i, j) == radius(j, i) ALWAYS
// q = K * radius(i, j) ^ 4 * (P_i - P_j) Flow of liquid out of node_i

// N is the diamension of node-grid

int ReadDiamension()
{
	int n;
	fin >> n;
	return n;
}

std::vector<std::vector<double>> ReadMatrix(int n, int m)
{
	std::vector<std::vector<double>> input_matrix(n, std::vector<double>(m));
	for(auto& row: input_matrix)
	{
		for(auto& cell: row)
		{
			fin >> cell;
		}
	}
	
	return input_matrix;
}

std::vector<std::vector<double>> ReadRadiusHorizontal(int n)
{
	return ReadMatrix(n, n - 1);
}

std::vector<std::vector<double>> ReadRadiusVertical(int n)
{
	return ReadMatrix(n - 1, n );
}
	
struct point
{
	int row;
	int col;
	int n;
	
	point(int row_input, int col_input, int n)
	{
		row = row_input;
		col = col_input;
	}
	
	point up() const
	{
		return point(row - 1, col, n);
	}
	
	point down() const
	{
		return point(row + 1, col, n);
	}
	
	point left() const
	{
		return point(row, col - 1, n);
	}
	
	point right() const
	{
		return point(row, col + 1, n);
	}
	
	point rel(int u) const
	{
		if(u == 0)
		{
			return up();
		}
		if(u == 1)
		{
			return right();
		}
		if(u == 2)
		{
			return down();
		}
		if(u == 3)
		{
			return left();
		}
		
		return point(-1, -1, -1);
	}
	
	int linear() const
	{
		return (n - 1) * row + col - 1;
	}
};

class radius_find
{
	int n;
	std::vector<std::vector<double>> ver;
	std::vector<std::vector<double>> hor;
	
	double fourth(double x) const
	{
		return pow(x, 4);
	}
	
public:
	radius_find(int size_matrix, const std::vector<std::vector<double>>& vertical, const std::vector<std::vector<double>>& horizontal)
	{
		n = size_matrix;
		ver = vertical;
		hor = horizontal;
	}
	
	double operator () (const point& node_i, const point& node_j) const
	{
		if(node_i.col == node_j.col)
		{
			if(node_j.row < 0)
			{
				return 0;
			}
			if(node_j.row >= n)
			{
				return 0;
			}
			return fourth(ver[std::min(node_i.row, node_j.row)][node_i.col]);
		}
		if(node_i.row == node_j.row)
		{
			if(node_j.col < 0)
			{
				return 0;
			}
			if(node_j.col >= n)
			{
				return fourth(LEAK_RADIUS);
			}
			return fourth(hor[node_i.row][std::min(node_i.col, node_j.col)]);
		}
		return -1;	
	}
};

int main()
{
	const int N = ReadDiamension();
	const auto matrix_radius_vertical = ReadRadiusVertical(N);
	const auto matrix_radius_horizontal = ReadRadiusHorizontal(N);
	
	std::string s;
	fin >> s;
	std::cout << s;
	std::cin.get();
	
	const radius_find radius(N, matrix_radius_vertical, matrix_radius_horizontal);
	
	for(int i = 0; i < N; ++ i)
	{
		for(int j = 1; j + 1 < N; ++ j)
		{
			std::vector<double> equation(N*(N-1) + 1);
			const point node_i(i, j, N);
			for(int k = 0; k < 4; ++ k)
			{	
				const auto node_other = node_i.rel(k);
				const auto k_radius_between = K * radius(node_i, node_other);
				equation[node_i.linear()] += k_radius_between;
				if(j == 1 && k == 3)
				{
					equation.back() += k_radius_between * AMBIENT_PRESSURE_INPUT;
				}
				else if(j == (N-1) && k == 1)
				{
					equation.back() += k_radius_between + AMBIENT_PRESSURE_OUTPUT;
				}
				else
				{
					equation[node_other.linear()] -= k_radius_between;
				}
			}
		}
	}
				

		
	return 0;
}


