#include "func/funcdetermine.h"

Tfloat func::Determine::determine_volume(Tfloat velocity, const Tfloat& radius, const float time_step, const dst::Diamension& diamension)
{
	for(int i = 0; i < diamension.rows; ++ i)
	{
		auto& v = velocity[i];
		for(int j = 0; j < diamension.cols; ++ j)
		{
			v[j] = std::abs(v[j] * std::pow(radius[i][j], 2) * declconst::PI * time_step);
		}
	}
	
	return velocity;
}

