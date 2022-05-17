#ifndef DEF_TYPE_RADIUS
#define DEF_TYPE_RADIUS

#include "project_includes.h"

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



#endif
