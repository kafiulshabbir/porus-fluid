#include "func/functimestep.h"

func::TimeStepResult func::TimeStep::decide_time_step(const TMns& mnsc, const Tfloat& velocity, const dst::Diamension& diamension)
{
	Tfloat timev = diamension.empty_table();
	
	float time_min = declconst::HUGE;
	
	int row_min = -1;
	int col_min = -1;
	
	
	for(int row = 0; row < diamension.rows; ++ row)
	{
		for(int col = 0; col < diamension.cols; ++ col)
		{
			float& time_ref = timev[row][col];
			time_ref = mnsc[row][col].time(velocity[row][col], declconst::TUBE_LENGTH, declconst::TIME_DIV);
			
			if(time_ref < time_min)
			{
				time_min = time_ref;
				row_min = row;
				col_min = col;
			}
		}
	}
	
	return func::TimeStepResult{row_min, col_min, timev};
}
	
const float func::TimeStepResult::time_step() const
{
	return this->timev[this->row][this->col];
}
