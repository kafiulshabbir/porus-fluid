#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include "drw.h"
#include <iostream>
#include <utility>

//Type Definitions
typedef std::vector<std::vector<float>> matrix;
typedef std::vector<std::vector<bool>> tlock;

//Input and Output files
const std::string IN_FILE = "radius.txt";
const std::string FOLDER_SAVE_PIC = "pic/";

//GENERAL CONSTANTS
const float PI = std::acos(-1);

//Physical Characterstics of the experiment
const float PRESSURE_BOTTOM = 100;
const float PRESSURE_TOP = 2;
const float SIGMA = 0; //7.56e-2; // FOR water at 20C in SI units, produces 75Pa pressure difference for tube of radius 1mm
const float TUBE_LENGTH = 1;//0.1;
const float U1 = 1;//1e-3; // viscosity of the invading liquid: water
const float U2 = 1;//1e-5; // viscosity of defending liquid: air

//Parameters of simulation
const float THRESHOLD_FILL = 0.05;
const float INITIAL_FILL = 0.3;
const float TIME_SIMULATE = 100;
const float TIME_TUBE_DIV = 10;
const int IMAGE_SIZE = 1000;


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

void print(const matrix& m)
{
	for(const auto& row: m)
	{
		for(auto x: row)
		{
			std::cout << x << ' ';
		}
		std::cout << '\n';
	}
}


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

std::vector<std::vector<float>> InitialFill(const int row_n, const int col_n)
{
	std::vector<std::vector<float>> v(row_n, std::vector<float>(col_n));
	
	for(auto& x: v.back())
	{
		x = INITIAL_FILL;
	}
	
	return v;
}



void Draw(matrix radius, matrix fill, tlock lock, int count, const float time_ran)
{
	std::reverse(fill.begin(), fill.end());
	std::reverse(radius.begin(), radius.end());
	std::reverse(lock.begin(), lock.end());
	
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
	const int length = fill.front().size();
	const int height = fill.size();
	
	const int effective_length = image_size / (std::max(length, height) + 2);
	
	drw::bmp a(image_size, image_size);
	
	const int start_y = effective_length;
	const int start_x = effective_length;
	const int length_vector = std::sqrt(2) * effective_length;
	const float max_thick = length_vector / 4.0;
	const float min_thick = length_vector / 20.0;
	
	
	int y = start_y;
	for(int row = 0; row < fill.size(); ++ row)
	{
		const auto& w = fill[row];
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
			a.drawVector(x, y, length_vector, thick, sign, w[col], lock[row][col]);
			x += 2 * effective_length * (sign > 0);
		}
		
		y += effective_length;
	}
	a.save(FOLDER_SAVE_PIC + "pic-" + std::to_string(count) + "_t-" + std::to_string(time_ran) + ".bmp");
}

struct Pcordinate
{
	int row;
	int col;
	
	int linear(const int m) const
	{
		return (row * (m + 1) + (row % 2)) / 2 + col;
	}
	
	Pcordinate(int row, int col): row(row), col(col) {}
	
	static std::pair<int, int> get_linears(const int i, const int j, const int m)
	{
		int a = ((i + 1) / 2) * (m + 1) + (j + 1) / 2;
		int b = (i / 2) * (m + 1) + m / 2 + 1 + j / 2;
		
		if(i % 2)
		{
			std::swap(a, b);
		}
		
		return {a, b};
	}
};

struct NodeConnections
{
	Pcordinate p;
	int row_tube;
	int col_tube;
	
	NodeConnections(int row_p, int col_p, int row_tube, int col_tube): p(row_p, col_p), row_tube(row_tube), col_tube(col_tube) {}
};


std::vector<NodeConnections> NodeConnectionsGen(const int i, const int j, const int n, const int m)
{
	const int r = i % 2;
	
	std::vector<NodeConnections> v
	{
		NodeConnections(i - 1, j + r - 1,	i - 1,	2 * j + r - 1	),
		NodeConnections(i - 1, j + r,		i - 1,	2 * j + r		),
		NodeConnections(i + 1, j + r - 1,	i,		2 * j + r - 1	),
		NodeConnections(i + 1, j + r, 		i,		2 * j + r		)
	};
	
	if(r)
	{
		return v;
	}
	
	std::vector<NodeConnections> w;
	
	if(i > 0 && j > 0)
	{
		w.push_back(v[0]);
	}
	if(i > 0 && j < m / 2)
	{
		w.push_back(v[1]);
	}
	if(i < n && j > 0)
	{
		w.push_back(v[2]);
	}
	if(i < n && j < m / 2)
	{
		w.push_back(v[3]);
	}
	
	return w;
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
	if(std::abs(f) > (1.0 - THRESHOLD_FILL) || std::abs(f) < THRESHOLD_FILL)
	{
		return 0;
	}
	return ((f < 0) ? -1 : 1);
}

