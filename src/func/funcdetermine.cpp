#include "func/funcdetermine.h"

Tdouble func::Determine::determine_volume
(
	const Tdouble& radius,
	const Tdouble& velocity,
	const double time_step,
	const dst::Diamension& diamension
)
{
	Tdouble volume = diamension.empty_table();
	
	for(int row = 0; row < diamension.rows; ++ row)
	{
		for(int col = 0; col < diamension.cols; ++ col)
		{
			const double vel_abs = std::abs(velocity[row][col]);
			const double area_tube = std::pow(radius[row][col], 2) * declconst::PI;
			volume[row][col] = vel_abs * area_tube * time_step;
		}
	}
	
	return volume;
}

