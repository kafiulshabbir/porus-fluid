#include "funcpressure.h"

Tdouble func::Pressure::generate_equations_aug_matrix(
	const Tdouble& radius,
	const TMns& mnsc,
	const std::vector<std::vector<int>>& add_mnsc,
	const dst::Diamension& diamension
)
{
	Tdouble equations_matrix = diamension.empty_aug_matrix();

	for(int row = 0; row < diamension.node_rows(); ++ row)
	{
		for(int col = 0; col < diamension.node_cols(row); ++ col)
		{
			const int linear_node = diamension.linear_node_from_coordinate(row, col);
			std::vector<double>& equation = equations_matrix[linear_node];

			if(diamension.is_this_an_open_node(row, col))
			{
				equation[linear_node] = 1;
				equation.back() = ((col == 0) ? declconst::PRESSURE_LEFT : declconst::PRESSURE_RIGHT);
				continue;
			}

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

					const int sign_of_capll_pressure
						= calc_sign_capll_pressure(
							mns.sign_of_capll_pressure(direction),
							add_mnsc[connection.row][connection.col],
							direction
						);


					const double K = std::pow(r, 3) / mns.mu(declconst::MU1, declconst::MU2);

					equation[linear_node] += r * K;
					equation[connection.linear_node] -= r * K;
					equation.back() -= declconst::SIGMA * 2 * sign_of_capll_pressure  * K;

				}
			}
		}
	}

	//std::cout << "can-readh-here-240425" << std::endl;

	// The special adder is activated for an open system.
	/*
	const double adder = 1;
	const int special_row = diamension.rows / 2;
	const int special_col = diamension.cols / 4;
	const int special_linear = diamension.linear_node_from_coordinate(
		special_row, special_col);

	for(int row = 0; row < diamension.total_nodes(); ++ row)
	{
		equations_matrix[row][special_linear] += adder;
	}
	*/

	return equations_matrix;
}

std::vector<double> func::Pressure::calculate_pressure(
	const Tdouble& radius,
	const TMns& mnsc,
	const std::vector<std::vector<int>>& add_mnsc,
	const dst::Diamension& diamension
)
{
	const Tdouble equations = func::Pressure::generate_equations_aug_matrix(radius, mnsc, add_mnsc, diamension);

	//cmdio::Print::pmat("set-of-linear-equations", equations);

	const std::vector<double> solution_of_equation = math::Linear::gauss_elimination(equations);
	return solution_of_equation;
}

int func::Pressure::calc_sign_capll_pressure(
	const int internal,
	const int add_table,
	const int direction
)
{
	int add = add_table;
	if(direction > 1)
	{
		add *= -1;
	}
	return internal + add;
}
