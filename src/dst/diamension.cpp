#include "dst/diamension.h"

Diamension::Diamension(const int rows, const int cols):
	rows(rows), cols(cols) {}

Diamension::Diamension(): rows(0), cols(0) {}

std::pair<int, int> Diamension::connection_ends(int r, int c, int m)
{
	return {linear_nodes(r, c/2 + 1 - (r % 2), m), linear_nodes(r + 1, c/2 + (r % 2), m)};
}

int Diamension::total_nodes(int n, int m)
{
	return ((n + 1) * (m + 1) + 1) / 2;
}

int Diamension::linear_nodes(int i, int j, int m)
{
	return (i * (m + 1) + (i % 2)) / 2 + j;
}

bool Diamension::operator== (const Diamension& other) const
{
	return (rows == other.rows) && (cols == other.cols);
}
