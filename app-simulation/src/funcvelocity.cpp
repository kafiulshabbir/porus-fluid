#include "func/funcvelocity.h"


Tdouble func::Velocity::calculate_velocity
(
	const Tdouble& radius,
	const TMns& mnsc,
	const std::vector<std::vector<int>>& add_mnsc,
	const std::vector<double>& pressure,
	const dst::Diamension& diamension
)
{
	Tdouble velocity = diamension.empty_table();
	for(int row = 0; row < diamension.rows; ++ row)
	{
		for(int col = 0; col < diamension.cols; ++ col)
		{
			const std::pair<int, int> linear_nodes_pair = diamension.linear_node_at_ends_of_tube(row, col);
			
			
			const double delp = pressure[linear_nodes_pair.second] - pressure[linear_nodes_pair.first];
			const double rad = radius[row][col];
			const double mu = mnsc[row][col].mu(declconst::MU1, declconst::MU2);
			const double sign_of_capll_pressure = mnsc[row][col].sign_of_capll_pressure(0) + add_mnsc[row][col];
			velocity[row][col] = rad / 8 / mu / declconst::TUBE_LENGTH * (delp * rad + sign_of_capll_pressure * 2 * declconst::SIGMA);
			
			//std::cout << "tr=" << row << " tc=" << col << " up=" << linear_nodes_pair.second
			//	<< " dn=" << linear_nodes_pair.first << " dp=" << delp << " r=" << rad
			//	<< " sig= " << sign_of_capll_pressure << " v=" << velocity[row][col] << std::endl;
		}
		
		//std::cout << std::endl;
	}
	
	return refine_velocity(velocity);
}

Tdouble func::Velocity::refine_velocity(Tdouble v)
{
	double max = -1;
	for(const auto& row: v)
	{
		for(auto x: row)
		{
			max = std::max(max, std::abs(x));
		}
	}
	
	max /= declconst::IGNORE_VEL;
	for(auto& row: v)
	{
		for(auto &x: row)
		{
			if(std::abs(x) < max)
			{
				x = 0;
			}
		}
	}
	
	return v;
}
