#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <cmath>
#include <limits>
#include "drw.h"

const std::string FILE_PARAMETERS = "in_01_parameters.txt";
const std::string FILE_PRESSURE_IN = "in_02_pressures_input.txt";
const std::string FILE_PRESSURE_OUT = "in_03_pressures_output.txt";
const std::string FILE_RADIUS = "in_04_radius_dist.txt";
const std::string FILE_FILL = "in_05_fill_dist.txt";
const std::string FILE_SIGMA = "in_06_sigma_dist.txt";
const std::string FILE_PRESSURE_DISTRIBUTION = "out_01_pressure_distribution.txt";
const std::string FILE_FLOW_RATES = "out_02_flow_rates.txt";
const std::string FILE_VELOCITY = "out_03_velocity.txt";
const std::string FILE_FILL_AFTER_STEP1 = "out_04_fill_after_one_step.txt";

const std::vector<std::pair<std::string, double>> FILE_PARAMETERS_CONTENTS
{
	{"Number_of_rows_'n'", 3},
	{"Number_of_columns_'m'", 5},
	{"Length_of_a_tube_'l'", 0.1},
	{"Viscosity_of_fluid_1_'u1'", 1e-3},
	{"Viscosity_of_fluid_2_'u2'", 2e-3},
	{"Type_of_fluid_on_the_input_'type_in'", 1},
	{"Type_of_fluid_on_the_output_'type_out'", 2}
};

class Parameters
{
	template <class T>
	T ReadLabeled(std::istream& in) const
	{
		std::string label;
		T value;
		in >> label >> value;
		
		return value;
	}	
	
public:
	int n;
	int m;
	double l;
	double u1;
	double u2;
	int type_in;
	int type_out;
	
	Parameters() = default;
	Parameters(const std::string& file_name)
	{
		std::ifstream fin(file_name);
		
		n = ReadLabeled<int>(fin);
		m = ReadLabeled<int>(fin);
		l = ReadLabeled<double>(fin);
		u1 = ReadLabeled<double>(fin);
		u2 = ReadLabeled<double>(fin);
		type_in = ReadLabeled<int>(fin);
		type_out = ReadLabeled<int>(fin);
	}
	
	 
	void write(const std::string& file_name, const std::vector<std::pair<std::string, double>>& default_values) const
	{
		std::ofstream fout(file_name);
		
		for(const auto& [name, value]: default_values)
		{
			fout << name << "= " << value << '\n';
		}
	}
};



template <class T>
void Print(std::ostream& out, const std::vector<std::vector<T>>& matrix)
{
	out << '\n';
	
	for(const auto& row: matrix)
	{
		for(const auto& col: row)
		{
			out << col << ' ';
		}
		out << '\n';
	}
}

class Node
{
public:
	int r;
	int c;
	
	Node() = delete;
	Node(int r, int c): r(r), c(c) {}
	
	Node relative(int direction) const 
	{
		switch(direction)
		{
			case 0:
				return up();
			case 1:
				return right();
			case 2:
				return down();
			case 3:
				return left();
			default:
				throw std::invalid_argument("Bad direction");
		}
	}
	
	Node up() const
	{
		return Node(r - 1, c);
	}
	
	Node right() const
	{
		return Node(r, c + 1);
	}
	
	Node down() const
	{
		return Node(r + 1, c);
	}
	
	Node left() const
	{
		return Node(r, c - 1);
	}
	
	bool operator == (const Node& other) const
	{
		return r == other.r && c == other.c;
	}
	
	int linear(int m) const
	{
		return r * (m - 2) + c - 1;
	}
};


class Pressure
{
	void read(std::istream& in, std::vector<double>& v)
	{
		for(auto& x: v)
		{
			in >> x;
		}
	}
	
