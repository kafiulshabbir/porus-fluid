#include "stc_node.h"
namespace stc
{
	node::node(int r, int c): r(r), c(c) {}

	node node::up() const
	{
		return node(r - 1, c);
	}

	node node::down() const
	{
		return node(r + 1, c);
	}

	node node::left() const
	{
		return node(r, c - 1);
	}

	node node::right() const
	{
		return node(r, c + 1);
	}

	node node::rel(int direction) const
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
		}
		
		std::cout << "An invalid direction was asked for!";
		throw std::invalid_argument("relative position is not correct!");
		
		return node(-1, -1);
	}

	int node::linear(int n) const
	{
		return n * r + c;
	}

	bool node::vertical(const node& other) const
	{
		return c == other.c;
	}

	bool node::horizontal(const node& other) const
	{
		return r == other.r;
	}
	
	bool node::outside(int n) const
	{
		return outside(r, n) || outside(c, n);
	}
	
	bool node::outside(int m, int n) const
	{
		return m < 0 || m >= n;
	}
	
	int node::row() const
	{
		return r;
	}
	
	int node::col() const
	{
		return c;
	}
}
