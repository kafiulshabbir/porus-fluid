#include "func/funcconnection.h"

int func::Connection::linear_nodes(int i, int j, int m)
{
	return (i * (m + 1) + (i % 2)) / 2 + j;
}

std::pair<int, int> func::Connection::connection_ends(int r, int c, int m)
{
	return {linear_nodes(r, c / 2 + (c % 2) * ((r + 1) % 2), m), linear_nodes(r + 1, c / 2 + (c % 2) * (r % 2), m)};
}

int func::Connection::total_nodes(int n, int m)
{
	return ((n + 1) * (m + 1) + 1) / 2;
}

std::vector<func::Connections> func::Connection::FGenConnectionsEqu(int r, int c, int n, int m)
{
	const auto p = linear_nodes(r, c, m);
	std::vector<func::Connections> v
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

int func::Connection::FCountConnections(const std::vector<Connections>& connections)
{
	int count = 0;
	
	for(const auto& connection: connections)
	{
		count += connection.a;
	}
	
	return count;
}