	double multiplier;
	std::vector<double> vec;
	
	
public:
	Pressure(int n, double multiplier, double value): multiplier(multiplier), vec(n, value) {}
	Pressure(int n, const std::string& file_name): vec(n)
	{
		std::ifstream fin(file_name);
		fin >> multiplier;
		
		read(fin, vec);
	}
		
		
	void write(const std::string& file_name) const 
	{
		std::ofstream fout(file_name);
		fout << multiplier << '\n';
		
		for(const auto& value: vec)
		{
			fout << value << '\n';
		}
	}
	
	double operator [] (int index) const
	{
		return vec[index] * multiplier;
	}
};
		
	
class Tube
{
	typedef std::vector<std::vector<double>> matrix;
	double multiplier;
	matrix ver;
	matrix hor;
	
	matrix Ver(int n, int m, double value = 0)
	{
		return matrix(n - 1, std::vector<double>(m - 2, value));
	}

	matrix Hor(int n, int m, double value = 0)
	{
		return matrix(n, std::vector<double>(m - 1, value));
	}
	
	void read(std::istream& in, matrix& M)
	{
		for(auto& row: M)
		{
			for(auto& col: row)
			{
				in >> col;
			}
		}
	}
	
public:
	Tube(int n, int m, double multiplier = 1, double value = 0): multiplier(multiplier), ver(Ver(n, m, value)), hor(Hor(n, m, value)) {}
	Tube(int n, int m, const std::string& file_name): ver(Ver(n, m)), hor(Hor(n, m))
	{
		std::ifstream fin(file_name);
		fin >> multiplier;
		
		read(fin, ver);
		read(fin, hor);
	}
		
		
	void write(const std::string& file_name) const
	{
		std::ofstream fout(file_name);
		fout << multiplier << '\n';
		
		Print(fout, ver);
		Print(fout, hor);
	}
	
	double& operator () (int row, int col, int direction)
	{
		if(direction == 0)
		{
			return ver[row - 1][col - 1];
		}
		if(direction == 1)
		{
			return hor[row][col];
		}
		if(direction == 2)
		{
			return ver[row][col - 1];
		}
		if(direction == 3)
		{
			return hor[row][col - 1];
		}
		
		throw std::invalid_argument("direction is bad");
	}
	
	double operator () (int row, int col, int direction) const
	{
		if(direction == 0)
		{
			return multiplier * ver[row - 1][col - 1];
		}
		if(direction == 1)
		{
			return multiplier * hor[row][col];
		}
		if(direction == 2)
		{
			return multiplier * ver[row][col - 1];
		}
		if(direction == 3)
		{
			return multiplier * hor[row][col - 1];
		}
		
		throw std::invalid_argument("direction is bad");
	}
	double operator () (const Node& a, const Node& b) const
	{
		if(b == a.up() || b == a.down())
		{
			int col = a.c;
			int row_max = std::max(a.r, b.r);
			int row_min = std::min(a.r, b.r);
			if(row_min < 0 || row_max > int(ver.size()))
			{
				return 0;
			}
			return multiplier * ver[row_min][col - 1];
		}
		if(b == a.right() || b == a.left())
		{
			int row = a.r;
			int col_max = std::max(a.c, b.c);
			int col_min = std::min(a.c, b.c);
			if(col_min < 0 || col_max > int(hor.front().size()))
			{
				return 0;
			}
			return multiplier * hor[row][col_min];
		}
		
		throw std::invalid_argument("Nodes have nothing in common.");
	}
};

void FileWriteAllDefault(int n, int m)
{
	Parameters().write(FILE_PARAMETERS, FILE_PARAMETERS_CONTENTS);
	Pressure(n, 1e3, 10).write(FILE_PRESSURE_IN);
	Pressure(n, 1e3, 1).write(FILE_PRESSURE_OUT);
	Tube(n, m, 0.001, 1).write(FILE_RADIUS);
	Tube(n, m, 1, 1).write(FILE_FILL);
	Tube(n, m, 1e-3, 75).write(FILE_SIGMA);
}

