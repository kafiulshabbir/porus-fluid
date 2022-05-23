#include "dat_fill.h"

namespace dat
{
	cls_fill::cls_fill(const std::string& name_file, int n): name_file(name_file), n(n) {}
	
	int cls_fill::good_begin(int a, int b) const
	{
		return std::min(a, b);
	}
	
	bool cls_fill::read_file()
	{
		std::ifstream fin(name_file);
		
		if(!fin.is_open())
		{
			FileOpenFailure(name_file);
			return true;
		}
		
		ver = ReadMatrix(n - 1, n, fin);
		hor = ReadMatrix(n, n - 1, fin);
		
		std::cout << name_file << " correctly read!\n";
		return false;
	}
	
	double cls_fill::find(const node& node_i, const node& node_j) const
	{
		if(node_i.vertical(node_j))
		{
			if(node_j.outside_matrix(n))
			{
				return 0;
			}
			
			int begin = good_begin(node_i.row, node_j.row);
			std::cout << "accessing vertical: " << ver[begin][node_i.col] << " r: " << begin << " c: " << node_i.col << '\n';
			return ver[begin][node_i.col];
		}
		if(node_i.horizontal(node_j))
		{
			if(node_j.outside_matrix(n))
			{
				return 0;
			}
			
			int begin = good_begin(node_i.col, node_j.col);
			std::cout << "accessing horizontal: " << hor[node_i.row][begin] << " r: " << node_i.row << " c: " << begin << '\n';
			return hor[node_i.row][begin];
		}
		
		std::cout << "porgram failed due to non-adjacent radius access attempt!";
		throw std::invalid_argument("the two nodes are not adjacent to each other, and a tube between them does not exist!");
		return -1;
	}
	
	double cls_fill::operator() (const node& node_i, const node& node_j) const
	{
		return std::abs(find(node_i, node_j));
	}
	
	double cls_fill::sign(const node& node_i, const node& node_j) const
	{
		double x = find(node_i, node_j);
		if(x < 0)
		{
			return -1;
		}
		return 1;
	}
}

