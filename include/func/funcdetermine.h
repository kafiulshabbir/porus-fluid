#ifndef FUNCDETERMINE_H
#deifne FUNCDETERMINE_H

class Determine
{
	
	
	
public:

	
std::vector<float> FCalcPressure(const Tfloat& radius, const TMns& dst::Mns)
{
	//std::cout << "okay-gauss Fclac pres" << std::endl;
	return FGaussElimination(FGenEquForGauss(radius, dst::Mns));
}



Tfloat FCalcVelocity(const std::vector<float>& pressure, const Tfloat& radius, const TMns& dst::Mns)
{
	const int n = radius.size();
	const int m = radius.front().size();
	auto velocity = radius;
	for(int i = 0; i < n; ++ i)
	{
		for(int j = 0; j < m; ++ j)
		{
			const auto locs = connection_ends(i, j, m);
			const auto delp = pressure[locs.second] - pressure[locs.first];
			const auto& r = radius[i][j];
			const auto& mu = dst::Mns[i][j].mu(MU1, MU2);
			const auto& s = dst::Mns[i][j].scontb(0);
			velocity[i][j] = r / 8 / mu / TUBE_LENGTH * (delp * r + s * 2 * SIGMA);
		}
	}
	
	return velocity;
}
				
Tfloat FCalcVolume(Tfloat velocity, const Tfloat& radius, const float time_step)
{
	for(int i = 0; i < velocity.size(); ++ i)
	{
		auto& v = velocity[i];
		for(int j = 0; j < v.size(); ++ j)
		{
			v[j] = std::abs(v[j] * std::pow(radius[i][j], 2) * PI * time_step);
		}
	}
	
	return velocity;
}

float FDetermineTimeStep(const TMns& dst::Mns, const Tfloat& velocity)
{
	float min_time = HUGE;
	for(int i = 0; i < dst::Mns.size(); ++ i)
	{
		for(int j = 0; j < dst::Mns[i].size(); ++ j)
		{
			
			const float temp_time = dst::Mns[i][j].time(velocity[i][j], TUBE_LENGTH, TIME_DIV);
			min_time = std::min(temp_time, min_time);
		}
	}
	
	return min_time;
}


};

#endif
