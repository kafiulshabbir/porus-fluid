#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include "drw.h"
#include <iostream>
const std::string IN_FILE = "radius.txt";
const int IMAGE_SIZE = 2000;

const float PRESSURE_BOTTOM = 100;
const float PRESSURE_TOP = 0;
const float SIGMA = 0; //7.56e-2; // FOR water at 20C in SI units, produces 75Pa pressure difference for tube of radius 1mm
const float TUBE_LENGTH = 1;//0.1;
const float U1 = 1;//1e-3; // viscosity of the invading liquid: water
const float U2 = 1;//1e-5; // viscosity of defending liquid: air
const float PI = 3.14; //std::acos(-1);
  
std::vector<std::vector<float>> Radius()
{
	std::ifstream fin(IN_FILE);
	int n, m;
	fin >> n >> m;
	
	std::vector<std::vector<float>> v(n, std::vector<float>(m));
	for(auto& row: v)
	{
		for(auto& x: row)
		{
			fin >> x;
		}
	}
	
	return v;
}

std::vector<std::vector<float>> Fill(std::vector<std::vector<float>> v)
{
	for(auto& row: v)
	{
		for(auto& x: row)
		{
			x = 0;
		}
		
	}
	
	for(auto& x: v.back())
	{
		x = 0.3;
	}
	
	return v;
}

void Draw(std::vector<std::vector<float>> v, std::vector<std::vector<float>> radius)
{
	std::reverse(v.begin(), v.end());
	std::reverse(radius.begin(), radius.end());
	
	float max_radius = -1;
	float min_radius = 1e12;
	
	for(const auto& x: radius)
	{
		for(auto y: x)
		{
			max_radius = std::max(max_radius, y);
			min_radius = std::min(min_radius, y);
		}
	}
	

	
	const int image_size = IMAGE_SIZE;
	const int length = v.front().size();
	const int height = v.size();
	
	const int effective_length = image_size / (std::max(length, height) + 2);
	
	drw::bmp a(image_size, image_size);
	
	const int start_y = effective_length;
	const int start_x = effective_length;
	const int length_vector = std::sqrt(2) * effective_length;
	const float max_thick = length_vector / 4.0;
	const float min_thick = length_vector / 20.0;
	
	
	int y = start_y;
	for(int row = 0; row < v.size(); ++ row)
	{
		const auto& w = v[row];
		int x = start_x + effective_length * (row % 2);
		for(int col = 0; col < w.size(); ++ col)
		{
			int sign = (1 - 2 * (row % 2)) * (1 - 2 * (col % 2));
			const float r = radius[row][col];
			float thick = min_thick;
			if(max_thick != min_thick)
			{
				thick  += (r - min_radius) * (max_thick - min_thick) / (max_radius - min_radius);
			}
			a.drawVector(x, y, length_vector, thick, sign, w[col]);
			x += 2 * effective_length * (sign > 0);
		}
		
		y += effective_length;
	}
	a.save("a.bmp");
}

float vl(const float f)
{
	auto u1 = U1;
	auto u2 = U2;
	if(f < 0)
	{
		std::swap(u1, u2);
	}
	return u1 * std::abs(f) + u2 * (1.0 - std::abs(f));
}

float sig(const float f)
{
	if(std::abs(f) > 0.95 || std::abs(f) < 0.05)
	{
		return 0;
	}
	return ((f < 0) ? -1 : 1);
}

struct RelationS
{
	int i;
	int j;
	float f;
	float r;
	RelationS(int i, int j, float f, float r): i(i), j(j), f(f), r(r){}
	float vl() const
	{
		auto u1 = U1;
		auto u2 = U2;
		if(f < 0)
		{
			std::swap(u1, u2);
		}
		return u1 * std::abs(f) + u2 * (1.0 - std::abs(f));
	}
	float sig() const
	{
		if(std::abs(f) > 0.95 || std::abs(f) < 0.05)
		{
			return 0;
		}
		return ((f < 0) ? -1 : 1);
	}
};

