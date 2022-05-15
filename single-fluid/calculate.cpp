#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <stdexcept>

std::ifstream finr("in_radius_distribution.txt");
std::ifstream fins("in_settings.txt");
std::ofstream foutp("out_pressure_matrix.txt");
std::ofstream foutvr("out_flow_vertical.txt");
std::ofstream fouthr("out_flow_horizontal.txt");

typedef std::vector<std::vector<double>> matrix;
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
	
	matrix ver;
	matrix hor;
	
	double fourth(double x) const
	{
		return std::pow(x, 4);
	}
	
	template <class T>
	T Read()
	{
		std::string s;
		finr >> s;
		
		T ipt;
		finr >> ipt;
		
		return ipt;
	}
	
	matrix ReadMatrix(int n, int m, double multiplier)
	{
		matrix input_matrix(n, std::vector<double>(m));
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

	matrix ReadRadiusHorizontal(int n, double multiplier = 1)
	{
		return ReadMatrix(n, n - 1, multiplier);
	}

	matrix ReadRadiusVertical(int n, double multiplier = 1)
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

void print(const matrix& v)
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

std::vector<double> GaussElimination(matrix v)
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
	
	matrix gauss_matrix;
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
	
	matrix pressure_matrix(N);
	
	int count = 0;
	for(int i = 0; i < N; ++ i)
	{
		pressure_matrix[i].push_back(AMBIENT_PRESSURE_INPUT);
		for(int j = 0; j < N; ++ j)
		{
			pressure_matrix[i].push_back(calculated_pressures[count++]);
		}
		pressure_matrix[i].push_back(AMBIENT_PRESSURE_OUTPUT);
	}
	
	for(const auto& v: pressure_matrix)
	{
		for(auto x: v)
		{
			foutp << x << ' ';
		}
		foutp << '\n';
	}
	
	for(int i = 0; i + 1 < N; ++ i)
	{
		for(int j = 0; j < N; ++ j)
		{
			double pressure_difference = std::abs(pressure_matrix[i][j + 1] - pressure_matrix[i + 1][j + 1]);
			point node(i, j, N);
			double flow_rate = K * radius(node, node.down()) * pressure_difference;
			foutvr << flow_rate << ' ';
		}
		foutvr << '\n';
	}
	
	for(int i = 0; i < N; ++ i)
	{
		for(int j = 0; j <= N; ++ j)
		{
			double pressure_difference = std::abs(pressure_matrix[i][j] - pressure_matrix[i][j + 1]);
			point node(i, j, N);
			double flow_rate = K * radius(node, node.right()) * pressure_difference;
			fouthr << flow_rate << ' ';
		}
		fouthr << '\n';
	}
	/*
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
	*/
	return 0;
}


