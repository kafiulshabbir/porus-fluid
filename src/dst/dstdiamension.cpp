#include "dst/dstdiamension.h"


dst::Diamension::Diamension(const int rows, const int cols):
	rows(rows), cols(cols) {}


dst::Diamension::Diamension(): rows(0), cols(0) {}


std::pair<int, int> dst::Diamension::linear_node_at_ends_of_tube(const int row, const int col) const
{
	const int first_linear_node = this->linear_node_from_coordinate(row, col / 2 + (col % 2) * ((row + 1) % 2));
	const int second_linear_node = this->linear_node_from_coordinate(row + 1, col / 2 + (col % 2) * (row % 2));
	return {first_linear_node, second_linear_node};
}

int dst::Diamension::node_rows() const
{
	return this->rows + 1;
}

int dst::Diamension::node_cols(const int row) const
{
	return this->cols / 2 - (row % 2) + 1;
}

int dst::Diamension::total_nodes() const
{
	return ((this->rows + 1) * (this->cols + 1) + 1) / 2;
}

int dst::Diamension::linear_node_from_coordinate(const int row, const int col) const
{
	return (row * (this->cols + 1) + (row % 2)) / 2 + col;
}


bool dst::Diamension::operator== (const Diamension& other) const
{
	return (this->rows == other.rows) && (this->cols == other.cols);
}


std::vector<dst::Tube> dst::Diamension::generate_tubes_connected_to_node(int row, int col) const
{	
	const auto linear_node = this->linear_node_from_coordinate(row, col);

	std::vector<dst::Tube> tubes_connected_vec(4);
	
	tubes_connected_vec[0].row = row - 1;
	tubes_connected_vec[0].col = 2 * col - 1 + row % 2;
	tubes_connected_vec[0].linear_node = linear_node - cols / 2 - 1;
	
	tubes_connected_vec[1].row = row - 1;
	tubes_connected_vec[1].col = 2 * col + row % 2;
	tubes_connected_vec[1].linear_node = linear_node - cols / 2 ;
	
	tubes_connected_vec[2].row = row;
	tubes_connected_vec[2].col = 2 * col + row % 2;
	tubes_connected_vec[2].linear_node = linear_node + cols / 2 + 1;
	
	tubes_connected_vec[3].row = row;
	tubes_connected_vec[3].col = 2 * col - 1 + row % 2;
	tubes_connected_vec[3].linear_node = linear_node + cols / 2;

	
	if(row % 2)
	{
		return tubes_connected_vec;
	}
	
	if(row == 0)
	{
		tubes_connected_vec[0].active = false;
		tubes_connected_vec[1].active = false;
	}
	if(col == 0)
	{
		tubes_connected_vec[0].active = false;
		tubes_connected_vec[3].active = false;
	}
	if(2 * col == this->cols)
	{
		tubes_connected_vec[1].active = false;
		tubes_connected_vec[2].active = false;
	}
	if(row == this->rows)
	{
		tubes_connected_vec[2].active = false;
		tubes_connected_vec[3].active = false;
	}
	
	return tubes_connected_vec;
}


int dst::Diamension::count_active_tubes(const std::vector<dst::Tube>& connections)
{
	int count = 0;
	
	for(const auto& connection: connections)
	{
		count += connection.active;
	}
	
	return count;
}

Tfloat dst::Diamension::empty_table() const
{
	return empty_table(this->rows, this->cols);
}

Tfloat dst::Diamension::empty_table(const int rows, const int cols) const
{
	return Tfloat(rows, std::vector<float>(cols));
}

Tfloat dst::Diamension::empty_aug_matrix() const
{
	const int total_num_nodes = this->total_nodes();
	return empty_table(total_num_nodes, total_num_nodes + 1);
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
