#ifndef DEF_STC_NODE
#define DEF_STC_NODE

#include "dec.h"

namespace stc
{
	class node
	{
		int r;
		int c;
		bool outside(int r, int n) const;
		
	public:
		node() = delete;
		node(int row, int col);
		
		int row() const;
		int col() const;
		node up() const;
		node down() const;
		node left() const;
		node right() const;
		node rel(int direction) const;
		int linear(int n) const;
		bool vertical(const node& other) const;
		bool horizontal(const node& other) const;
		bool outside(int n) const;
		
	};
}
#endif