double MakeMatrixCalculateK(double fill, double l, double u1, double u2)
{
	double l1 = std::abs(fill) * l;
	double l2 = l - l1;
	
	if(fill < 0)
	{
		std::swap(l1, l2);
	}
	
	return std::acos(-1) / 8 / (l1 * u1 + l2 * u2);
}

int JumpContributionSignDirection(int direction)
{
	switch(direction)
	{
		case 0:
		return -1;
		case 1:
		return 1;
		case 2:
		return 1;
		case 3:
		return -1;
	}
	throw std::invalid_argument("direction is not good for the function jump contribution");
}

int JumpContributionSignFill(double fill)
{
	if(std::abs(fill) == 1)
	{
		return 0;
	}
	
	if(fill < 0)
	{
		return -1;
	}
	
	return 1;
}


int JumpContributionSign(double fill, int direction)
{
	return JumpContributionSignFill(fill) * JumpContributionSignDirection(direction);
}


std::vector<std::vector<double>> MakeMatrixForGauss(const Parameters& parameters, const Pressure& Pressure_in, const Pressure& Pressure_out, const Tube& Radius, const Tube& Fill, const Tube& Sigma)
{
	const int n = parameters.n;
	const int m = parameters.m;
	const double l = parameters.l;
	const double u1 = parameters.u1;
	const double u2 = parameters.u2;
	const size_t length_equation = n * (m - 2) + 1;
	
	std::vector<std::vector<double>> matrix_for_gauss;
	
	for(int row = 0; row < n; ++ row)
	{
		for(int col = 1; col + 1 < m; ++ col)
		{
			Node node(row, col);
			std::vector<double> equation(length_equation);
			
			for(int direction = 0; direction < 4; ++ direction)
			{
				Node node_other = node.relative(direction);
				
				const double radius = Radius(node, node_other);
				
				if(radius == 0)
				{
					continue;
				}
				
				const double fill = Fill(node, node_other);
				const double sigma = Sigma(node, node_other);
				
				std::cout << "\n\nrow: " << row << ", col: " << col << ", direction: " << direction << ", radius:" << radius << ", sigma: " << sigma << ", fill: " << fill;
				
				const double K = MakeMatrixCalculateK(fill, l, u1, u2);
				const double coeff_pressure = K * std::pow(radius, 4);
				const double coeff_jump = JumpContributionSign(fill, direction) * K * 2 * sigma * std::pow(radius, 3);
				
				equation[node.linear(m)] += coeff_pressure;
				equation.back() -= coeff_jump;
				std::cout << ", K:" << K << ", jump sign = " << JumpContributionSign(fill, direction) << ", dp = " << coeff_pressure << " coeff_jump = " << coeff_jump << '\n';
				if(col == 1 && direction == 3)
				{
					equation.back() += coeff_pressure * Pressure_in[row];
					continue;
				}
				if(col == m - 2 && direction == 1)
				{
					std::cout << " in: " << coeff_pressure * Pressure_out[row] << '\n';
					equation.back() += coeff_pressure * Pressure_out[row];
					continue;
				}
				
				equation[node_other.linear(m)] -= coeff_pressure;
			}
			
			matrix_for_gauss.push_back(equation);
			
			Print(std::cout, matrix_for_gauss);
		}
	}	
	return matrix_for_gauss;
}

std::vector<double> GaussElimination(std::vector<std::vector<double>> M)
{
	const int n = M.front().size() - 1;
	for(int i = 0; i < n; ++ i)
	{
		double divider = M[i][i];
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
			
			double coeff = M[j][i];
			
			for(int k = 0; k <= n; ++ k)
			{
				M[j][k] -= M[i][k] * coeff;
			}
		}
		std::cout << "\n\n----------------\n";
		Print(std::cout, M);
	}
	std::vector<double> v;

	for(auto& row: M)
	{
		v.push_back(row.back());
	}
	
	return v;
}

