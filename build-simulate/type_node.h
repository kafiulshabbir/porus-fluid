#ifndef DEF_TYPE_NODE
#define DEF_TYPE_NODE

#include "project_includes.h"

namespace typ
{
	struct node
	{
		int row;
		int col;
		
		node(int, int);
		node up() const;
		node down() const;
		node left() const;
		node right() const;
		node rel(int u) const;
		int linear(int n) const;
		bool vertical(const node& other) const;
		bool horizontal(const node& other) const;
	};
}
#endif
