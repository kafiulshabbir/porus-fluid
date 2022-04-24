#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <stdexcept>

std::ifstream finr("radius_distribution.txt");
std::ifstream fins("settings.txt");
std::ofstream foutp("pressure_matrix_out.txt");

/*
 * Pouisell's law:
 * q: volumetric fow rate out from node_i to node_j
 * q = PI / (8 * VISCOSITY_FLUID * LENGTH_TUBE) * radius(i,j) ^ 4 * (P_i - P_j)

 * K = PI / (8 * VISCOSITY_FLUID * LENGTH_TUBE)
 * radius(i, j) == radius(j, i) ALWAYS
 * q = K * radius(i, j) ^ 4 * (P_i - P_j) Flow of liquid out of node_i

 * N is the diamension of node-grid
 */


const double ReadSettings()
{
	std::string s;
	fins >> s;
	
	double ipt;
	fins >> ipt;
	
	return ipt;
}

struct point
{
	int row;
	int col;
	int n;
	
	point(int row, int col, int n): row(row), col(col), n(n) {}
	
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
		
		throw std::invalid_argument("relative position is not correct!");
		
		return point(-1, -1, -1);
	}
	
	int linear() const
	{
		return n * row + col;
	}
};

class radius_class
{
private:
	int n;
	double leak_radius;
	
	std::vector<std::vector<double>> ver;
	std::vector<std::vector<double>> hor;
	
	double fourth(double x) const
	{
		return std::pow(x, 4);
	}
	
	template <typename T>
	const T Read()
	{
		std::string s;
		finr >> s;
		
		double ipt;
		finr >> ipt;
		
		return ipt;
	}
	
	std::vector<std::vector<double>> ReadMatrix(int n, int m, double multiplier)
	{
		std::vector<std::vector<double>> input_matrix(n, std::vector<double>(m));
		for(auto& row: input_matrix)
		{
			for(auto& cell: row)
			{
				finr >> cell;
				cell *= multiplier;
			}
		}
		
		return input_matrix;
	}

	std::vector<std::vector<double>> ReadRadiusHorizontal(int n, double multiplier = 1)
	{
		return ReadMatrix(n, n - 1, multiplier);
	}

	std::vector<std::vector<double>> ReadRadiusVertical(int n, double multiplier = 1)
	{
		return ReadMatrix(n - 1, n, multiplier);
	}

public:
	radius_class()
	{
		n = Read<int>();
		double multiplier = Read<double>();
		leak_radius = multiplier * Read<double>();
		
		ver = ReadRadiusVertical(n, multiplier);
		hor = ReadRadiusHorizontal(n, multiplier);
	}
	
	double operator () (const point& node_i, const point& node_j) const
	{
		if(node_i.col == node_j.col)
		{
			if(node_j.row < 0 || node_j.row >= n)
			{
				return 0;
			}
			return fourth(ver[std::min(node_i.row, node_j.row)][node_i.col]);
		}
		if(node_i.row == node_j.row)
		{
			if(node_j.col < 0 || node_j.col >= n)
			{
				return fourth(leak_radius);
			}
			return fourth(hor[node_i.row][std::min(node_i.col, node_j.col)]);
		}
		
		throw std::invalid_argument("the two nodes are not adjacent to each other, and a tube between them does not exist!");
		
		return -1;	
	}
	
	int N() const
	{
		return n;
	}
};

void print(const std::vector<std::vector<double>>& v)
{
	for(const auto& row: v)
	{
		for(auto& cell: row)
		{
			std::cout << cell << ", ";
		}
		
		std::cout << "\n";
	}
}

std::vector<double> GaussElimination(std::vector<std::vector<double>> v)
{
	const int N = v.front().size() - 1;
	for(int i = 0; i < N; ++ i)
	{
		double divider = v[i][i];
		for(int j = 0; j <= N; ++ j)
		{
			v[i][j] /= divider;
		}
		
		for(int j = 0; j < N; ++ j)
		{
			if(i == j)
			{
				continue;
			}
			
			double coeff = v[j][i];
			
			for(int k = 0; k <= N; ++ k)
			{
				v[j][k] -= v[i][k] * coeff;
			}
		}
		//std::cout << "\n\n----------------\n";
		//print(v);
	}
	
	std::vector<double> pressures;
	
	for(auto& row: v)
	{
		pressures.push_back(row.back());
	}
	
	return pressures;
}
		
int main()
{
	const double PI = std::acos(-1);
	const double AMBIENT_PRESSURE_INPUT = ReadSettings();
	const double AMBIENT_PRESSURE_OUTPUT = ReadSettings();
	const double LENGTH_TUBE = ReadSettings();
	const double VISCOSITY_FLUID = ReadSettings();
	const double DENSITY_FLUID = ReadSettings();
	
	const double K = PI / 8 / VISCOSITY_FLUID / LENGTH_TUBE;
	
	const radius_class radius;
	const int N = radius.N();
	//std::cout << K << "\n";
	//std::cin.get();
	
	std::vector<std::vector<double>> gauss_matrix;
	for(int i = 0; i < N; ++ i)
	{
		for(int j = 0; j < N; ++ j)
		{
			std::vector<double> equation(N * N + 1);
			const point node_i(i, j, N);
			for(int k = 0; k < 4; ++ k)
			{	
				const point node_other = node_i.rel(k);
				const double k_radius_between = K * radius(node_i, node_other);
				equation[node_i.linear()] += k_radius_between;
				if(j == 0 && k == 3)
				{
					equation.back() += k_radius_between * AMBIENT_PRESSURE_INPUT;
				}
				else if(j == (N-1) && k == 1)
				{
					equation.back() += k_radius_between * AMBIENT_PRESSURE_OUTPUT;
				}
				else
				{
					equation[node_other.linear()] -= k_radius_between;
				}
			}
			
			gauss_matrix.push_back(equation);
		}
	}
	
	std::vector<double> calculated_pressures = GaussElimination(gauss_matrix);
	
	int count = 0;
	for(int i = 0; i < N; ++ i)
	{
		foutp << AMBIENT_PRESSURE_INPUT << ' ';
		for(int j = 0; j < N; ++ j)
		{
			foutp << calculated_pressures[count++] << ' ';
		}
		foutp << AMBIENT_PRESSURE_OUTPUT << '\n';
	}
	
	return 0;
}