matrix PressureRelationMatrix(const matrix& radius, const matrix& fill, const tlock& lock)
{
	const int n = fill.size();
	const int m = fill.front().size();
	const int num_of_nodes = ((n + 1) * (m + 1) + 1) / 2;
	matrix equations; equations.reserve(num_of_nodes);
	
	for(int i = 0; i <= n; ++ i)
	{
		int mf = m / 2 + 1 - (i % 2);
		for(int j = 0; j < mf; ++ j)
		{
			std::vector<float> equation(num_of_nodes + 1);
			const int linear = Pcordinate(i, j).linear(m);
			
			//std::cout << "i=" << i << ", j=" << j << ", lin=" << linear << " ";
			
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
				const auto connections = NodeConnectionsGen(i, j, n, m);
				for(const auto& connection: connections)
				{
					//std::cout << "pair " << connection.row_tube << ", " << connection.col_tube << '\n';
					
					if(lock[connection.row_tube][connection.col_tube])
					{
						continue;
					}
					const float r = radius[connection.row_tube][connection.col_tube];
					const float f = fill[connection.row_tube][connection.col_tube];
					
					//const float K = PI / 8 / TUBE_LENGTH / vl(f);
					const float K = 1.0;
					const float K1 = K * std::pow(r, 4);
					const float K2 = K * 2 * SIGMA * std::pow(r, 3) * sig(f);

					const int linear_j = connection.p.linear(m);
					//std::cout << ", lin_j: " << linear_j << '\n';
					equation[linear] += K1;
					equation[linear_j] -= K1;
					equation.back() -= K2;
				}
				
			}
			
			//std::cout << '\n';
			equations.push_back(equation);
			//print(equations);
			//std::cout << "-------------\n\n";
		}
	}
	
	return equations;
}

std::vector<float> GaussElimination(matrix M)
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


matrix Velocity(const matrix& radius, const matrix& fill, const std::vector<float>& pressure, const tlock& lock)
{
	
	const int n = fill.size();
	const int m = fill.front().size();
	
	//print(pressure, m);
	
	std::vector<std::vector<float>> velocity(n, std::vector<float>(m));
	for(int i = 0; i < n; ++ i)
	{
		for(int j = 0; j < m; ++ j)
		{
			if(lock[i][j])
			{
				velocity[i][j] = 0;
				continue;
			}
			const auto p = Pcordinate::get_linears(i, j, m);
			const auto r = radius[i][j];
			const auto f = fill[i][j];
			/*
			const float K = PI * std::pow(r, 3) / 8.0 / TUBE_LENGTH / vl(fill[i][j]);
			velocity[i][j] = K * (r * (pressure[p.second] - pressure[p.first]) + 2.0 * SIGMA * sig(fill[i][j]) );
			*/
			//const float K = r / 8.0 / TUBE_LENGTH / vl(f);------------------------------------------------------remember----------
			const float K = r / 8.0  / vl(f);
			velocity[i][j] = K * (r * (pressure[p.second] - pressure[p.first]) + 2.0 * SIGMA * sig(f) );
		}
	}
	
	return velocity;
}


struct MinApproach
{
	float time;
	int i;
	int j;
};

MinApproach MinApproachFind(const matrix& velocity, const matrix& fill)
{
	const int n = velocity.size();
	const int m = velocity.front().size();
	
	float min_time = 1e30;
	int r;
	int c;
	for(int i = 0; i < n; ++ i)
	{
		for(int j = 0; j < m; ++ j)
		{
			const auto f = fill[i][j];
			const auto v = velocity[i][j];
			if(sig(f) != 0)
			{
				const float time = (1.0 - std::abs(f)) / v;
				if(time < min_time)
				{
					min_time = time;
					r = i;
					c = j;
				}
			}
		}
	}
	
	return {min_time, r, c};
}