std::vector<std::vector<double>> MakePressureMatrixFromVector(const std::vector<double>& v, const Pressure& Pressure_in, const Pressure& Pressure_out, int n, int m)
{
	int count = 0;
	std::vector<std::vector<double>> M(n);
	for(int row = 0; row < n; ++ row)
	{
		M[row].push_back(Pressure_in[row]);
		for(int col = 0; col + 2 < m; ++ col)
		{
			M[row].push_back(v[count ++]);
		}
		M[row].push_back(Pressure_out[row]);
	}
	
	return M;
}

void FileWritePressureDistribution(const std::vector<std::vector<double>>& pressure_matrix, const std::string& file_name)
{
	std::ofstream fout(file_name);
	
	Print(fout, pressure_matrix);
}


Tube FindFlowRateInEachTube(const Parameters& parameters, const std::vector<std::vector<double>>& pressures, const Tube& Radius, const Tube& Fill, const Tube& Sigma)
{
	const int n = parameters.n;
	const int m = parameters.m;
	const double l = parameters.l;
	const double u1 = parameters.u1;
	const double u2 = parameters.u2;
	
	Tube flow_rates(n, m);
	
	for(int row = 0; row < n; ++ row)
	{
		for(int col = 0; col < m; ++ col)
		{
			for(int direction = 1; direction <= 2; ++ direction)
			{
				if(direction == 2 && (col == 0 || col == m - 1))
				{
					continue;
				}
				if(direction == 1 && col == m - 1)
				{
					continue;
				}
				if(direction == 2 && row == n - 1)
				{
					continue;
				}
				
				auto node = Node(row, col).relative(direction);
							
				const double radius = Radius(row, col, direction);
				const double fill = Fill(row, col, direction);
				const double sigma = Sigma(row, col, direction);
				
				const double K = MakeMatrixCalculateK(fill, l, u1, u2);
				const double coeff_pressure = K * std::pow(radius, 4) * (pressures[row][col] - pressures[node.r][node.c]);
				
				std::cout << "row: " << row << ", col: " << col << ", direc: " << direction << ", pi " << pressures[row][col] << ", pj " << pressures[node.r][node.c] << " radius: " << radius << " fill: " << fill << " sigma: " << sigma << '\n';
				const double coeff_jump = JumpContributionSign(fill, direction) * K * 2 * sigma * std::pow(radius, 3);
				
				flow_rates(row, col, direction) = coeff_pressure + coeff_jump;
			}
		}
		std::cout << '\n';
	}
	
	return flow_rates;
}
				
				
Tube FindVelocityInEachTube(const Parameters& parameters, const Tube& radius, const Tube& flow)
{
	const int n = parameters.n;
	const int m = parameters.m;

	Tube velocity(n, m);
	
	for(int row = 0; row < n; ++ row)
	{
		for(int col = 0; col < m; ++ col)
		{
			for(int direction = 1; direction <= 2; ++ direction)
			{
				if(direction == 2 && (col == 0 || col == m - 1))
				{
					continue;
				}
				if(direction == 1 && col == m - 1)
				{
					continue;
				}
				if(direction == 2 && row == n - 1)
				{
					continue;
				}
				
				velocity(row, col, direction) = flow(row, col, direction) / (std::acos(-1) * std::pow(radius(row, col, direction), 2));
			}
		}
		//std::cout << '\n';
	}
	
	return velocity;		
}
struct ClosestMeniscus
{
	int row;
	int col;
	int direction;
	double time;
};

