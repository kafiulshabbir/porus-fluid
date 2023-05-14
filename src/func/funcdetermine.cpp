#include "func/funcdetermine.h"

Tfloat func::Determine::determine_volume
(
	const Tfloat& radius,
	const Tfloat& velocity,
	const float time_step,
	const dst::Diamension& diamension
)
{
	Tfloat volume = diamension.empty_table();
	
	for(int row = 0; row < diamension.rows; ++ row)
	{
		for(int col = 0; col < diamension.cols; ++ col)
		{
			const float vel_abs = std::abs(velocity[row][col]);
			const float area_tube = std::pow(radius[row][col], 2) * declconst::PI
			volume[row][col] = vel_abs * area_tube * time_step;
		}
	}
	
	return volume;
}