matrix UpdateFill(matrix fill, const matrix& velocity, const float time_step)
{
	const int n = velocity.size();
	const int m = velocity.front().size();
	
	for(int i = 0; i < n; ++ i)
	{
		for(int j = 0; j < m; ++ j)
		{
			if(sig(fill[i][j]) != 0)
			{
				fill[i][j] += velocity[i][j] * time_step;
			}
		}
	}
	
	return fill;
}

bool Good(int i, int n)
{
	return i >= 0 && i < n;
}

std::vector<std::pair<int, int>> TubeConnections(int i, int j, int n, int m)
{
	int s = 1;
	if((i % 2) && (j % 2))
	{
		s = -1;
	}
	else if((i % 2 == 0) && (j % 2 == 0))
	{
		s = -1;
	}
	
	std::vector<std::pair<int, int>> v
	{
		{i - 1, j},
		{i - 1, j + s},
		{i, j + s}
	};
	
	std::vector<std::pair<int, int>> w;
	
	for(auto x: v)
	{
		if(Good(x.first, n) && Good(x.second, m))
		{
			w.push_back(x);
		}
	}
	
	return w;
}

matrix Spread(matrix fill, tlock& lock)
{
	const int n = fill.size();
	const int m = fill.front().size();
	
	for(int i = 0; i < n; ++ i)
	{
		for(int j = 0; j < m; ++ j)
		{
			if(std::abs(fill[i][j]) > 1.0 - THRESHOLD_FILL)
			{
				fill[i][j] = 1;
			
				const auto tube_connections = TubeConnections(i, j, n, m);
				std::cout << "\ni = " << i << ", j = " << j << " connections = \n";
				
				for(auto [r, c]: tube_connections)
				{
					std::cout << "connections are, r = " << r << ", c = " << c << '\n';
					if(r == i - 1)
					{
						if(fill[r][c] == 0)
						{
							fill[r][c] = THRESHOLD_FILL + 0.01;
						}
					}	
					else if (fill[r][c] > 1.0 - THRESHOLD_FILL)
					{
						fill[r][c] = 1;
					}
					else 
					{
						lock[r][c] = true;
					}
				}
			} 
		}
	}
	
	return fill;
}

int main()
{
	const auto radius = Radius();
	const int n = radius.size();
	const int m = radius.front().size();
	
	auto fill = InitialFill(n, m);
	auto lock = tlock(n, std::vector<bool>(m, false));
	
	float time_ran = 0;
	int count = 0;
	Draw(radius, fill, lock, count, time_ran);
	while(time_ran < TIME_SIMULATE)
	{
		//form the system of linear equations
		//Determine pressure: solve the sysetm of linear euquations, through gaussian elimination
		//Calculate the velocites in each of the tubes
		//if in any tube the meniscus is being pushed backwards, close it -> recalculate pressure and velocities again
		//Find the closest in time of all tubes with meniscus
		//if abs(fill) > (1 - 1.0/TIME_TUBE_DIV) time_step = fill/velocity else time_step = Length / TIME_TUBE_DIV / velocity
		//perform the integration
		//if any meniscus reached THRESHOLD_FILL then spread it out
		//save the picture
		
		auto pressure_relation_matrix = PressureRelationMatrix(radius, fill, lock);
		auto pressure_vector = GaussElimination(pressure_relation_matrix);
		auto velocity = Velocity(radius, fill, pressure_vector, lock);
		
		const auto min_approach = MinApproachFind(velocity, fill);
		auto time_step = min_approach.time;
		const auto i = min_approach.i;
		const auto j = min_approach.j;
		const auto f = std::abs(fill[i][j]); 
		if((1.0 - f) * TIME_TUBE_DIV > 1)
		{
			time_step = 1.0 / TIME_TUBE_DIV / velocity[i][j];
		}		
		fill = UpdateFill(fill, velocity, time_step);
		fill = Spread(fill, lock);
		//std::cout << "\nvelo: -------------\n";print(velocity);std::cout << "\noldfill: -------------\n";print(fill);
		std::cout << "\n\n---------------\nCount = " << count << "\nfill:\n"; print(fill); std::cout << '\n';
		
		time_ran += time_step;
		++ count;
		Draw(radius, fill, lock, count, time_ran);
	}
	return 0;
}
