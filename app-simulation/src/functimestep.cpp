#include "func/functimestep.h"

double func::TimeStep::decide_time_step(const Tdouble& velocity, const dst::Diamension& diamension)
{
	double max_vel = 0;

	for(int row = 0; row < diamension.rows; ++ row)
	{
		for(int col = 0; col < diamension.cols; ++ col)
		{
			max_vel = std::max(std::abs(velocity[row][col]), max_vel);
		}
	}
	
	return declconst::TUBE_LENGTH / max_vel / declconst::TIME_DIV;
}
