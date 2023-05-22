#include "func/functimestep.h"

func::TimeStepResult func::TimeStep::decide_time_step(const TMns& mnsc, const Tfloat& velocity, const dst::Diamension& diamension)
{
	Tfloat timev = diamension.empty_table();
	
	float time_min = declconst::HUGE;
	
	int row_min = -1;
	int col_min = -1;
	bool does_mns_reach_node = false;
	
	for(int row = 0; row < diamension.rows; ++ row)
	{
		for(int col = 0; col < diamension.cols; ++ col)
		{
			//std::cout << "(row, col)=(" << row << ", " << col << ") : ";
			float& time_ref = timev[row][col];
			
			const dst::Mns::TimeType time_type =
				mnsc[row][col].time(velocity[row][col],
				declconst::TUBE_LENGTH, declconst::TIME_DIV);
			
			time_ref = time_type.time;
			if(time_ref < time_min)
			{
				time_min = time_ref;
				row_min = row;
				col_min = col;
				does_mns_reach_node = time_type.does_mns_reach_node;
			}
		}
	}
	
	return func::TimeStepResult{row_min, col_min, does_mns_reach_node, timev};
}
	
const float func::TimeStepResult::time_step() const
{
	return this->timev[this->row][this->col];
}