ClosestMeniscus ClosestMeniscusCalculate(const Parameters& parameters, const Tube& velocity, const Tube& fill)
{
	const int n = parameters.n;
	const int m = parameters.m;
	const double l = parameters.l;
	
	ClosestMeniscus result;
	result.time = std::numeric_limits<double>::max();
	for(int row = 0; row < n; ++ row)
	{
		for(int col = 0; col < m; ++ col)
		{
			for(int direction = 1; direction <= 2; ++ direction)
			{
				if(direction == 2 && (col == 0 || col == m - 1))
				{
					continue;
				}
				if(direction == 1 && col == m - 1)
				{
					continue;
				}
				if(direction == 2 && row == n - 1)
				{
					continue;
				}
				
				const double f = std::abs(fill(row, col, direction));
				if(f == 1)
				{
					continue;
				}
				
				const double v = velocity(row, col, direction);
				
				const double l1 = f * l;
				const double l2 = l - l1;
				
				const double l_eff = (v > 0 ? l2 : l1);
				const double time = l_eff / v;
				if(time < result.time)
				{
					result.row = row;
					result.col = col;
					result.direction = direction;
					result.time = time;
				}
			}
		}
	}
	
	return result;
}

Tube FillDistributionAfterFirstStep(const Parameters& parameters, const Tube& fill, const Tube& velocity, double time)
{
	const int n = parameters.n;
	const int m = parameters.m;
	const double l = parameters.l;
	
	Tube fill_new(n, m);
	
	for(int row = 0; row < n; ++ row)
	{
		for(int col = 0; col < m; ++ col)
		{
			for(int direction = 1; direction <= 2; ++ direction)
			{
				if(direction == 2 && (col == 0 || col == m - 1))
				{
					continue;
				}
				if(direction == 1 && col == m - 1)
				{
					continue;
				}
				if(direction == 2 && row == n - 1)
				{
					continue;
				}
				
				const double f = std::abs(fill(row, col, direction));
				if(f == 1)
				{
					fill_new(row, col, direction) = fill(row, col, direction);
					continue;
				}
				
				const double v = velocity(row, col, direction);
				const double displacement = v * time;
				const double l1 = f * l;
				double l1_new = l1 + displacement;
				
				if(fill(row, col, direction) < 0)
				{
					l1_new *= -1;
				}
				
				fill_new(row, col, direction) = l1_new / l;
			}
		}
	}
	
	return fill_new;
}


void PlotFillDistribution(const std::string& file_name, const Tube& fill, int n, int m)
{	
	const int max_diamension = std::max(n, m);
	const int number_of_sticks = max_diamension - 1;
	const int size_pixcels_largest_length = 4000;
	const int length_stick = size_pixcels_largest_length / (number_of_sticks + 2);
	const int tube_thickness = length_stick / 10;
	const int tube_length = length_stick - 2 * tube_thickness;
	
	int height_image = (n + 1) * length_stick;
	int width_image = (m + 1) * length_stick;
	
	drw::bmp a(file_name, height_image, width_image);
	
	int draw_row = length_stick;
	for(int row = 0; row < n; ++ row)
	{
		int draw_col = length_stick;
		
		for(int col = 0; col < m; ++ col)
		{
			a.drawRectangle(draw_row, draw_col, draw_row + 2 * tube_thickness, draw_col + 2 * tube_thickness);
			for(int direction = 1; direction <= 2; ++ direction)
			{
				if(direction == 2 && (col == 0 || col == m - 1))
				{
					continue;
				}
				if(direction == 1 && col == m - 1)
				{
					continue;
				}
				if(direction == 2 && row == n - 1)
				{
					continue;
				}
				
				const double f = fill(row, col, direction);
				drw::colour main_draw_colour = drw::grey_dark;
				drw::colour sub_draw_colour = drw::grey_light;
				
				const int displacement = tube_length * std::abs(f);
				if(f < 0)
				{
					std::swap(main_draw_colour, sub_draw_colour);
				}
				
				if(direction == 1)
				{
					int r = draw_row + tube_thickness / 2;
					int c = draw_col + 2 * tube_thickness;
					a.drawRectangle(r, c, r + tube_thickness, c + tube_length, main_draw_colour);
					a.drawRectangle(r, c + displacement, r + tube_thickness, c + tube_length, sub_draw_colour);
				}
				if(direction == 2)
				{
					int c = draw_col + tube_thickness / 2;
					int r = draw_row + 2 * tube_thickness;
					a.drawRectangle(r, c, r + tube_length, c + tube_thickness, main_draw_colour);
					a.drawRectangle(r + displacement, c, r + tube_length, c + tube_thickness, sub_draw_colour);
				}
			
			}
			draw_col += length_stick;
		}
		
		draw_row += length_stick;
	}
}

