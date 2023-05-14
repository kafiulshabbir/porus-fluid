#include "func/funcpressure.h"

Tfloat func::Pressure::generate_equations_aug_matrix(const Tfloat& radius, const TMns& mnsc, const dst::Diamension& diamension)
{
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
				equation[linear_node] = 1;
				equation.back() = declconst::PRESSURE_TOP;
				continue;
			}
			
			if(row == diamension.rows)
			{
				equation[linear_node] = 1;
				equation.back() = declconst::PRESSURE_BOTTOM;
				continue;
			}
			
			const std::vector<dst::Tube> connections_vec = diamension.generate_tubes_connected_to_node(row, col);
			
			const int total_directions = connections_vec.size();
			for(int direction = 0; direction < total_directions; ++ directions)
			{
				const dst::Tube& connection = connections_vec[direction];
				if(connection.active)
				{
					const float r = radius[connection.row][connection.col];
					const dst::Mns& mns = mnsc[connection.row][connection.col];
					const float scontb = mns.scontb(direction);
					
					const float K = std::pow(r, 3) / mns.mu(declconst::MU1, declconst::MU2);
					equation[linear_node] += r * K;
					equation[connection.linear_node] -= r * K;
					equation.back() -= declconst::SIGMA * 2 * scontb  * K;
				}
			}
		}
	}
	
	return equations_matrix;
}

std::vector<float> func::Pressure::calculate_pressure(const Tfloat& radius, const TMns& mnsc, const dst::Diamension& diamension)
{
	const Tfloat equations = func::Pressure::generate_equations_aug_matrix(radius, mnsc, diamension);
	const std::vector<float> solution_of_equation = math::Linear::gauss_elimination(equations);
	
	return solution_of_equation;
}

