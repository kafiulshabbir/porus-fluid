#ifndef DSTDIAMENSION_H
#define DSTDIAMENSION_H

#include "decl/decltypedef.h"

namespace dst
{
	struct Tube
	{
		bool a = true;
		int r;
		int c;
		int p;	
	};
	
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
		
		
		std::pair<int, int> linear_node_at_ends_of_tube(const int r, const int c) const;
		int total_nodes() const;
		int linear_node_from_coordinate(const int i, const int j) const;
		bool operator== (const Diamension& other) const;
		
		static int count_active_tubes(const std::vector<dst::Tube>& connections);
		std::vector<dst::Tube> generate_tubes_connected_to_node(int r, int c) const;
		
		Tfloat empty_table() const;
	};
}

#endif
