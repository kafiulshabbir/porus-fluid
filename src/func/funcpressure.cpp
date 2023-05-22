#include "func/funcpressure.h"

Tfloat func::Pressure::generate_equations_aug_matrix(const Tfloat& radius, const TMns& mnsc, const dst::Diamension& diamension)
{
	Tfloat equations_matrix = diamension.empty_aug_matrix();
	
	for(int row = 0; row < diamension.node_rows(); ++ row)
	{	
		//const int num_cols = diamension.node_cols(row);
		//std::cout << std::endl << "row: " << row << ", has cols: " << num_cols << std::endl;
		
		for(int col = 0; col < diamension.node_cols(row); ++ col)
		{
			//std::cout << "(r,c): (" << row << ", " << col << "), lin: " << diamension.linear_node_from_coordinate(row, col) << std::endl;	
			
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
			
			const std::vector<dst::Tube> tubes_connected_vec = diamension.generate_tubes_connected_to_node(row, col);
			
			const int total_directions = tubes_connected_vec.size();
			for(int direction = 0; direction < total_directions; ++ direction)
			{
				const dst::Tube& connection = tubes_connected_vec[direction];
				if(connection.active)
				{
					//std::cout << "connection at direction: " << direction << ", row = " << connection.row << ", col = " << connection.col << ", linear_prnode = " << connection.linear_node << std::endl;
					
					const float r = radius[connection.row][connection.col];
					const dst::Mns& mns = mnsc[connection.row][connection.col];
					const float sign_of_capll_pressure = mns.sign_of_capll_pressure(direction);
					
					const float K = std::pow(r, 3) / mns.mu(declconst::MU1, declconst::MU2);
					
					equation[linear_node] += r * K;
					equation[connection.linear_node] -= r * K;
					equation.back() -= declconst::SIGMA * 2 * sign_of_capll_pressure  * K;
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