std::vector<RelationS> Relation(const int i, const int j, const int n, const int m, const std::vector<std::vector<float>>& fill, const std::vector<std::vector<float>>& radius)
{
	std::vector<RelationS> v;
	if(i % 2)
	{
		int r1 = i - 1, c1 = 2 * j;
		int r2 = i - 1, c2 = 2 * j + 1;
		int r3 = i, c3 = 2 * j;
		int r4 = i, c4 = 2 * j + 1;
		
		v.push_back(RelationS(i - 1, j,			fill[r1][c1],			radius[r1][c1]));
		v.push_back(RelationS(i - 1, j + 1,		fill[r2][c2],		radius[r2][c2]));
		v.push_back(RelationS(i + 1, j,			fill[r3][c3] - 1,			radius[r3][c3]));
		v.push_back(RelationS(i + 1, j + 1,		fill[r4][c4] - 1,		radius[r4][c4]));
	}
	else
	{
		int r1 = i - 1, c1 = 2 * j - 1;
		int r2 = i - 1, c2 = 2 * j;
		int r3 = i, c3 = 2 * j - 1;
		int r4 = i, c4 = 2 * j;
		
		if(i > 0 && j > 0)
			v.push_back(RelationS(i - 1, j - 1,		fill[r1][c1],		radius[r1][c1]));
		if(i > 0 && j < m / 2)
			v.push_back(RelationS(i - 1, j,			fill[r2][c2],	radius[r2][c2]));
		if(i < n && j > 0)
			v.push_back(RelationS(i + 1, j - 1,		fill[r3][c3] - 1,		radius[r3][c3]));
		if(i < n && j < m / 2)
			v.push_back(RelationS(i + 1, j,			fill[r4][c4] - 1,	radius[r4][c4]));
	}
	
	return v;
}

int Linear(const int i, const int j, const int n, const int m)
{
	return (i * (m + 1) + (i % 2)) / 2 + j;
}

int NumOfNodes(const int n, const int m)
{
	return ((n + 1) * (m + 1) + 1) / 2;
}

std::vector<float> GaussElimination(std::vector<std::vector<float>> M)
{
	const int n = M.front().size() - 1;
	for(int i = 0; i < n; ++ i)
	{
		float divider = M[i][i];
		for(int j = 0; j <= n; ++ j)
		{
			M[i][j] /= divider;
		}
		
		for(int j = 0; j < n; ++ j)
		{
			if(i == j)
			{
				continue;
			}
			
			float coeff = M[j][i];
			
			for(int k = 0; k <= n; ++ k)
			{
				M[j][k] -= M[i][k] * coeff;
			}
		}
	}
	std::vector<float> v;

	for(auto& row: M)
	{
		v.push_back(row.back());
	}
	
	return v;
}