int main()
{
	Parameters parameters(FILE_PARAMETERS);
	int n = parameters.n;
	int m = parameters.m;
	
	Pressure pressure_in(n, FILE_PRESSURE_IN);
	Pressure pressure_out(n, FILE_PRESSURE_OUT);
	Tube radius(n, m, FILE_RADIUS);
	Tube fill(n, m, FILE_FILL);
	Tube sigma(n, m, FILE_SIGMA);
	
	auto matrix_for_gauss = MakeMatrixForGauss(parameters, pressure_in, pressure_out, radius, fill, sigma);
	auto pressure_vector_solved_by_gauss = GaussElimination(matrix_for_gauss);
	auto pressure_matrix = MakePressureMatrixFromVector(pressure_vector_solved_by_gauss, pressure_in, pressure_out, n, m);
	FileWritePressureDistribution(pressure_matrix, FILE_PRESSURE_DISTRIBUTION);
	
	Tube flow_rate_in_each_tube = FindFlowRateInEachTube(parameters, pressure_matrix, radius, fill, sigma);
	flow_rate_in_each_tube.write(FILE_FLOW_RATES);
	
	Tube velocity_in_each_tube = FindVelocityInEachTube(parameters, radius, flow_rate_in_each_tube);
	velocity_in_each_tube.write(FILE_VELOCITY);
	
	ClosestMeniscus closest_meniscus = ClosestMeniscusCalculate(parameters, velocity_in_each_tube, fill);
	std::cout << "row: " << closest_meniscus.row << ", col: " << closest_meniscus.col << ", direction: " << closest_meniscus.direction << ", time: " << closest_meniscus.time << '\n';
	
	Tube fill_new = FillDistributionAfterFirstStep(parameters, fill, velocity_in_each_tube, closest_meniscus.time);
	fill_new.write(FILE_FILL_AFTER_STEP1);
	
//	fill_new(2, 3, 1) = 0.1;
	//fill_new(2, 3, 0) = -0.9;
	PlotFillDistribution("out_fill_0.bmp", fill, n, m);
	PlotFillDistribution("out_fill_1.bmp", fill_new, n, m);
	
	
	auto matrix_for_gauss2 = MakeMatrixForGauss(parameters, pressure_in, pressure_out, radius, fill_new, sigma);
	auto pressure_vector_solved_by_gauss2 = GaussElimination(matrix_for_gauss2);
	auto pressure_matrix2 = MakePressureMatrixFromVector(pressure_vector_solved_by_gauss2, pressure_in, pressure_out, n, m);
	FileWritePressureDistribution(pressure_matrix2, FILE_PRESSURE_DISTRIBUTION + "2.txt");
	
	Tube flow_rate_in_each_tube2 = FindFlowRateInEachTube(parameters, pressure_matrix2, radius, fill_new, sigma);
	flow_rate_in_each_tube2.write(FILE_FLOW_RATES + "2.txt");
	
	Tube velocity_in_each_tube2 = FindVelocityInEachTube(parameters, radius, flow_rate_in_each_tube2);
	velocity_in_each_tube2.write(FILE_VELOCITY + "2.txt");
	
	
	ClosestMeniscus closest_meniscus2 = ClosestMeniscusCalculate(parameters, velocity_in_each_tube2, fill_new);
	Tube fill_new2 = FillDistributionAfterFirstStep(parameters, fill_new, velocity_in_each_tube2, closest_meniscus2.time);
	fill_new2.write(FILE_FILL_AFTER_STEP1 + "2.txt");
	PlotFillDistribution("out_fill_2.bmp", fill_new2, n, m);
	
	//FileWriteAllDefault(parameters.n, parameters.m);
	
	
	return 0;
}
