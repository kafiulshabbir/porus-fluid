#ifndef DSTDIAMENSION_H
#define DSTDIAMENSION_H

#include "decl/decltypedef.h"
#include <vector>

namespace dst
{
	class Diamension
	{
	public:
		int rows;
		int cols;
		
		Diamension();
		Diamension(const int rows, const int cols);
		
		template<class T>
		Diamension(const std::vector<std::vector<T>>& table):
			rows(table.size()), cols(table.front().size()) {}
		
		
		std::pair<int, int> connection_ends(int r, int c, int m);
		int total_nodes(int n, int m);
		int linear_nodes(int i, int j, int m);
		bool operator== (const Diamension& other) const;
	};
}

#endif
