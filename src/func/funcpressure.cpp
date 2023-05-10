#include "func/funcpressure.h"

Tfloat func::Pressure::generate_equations_aug_matrix(const Tfloat& radius, const TMns& mnsc, const dst::Diamension& diamension)
{
	const int total_nodes = diamension.total_nodes();
	Tfloat equations_matrix = diamension.empty_aug_matrix();
	
	for(int row = 0; row <= diamension.rows; ++ row)
	{
		const int nodes_in_this_row = diamension.number_nodes_in_this_row(row);
		
		for(int col = 0; col <= nodes_in_this_row; ++ col)
		{
			const int linear_node = diamension.linear_node_from_coordinate(row, col);
			std::vector<float>& equation = equations_matrix[linear_node];
			if(row == 0)
			{
				e[linear_node] = 1;
				e.back() = declconst::PRESSURE_TOP;
				continue;
			}
			if(row == n)
			{
				e[linear_node] = 1;
				e.back() = declconst::PRESSURE_BOTTOM;
				continue;
			}
			
			const std::vector<dst::Tube> connections_vec = diamension.generate_tubes_connected_to_node(row, col);
			
			for(const dst::Tube& connection: connections_vec)
			{
				if(connection.active)
				{
					const float r = radius[c.r][c.c];
					const dst::Mns& f = mnsc[c.r][c.c];
					const float s = f.scontb(i);
					
					const float K = std::pow(r, 3) / f.mu(declconst::MU1, declconst::MU2);
					e[l] += r * K;
					e[c.p] -= r * K;
					e.back() -= declconst::SIGMA * 2 * s * K;
				}
			}
		}
	}
	
	return equation;
}

std::vector<float> func::Pressure::calculate_pressure(const Tfloat& radius, const TMns& mnsc, const dst::Diamension& diamension)
{
	const Tfloat equations = func::Pressure::generate_equations_aug_matrix(radius, mnsc, diamension);
	const std::vector<float> solution_of_equation = math::Linear::gauss_elimination(equations);
	
	return solution_of_equation;
}