void print(const std::vector<std::vector<float>>& v)
{
	std::cout << "v:\n";
	for(const auto& row: v)
	{
		for(const auto& x: row)
		{
			std::cout << x << ", ";
		}
		std::cout << '\n';
	}
}
std::vector<std::vector<float>> Pressure(const std::vector<std::vector<float>>& fill, const std::vector<std::vector<float>>& radius)
{
	const int n = fill.size();
	const int m = fill.front().size();
	const int num_of_nodes = NumOfNodes(n, m);
	std::vector<std::vector<float>> equations; equations.reserve(num_of_nodes);
	
	for(int i = 0; i <= n; ++ i)
	{
		int mf = m / 2 + 1 - (i % 2);
		for(int j = 0; j < mf; ++ j)
		{
			std::vector<float> equation(num_of_nodes + 1);
			
			//std::cout << "\n-----------------\ni = " << i << ", j = " << j << '\n';
			
			const int linear = Linear(i, j, n, m);
			std::cout << "-----------------linear = " << linear << '\n';
			if(i == 0)
			{
				equation[linear] = 1;
				equation.back() = PRESSURE_TOP;
			}
			else if(i == n)
			{
				equation[linear] = 1;
				equation.back() = PRESSURE_BOTTOM;
			}
			else
			{
				const auto relations =  Relation(i, j, n, m, fill, radius);
				for(const auto& relation: relations)
				{
					const float r = relation.r;
					const float K = PI / 8 / TUBE_LENGTH / relation.vl();
					const int linear_j = Linear(relation.i, relation.j, n, m);
					
					const float K1 = K * std::pow(r, 4);
					const float K2 = K * 2 * SIGMA * std::pow(r, 3) * relation.sig();
					
					std::cout << "K1: " << K1 << " K2: " << K2 << " i: " << relation.i << " j: " << relation.j << " r: " << relation.r << " f: " << relation.f << '\n';
					
					equation[linear] += K1;
					equation[linear_j] -= K1;
					equation.back() -= K2;
				}
				
			}
			equations.push_back(equation);
			//print(equations);
		}
	}
	
	return equations;
}


	
	
std::vector<std::vector<float>> f()
{
	return std::vector<std::vector<float>>
	{
		{
			1, 2, 3, 5
		},
		{
			0, 1, 1, 1
		},
		{
			1, 0, 0, 1
		}
	};
}

std::vector<float> PressureDistribution(const std::vector<std::vector<float>>& fill, const std::vector<std::vector<float>>& radius)
{
	return GaussElimination(Pressure(fill, radius));
}

std::pair<int, int> Post(const int i, const int j, const int n, const int m)
{
	std::pair<int, int> p{((i + 1) / 2) * (m + 1) + (j + 1) / 2, (i / 2) * (m + 1) + m / 2 + 1 + j / 2};
	
	if(i % 2)
	{
		std::swap(p.first, p.second);
	}
	
	return p;
}

void print(const std::vector<float>& pressure, int m)
{
	int count = 0;
	
	for(auto x: pressure)
	{
		std::cout << x << "\t\t";
		++ count;
		if(count == (m / 2 + 1))
		{
			std::cout << "\n\t";
		}
		else if(count == m + 1)
		{
			count = 0;
			std::cout << '\n';
		}
	}
}

std::vector<std::vector<float>> Velocity(const std::vector<std::vector<float>>& fill, const std::vector<std::vector<float>>& radius)
{
	const auto pressure = GaussElimination(Pressure(fill, radius));
	
	std::cout << "Gausss:       \n";
	print(Pressure(fill, radius));
	
	
	const int n = fill.size();
	const int m = fill.front().size();
	
	print(pressure, m); //-----------------------------------------------------
	
	std::vector<std::vector<float>> velocity(n, std::vector<float>(m));
	for(int i = 0; i < n; ++ i)
	{
		for(int j = 0; j < m; ++ j)
		{
			const auto p = Post(i, j, n, m);
			const auto r = radius[i][j];
			const float K = PI * std::pow(r, 3) / 8.0 / TUBE_LENGTH / vl(fill[i][j]);
			velocity[i][j] = K * (r * (pressure[p.second] - pressure[p.first]) + 2.0 * SIGMA * sig(fill[i][j]) );
		}
	}
	
	return velocity;
}

std::vector<std::vector<float>> Speed(std::vector<std::vector<float>> velocity, const std::vector<std::vector<float>>& radius)
{
	for(int i = 0; i < radius.size(); ++ i)
	{
		for(int j = 0; j < radius[i].size(); ++ j)
		{
			velocity[i][j] /= PI * std::pow(radius[i][j], 2);
		}
	}
	
	return velocity;
}
			
int main()
{
	const auto radius = Radius();
	const auto fill = Fill(radius);
	
	Draw(fill, radius);
	
	const auto v = Velocity(fill, radius);
	const auto s = Speed(v, radius);
	print(s);
	
	return 0;
}

