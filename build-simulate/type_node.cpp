#include "type_node.h"
namespace typ
{
	node::node(int row, int col): row(row), col(col) {}

	node node::up() const
	{
		return node(row - 1, col);
	}

	node node::down() const
	{
		return node(row + 1, col);
	}

	node node::left() const
	{
		return node(row, col - 1);
	}

	node node::right() const
	{
		return node(row, col + 1);
	}

	node node::rel(int u) const
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
		
		return node(-1, -1);
	}

	int node::linear(int n) const
	{
		return n * row + col;
	}

	bool node::vertical(const node& other) const
	{
		return col == other.col;
	}

	bool node::horizontal(const node& other) const
	{
		return row == other.row;
	}
}
