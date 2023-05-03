#include "func/functimestep.h"

func::TimeStepResult func::TimeStep::decide_time_step(const TMns& mnsc, const Tfloat& velocity, const dst::Diamension& diamension)
{
	Tfloat timev = diamension.empty_table();
	
	float time_min = declconst::HUGE;
	
	int row_min = -1;
	int col_min = -1;
	
	
	for(int i = 0; i < diamension.rows; ++ i)
	{
		for(int j = 0; j < diamension.cols; ++ j)
		{
			float& time_ref = timev[i][j];
			time_ref = mnsc[i][j].time(velocity[i][j], declconst::TUBE_LENGTH, declconst::TIME_DIV);
			
			if(time_ref < time_min)
			{
				time_min = time_ref;
				row_min = i;
				col_min = j;
			}
		}
	}
	
	return func::TimeStepResult{row_min, col_min, timev};
}
	
