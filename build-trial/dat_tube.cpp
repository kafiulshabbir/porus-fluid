#include "dat_tube.h"

namespace dat
{
	cls_tube_frw::cls_tube_frw(const std::string& name_file, int n, double multiplier, double left_default, double right_default)
	{
		this->name_file = name_file;
		this->n = n;
		this->multiplier = multiplier;
		this->left_default = multiplier * left_default;
		this->right_default = multiplier * right_default;
	}
	
	matrix cls_tube_frw::read_matrix(int R, int C, double multiplier, std::ifstream& fin)
	{
		matrix M(R, vector(C));
		for(auto& row: M)
		{
			for(auto& cell: row)
			{
				fin >> cell;
				cell *= multiplier;
			}
		}
		
		return M;
	}
	
		
	void cls_tube_frw::print_matrix(const matrix& M) const
	{
		for(const auto& row: M)
		{
			for(const auto& x: row)
			{
				std::cout << x << ", ";
			}
			std::cout << '\n';
		}
	}
		
	double cls_tube_frw::operator () (const stc::node& first, const stc::node& second) const
	{
		if(first.vertical(second))
		{
			if(second.outside(n))
			{
				return 0;
			}
			
			int starts = std::min(first.row(), second.row());
			return ver[starts][first.col()];
		}
		if(first.horizontal(second))
		{
			if(second.col() < 0)
			{
				return left_default;
			}
			if(second.col() >= n)
			{
				return right_default;
			}
			
			int starts = std::min(first.col(), second.col());
			return hor[first.row()][starts];
		}
		
		std::cout << "porgram failed due to non-adjacent radius access attempt!";
		throw std::invalid_argument("the two nodes are not adjacent to each other, and a tube between them does not exist!");
		return -1;
	}

	bool cls_tube_frw::read_file()
	{
		std::ifstream fin(name_file);
		if(!fin.is_open())
		{
			std::cout << "ERROR: FILE OPEN FAILED: " << name_file << '\n';
			return true;
		}
		
		
		ver = read_matrix(n - 1, n , multiplier, fin);
		hor = read_matrix(n, n - 1, multiplier, fin);
		
		std::cout << "Feedback: " << name_file << " correctly read!\n";
		return false;
	}
	
	void cls_tube_frw::print_data() const
	{
		std::cout << "\n\n-----------\n" << name_file << '\n';
		std::cout << "ver:\n";
		print_matrix(ver);
		std::cout << "hor:\n";
		print_matrix(hor);
	}
}
