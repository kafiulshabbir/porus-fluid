#include "func/funcdetermine.h"

Tfloat func::Determine::FGenEquForGauss(const Tfloat& radius, const TMns& mnsc)
{
	//std::cout << "okay-FGenEquForGauss" << std::endl;
	const int n = radius.size();
	const int m = radius.front().size();
	const int total_nodes = func::Connection::total_nodes(n, m);
	Tfloat equation(total_nodes, std::vector<float>(total_nodes + 1));
	
	//std::cout << "okay-FGenEquForGauss" << std::endl;
	//std::cout << "total_nodes=" << total_nodes << std::endl;
	
	for(int i = 0; i <= n; ++ i)
	{
		int mt = m / 2 - (i % 2);
		//std::cout << "n= " << n << ", m=" << m << ", mt=" << mt << std::endl;
		for(int j = 0; j <= mt; ++ j)
		{
			
			//std::cout << "i=" << i << ", j=" << j << std::endl;
			
			const int l = func::Connection::linear_nodes(i, j, m);
			auto& e = equation[l];
			if(i == 0)
			{
				e[l] = 1;
				e.back() = declconst::PRESSURE_TOP;
				continue;
			}
			if(i == n)
			{
				e[l] = 1;
				e.back() = declconst::PRESSURE_BOTTOM;
				continue;
			}
			
			//derection: 0-topleft, 1-topright, 2-bottomright, 3-bottomleft
			const auto connections = func::Connection::FGenConnectionsEqu(i, j, n, m);
			
			for(int i = 0; i < connections.size(); ++ i)
			{
				const auto& c = connections[i];
				//std::cout << "connection, a=" << c.a << " c=" << c.c << ", r=" << c.r << ", p=" << c.p << std::endl;
				if(c.a)
				{
					const auto& r = radius[c.r][c.c];
					const auto& f = mnsc[c.r][c.c];
					const auto& s = f.scontb(i);
					
					const float K = std::pow(r, 3) / f.mu(declconst::MU1, declconst::MU2);
					e[l] += r * K;
					e[c.p] -= r * K;
					e.back() -= declconst::SIGMA * 2 * s * K;
				}
			}
		}
	}
	
	//std::cout << "okay-FGenEquForGauss" << std::endl;
	//TFPrintMatrix("Gauss", equation);
	return equation;
}
		
std::vector<float> func::Determine::FCalcPressure(const Tfloat& radius, const TMns& mnsc)
{
	//std::cout << "okay-gauss Fclac pres" << std::endl;
	return func::Linear::FGaussElimination(FGenEquForGauss(radius, mnsc));
}

Tfloat func::Determine::FCalcVelocity(const std::vector<float>& pressure, const Tfloat& radius, const TMns& mnsc)
{
	const int n = radius.size();
	const int m = radius.front().size();
	auto velocity = radius;
	for(int i = 0; i < n; ++ i)
	{
		for(int j = 0; j < m; ++ j)
		{
			const auto locs = func::Connection::connection_ends(i, j, m);
			const auto delp = pressure[locs.second] - pressure[locs.first];
			const auto& r = radius[i][j];
			const auto& mu = mnsc[i][j].mu(declconst::MU1, declconst::MU2);
			const auto& s = mnsc[i][j].scontb(0);
			velocity[i][j] = r / 8 / mu / declconst::TUBE_LENGTH * (delp * r + s * 2 * declconst::SIGMA);
		}
	}
	
	return velocity;
}

Tfloat func::Determine::FCalcVolume(Tfloat velocity, const Tfloat& radius, const float time_step)
{
	for(int i = 0; i < velocity.size(); ++ i)
	{
		auto& v = velocity[i];
		for(int j = 0; j < v.size(); ++ j)
		{
			v[j] = std::abs(v[j] * std::pow(radius[i][j], 2) * declconst::PI * time_step);
		}
	}
	
	return velocity;
}

float func::Determine::FDetermineTimeStep(const TMns& mnsc, const Tfloat& velocity)
{
	float min_time = declconst::HUGE;
	for(int i = 0; i < mnsc.size(); ++ i)
	{
		for(int j = 0; j < mnsc[i].size(); ++ j)
		{
			
			const float temp_time = mnsc[i][j].time(velocity[i][j], declconst::TUBE_LENGTH, declconst::TIME_DIV);
			min_time = std::min(temp_time, min_time);
		}
	}
	
	return min_time;
}
