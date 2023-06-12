#include "func/funcpressure.h"

Tdouble func::Pressure::generate_equations_aug_matrix(const Tdouble& radius, const TMns& mnsc, const dst::Diamension& diamension)
{
	Tdouble equations_matrix = diamension.empty_aug_matrix();

	for(int row = 0; row < diamension.node_rows(); ++ row)
	{		
		for(int col = 0; col < diamension.node_cols(row); ++ col)
		{
			const int linear_node = diamension.linear_node_from_coordinate(row, col);
			std::vector<double>& equation = equations_matrix[linear_node];
			
			const std::vector<dst::Tube> tubes_connected_vec =
				diamension.generate_tubes_connected_to_node(row, col);
			
			const int total_directions = tubes_connected_vec.size();
			for(int direction = 0; direction < total_directions; ++ direction)
			{
				const dst::Tube& connection = tubes_connected_vec[direction];
				if(connection.active)
				{
					const double r = radius[connection.row][connection.col];
					const dst::Mns& mns = mnsc[connection.row][connection.col];
					const double sign_of_capll_pressure = mns.sign_of_capll_pressure(direction);
					
					const double K = std::pow(r, 3) / mns.mu(declconst::MU1, declconst::MU2);
					
					equation[linear_node] += r * K;
					equation[connection.linear_node] -= r * K;
					equation.back() -= declconst::SIGMA * 2 * sign_of_capll_pressure  * K;

				}
			}
		}
	}
	
	const double adder = 1;
	const int special_row = diamension.rows / 2;
	const int special_col = diamension.cols / 4;
	const int special_linear = diamension.linear_node_from_coordinate(
		special_row, special_col);
		
	for(int row = 0; row < diamension.total_nodes(); ++ row)
	{
		equations_matrix[row][special_linear] += adder;
	}
	
	return equations_matrix;
}

std::vector<double> func::Pressure::calculate_pressure(const Tdouble& radius, const TMns& mnsc, const dst::Diamension& diamension)
{
	const Tdouble equations = func::Pressure::generate_equations_aug_matrix(radius, mnsc, diamension);
	const std::vector<double> solution_of_equation = math::Linear::gauss_elimination(equations);
	return solution_of_equation;
}

