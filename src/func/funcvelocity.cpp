#include "func/funcvelocity.h"


Tfloat func::Velocity::calculate_velocity
(
	const Tfloat& radius,
	const TMns& mnsc,
	const std::vector<float>& pressure,
	const dst::Diamension& diamension
)
{
	Tfloat velocity = diamension.empty_table();
	for(int row = 0; row < diamension.rows; ++ row)
	{
		for(int col = 0; col < diamension.cols; ++ col)
		{
			const std::pair<int, int> linear_nodes_pair = diamension.linear_node_at_ends_of_tube(row, col);
			const float delp = pressure[linear_nodes_pair.second] - pressure[linear_nodes_pair.first];
			const float rad = radius[row][col];
			const float mu = mnsc[row][col].mu(declconst::MU1, declconst::MU2);
			const float sign_of_capll_pressure = mnsc[row][col].sign_of_capll_pressure(0);
			velocity[row][col] = rad / 8 / mu / declconst::TUBE_LENGTH * (delp * rad + sign_of_capll_pressure * 2 * declconst::SIGMA);
		}
	}
	
	return velocity;
}
