#include "dst/dstdiamension.h"


dst::Diamension::Diamension(const int rows, const int cols):
	rows(rows), cols(cols) {}


dst::Diamension::Diamension(): rows(0), cols(0) {}


std::pair<int, int> dst::Diamension::linear_node_at_ends_of_tube(const int r, const int c) const
{
	const int first_linear_node = this->linear_node_from_coordinate(r, c / 2 + (c % 2) * ((r + 1) % 2));
	const int second_linear_node = this->linear_node_from_coordinate(r + 1, c / 2 + (c % 2) * (r % 2));
	return {first_linear_node, second_linear_node};
}


int dst::Diamension::total_nodes() const
{
	const int n = this->rows;
	const int m = this->cols;
	
	return ((n + 1) * (m + 1) + 1) / 2;
}


int dst::Diamension::linear_node_from_coordinate(const int i, const int j) const
{
	const int m = this->cols;
	return (i * (m + 1) + (i % 2)) / 2 + j;
}


bool dst::Diamension::operator== (const Diamension& other) const
{
	return (this->rows == other.rows) && (this->cols == other.cols);
}


std::vector<dst::Tube> dst::Diamension::generate_tubes_connected_to_node(int r, int c) const
{
	const int n = this->rows;
	const int m = this->cols;
	
	const auto p = this->linear_node_from_coordinate(r, c);
	std::vector<dst::Tube> v
	{
		{true, r - 1, 2 * c - 1 + r % 2, p - m / 2 - 1},
		{true, r - 1, 2 * c - 0 + r % 2, p - m / 2 - 0},
		{true, r - 0, 2 * c - 0 + r % 2, p + m / 2 + 1},
		{true, r - 0, 2 * c - 1 + r % 2, p + m / 2 + 0}
	};
	
	if(r % 2)
	{
		return v;
	}
	
	if(r == 0)
	{
		v[0].a = false;
		v[1].a = false;
	}
	if(c == 0)
	{
		v[0].a = false;
		v[3].a = false;
	}
	if(2 * c == m)
	{
		v[1].a = false;
		v[2].a = false;
	}
	if(r == n)
	{
		v[2].a = false;
		v[3].a = false;
	}
	
	return v;
}


int dst::Diamension::count_active_tubes(const std::vector<dst::Tube>& connections)
{
	int count = 0;
	
	for(const auto& connection: connections)
	{
		count += connection.a;
	}
	
	return count;
}

Tfloat dst::Diamension::empty_table() const
{
	return Tfloat(this->rows, std::vector<float>(this->cols));
}

/*
int network::Connection::linear_node_from_coordinate(int i, int j, int m)
{
	return (i * (m + 1) + (i % 2)) / 2 + j;
}

std::pair<int, int> network::Connection::linear_node_at_ends_of_tube(int r, int c, int m)
{
	return {linear_node_from_coordinate(r, c / 2 + (c % 2) * ((r + 1) % 2), m), linear_node_from_coordinate(r + 1, c / 2 + (c % 2) * (r % 2), m)};
}

int network::Connection::total_nodes(int n, int m)
{
	return ((n + 1) * (m + 1) + 1) / 2;
}
*/
