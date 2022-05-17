#include "type_radius.h"
	
point::point(int row, int col): row(row), col(col) {}

point point::up() const
{
	return point(row - 1, col);
}

point point::down() const
{
	return point(row + 1, col);
}

point point::left() const
{
	return point(row, col - 1);
}

point point::right() const
{
	return point(row, col + 1);
}

point point::rel(int u) const
{
	switch(u)
	{
		case 0:
			return up();
		
		case 1:
			return right();
		
		case 2:
			return down();
		
		case 3:
			return left();
	}
	
	throw std::invalid_argument("relative position is not correct!");
	
	return point(-1, -1);
}

int point::linear(int n) const
{
	return n * row + col;
}

bool point::vertical(const point& other) const
{
	return col == other.col;
}

bool point::horizontal(const point& other) const
{
	return row == other.row;
}
	
double type_radius::fourth(double x) const
{
	return std::pow(x, 4);
}

void type_radius::error(const std::string& message) const
{
	std::cout << name_file << " " << message << ", please run " << tool_generate_radius_distribution << ", and restart program.";
}

void type_radius::multiply(matrix& w, double m)
{
	for(auto& row: w)
	{
		for(auto& cell: row)
		{
			cell *= m;
		}
	}
}

bool type_radius::outside_matrix(int m) const
{
	return m < 0 || m >= n;
}

bool type_radius::outside_matrix(const point& node) const
{
	return outside_matrix(node.row) || outside_matrix(node.col);
}

int type_radius::good_begin(int a, int b) const
{
	return std::min(a, b);
}

double type_radius::extract_radius(const point& node_i, const point& node_j) const
{
	if(node_i.vertical(node_j))
	{
		if(outside_matrix(node_j))
		{
			return 0;
		}
		
		int begin = good_begin(node_i.row, node_j.row);
		return ver[begin][node_i.col];
	}
	if(node_i.horizontal(node_j))
	{
		if(outside_matrix(node_j))
		{
			return leak_radius;
		}
		
		int begin = good_begin(node_i.col, node_j.col);
		return hor[node_i.row][begin];
	}
	
	std::cout << "porgram failed due to non-adjacent radius access attempt!";
	throw std::invalid_argument("the two nodes are not adjacent to each other, and a tube between them does not exist!");
	return -1;
}

type_radius::type_radius(const std::string& name_file): name_file(name_file) {}

bool type_radius::read_file()
{
	std::ifstream fin(name_file);
	if(!fin.is_open())
	{
		error("does not exist");
		return true;
	}
	
	std::string s;
	fin >> s >> n;
	
	if(s != radius_distribution_txt_first_line)
	{
		error("has incorrect format");
		return true;
	}
	
	double multiplier = ReadNamedData(fin);
	leak_radius = multiplier * ReadNamedData(fin);
	
	
	ver = ReadMatrix(n - 1, n, fin);
	hor = ReadMatrix(n - 1, n, fin);
	
	multiply(ver, multiplier);
	multiply(hor, multiplier);
	
	return false;
}

double type_radius::operator () (const point& node_i, const point& node_j) const
{
	return fourth(extract_radius(node_i, node_j));
}

int type_radius::N() const
{
	return n;
}
