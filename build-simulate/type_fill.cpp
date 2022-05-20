#include "type_fill.h"

namespace typ
{
	type_fill::type_fill(const std::string& name_file, int n): name_file(name_file), n(n) {}

	bool type_fill::read_file()
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
	
	double type_fill::operator() (const node& node_i, const node& node_j)
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

