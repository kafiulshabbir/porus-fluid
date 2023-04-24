#include "func/funcglobal.h"
	
void func::Global::simulate(const Tfloat& radius, TMns& mnsc, const dst::Diamension& diamension)
{
	cmdio::Print::pmat("radius", radius);
	
	float clock = 0;
	int count = 10000;
	
	func::Global::makeplot(mnsc, radius, clock, count);
	
	float wetting_fluid_proportion;
	while(within_limits_fluid_first_type(wetting_fluid_proportion, mnsc, radius))
	{
		// PRESSURE
		const auto pressure = func::Determine::FCalcPressure(radius, mnsc);
		cmdio::Print::pmat("pressure", pressure, radius.size(), radius.front().size());
		
		// VELOCITY
		const auto velocity = func::Determine::FCalcVelocity(pressure, radius, mnsc);
		cmdio::Print::pmat("velocity", velocity);
		
		// TIME STEP
		const auto time_step = func::Determine::FDetermineTimeStep(mnsc, velocity);
		
		// VOLUME
		const auto volume = func::Determine::FCalcVolume(velocity, radius, time_step);
		cmdio::Print::pmat("volume", volume);
		//std::cout << "PRS-###### " << count << ", clock=" << clock << ", proportion=" << wetting_fluid_proportion << std::endl <<std::endl;
		
		// INTEGRATION
		mnsc = func::Global::integration(mnsc, volume, velocity, radius);
		clock += time_step;
		
		// TRIMMER
		mnsc = func::Global::trimmer(mnsc, velocity);
		if((count++) % declconst::PLOT_EACH_N)
		{
			continue;
		}
		
		//cmdio::Print::pmnsc(mnsc);
		
		func::Global::makeplot(mnsc, radius, clock, count);
		std::cout << "PRS-###### " << count << ", clock=" << clock << ", proportion=" << wetting_fluid_proportion << std::endl <<std::endl;
	}
}

void func::Global::makeplot(const TMns& mnsc, const Tfloat& radius, const float clock, const int count)
{
	fileio::Plot::with_radius(mnsc, radius, clock, count);
	fileio::Plot::without_radius(mnsc, count);
}
		
bool func::Global::within_limits_fluid_first_type(float& proportion, const TMns& mnsc, const Tfloat& radius)
{
	proportion = func::Global::FMeasureWettingFluidProportion(mnsc, radius);
	return proportion <= declconst::MAX_WETTING_PROPORTION;
}
		
TMns func::Global::integration(const TMns& mnsc, const Tfloat& volume, const Tfloat& velocity, const Tfloat& radius)
{
	const int n = volume.size();
	const int m = volume.front().size();
	
	float fluid1in = 0;
	float fluid1out = 0;
	float fluid2in = 0;
	float fluid2out = 0;
	
	std::vector<std::vector<std::vector<float>>> additions(n, std::vector<std::vector<float>>(m));
	
	for(int i = 0; i <= n; ++ i)
	{
		int mt = m / 2 - i % 2;
		for(int j = 0; j <= mt; ++ j)
		{
			//std::cout << "Performing integration i=" << i << ", j=" << j << std::endl;
			const auto connections = func::Connection::FGenConnectionsEqu(i, j, n, m);
			
			/*
			for(const auto& connection: connections)
			{
				std::cout << "connection, a=" << connection.a << " c=" << connection.c << ", r=" << connection.r << ", p=" << connection.p << std::endl;
			}
			*/
			std::vector<float> vol_in(2);
			std::vector<func::TubeWhereFlowOut> tubes_flow_out;
			for(int direction = 0; direction < connections.size(); ++ direction)
			{
				const auto& c = connections[direction];
				if(c.a)
				{
					const auto& f = mnsc[c.r][c.c];
					const auto& vel = velocity[c.r][c.c];
					const auto& vol = volume[c.r][c.c];
					const auto& r = radius[c.r][c.c];
					if(f.is_flow_into_node(direction, vel))
					{
						vol_in[f.type_fluid_into_node(direction)] += vol;
					}
					else
					{
						tubes_flow_out.push_back({r, c.r, c.c});
					}
				}
			}
			
			//for(const auto& tpshin: tubes_flow_out)	std::cout << "tube_push_out before short: radius=" << tpshin.radius << ", r=" << tpshin.r << ", c=" << tpshin.c << std::endl;
			
			//std::cout << "second stage reached!" << std::endl;
			if(i == 0)
			{
				fluid1out += vol_in.front();
				fluid2out += vol_in.back();
				continue;
			}
			if(i == n) // NOTE might remove else
			{ 
				for(const auto& tpshin: tubes_flow_out)
				{
					additions[tpshin.r][tpshin.c] = {volume[tpshin.r][tpshin.c], 0};
				}
				continue;
				
			}
			
			std::sort(tubes_flow_out.begin(), tubes_flow_out.end(), Fcomparison_outflow);
			for(const auto& tpshin: tubes_flow_out)
			{
				//std::cout << "tube_push_out after sort: radius=" << tpshin.radius << ", r=" << tpshin.r << ", c=" << tpshin.c << std::endl;
				
				additions[tpshin.r][tpshin.c] = FAmountVolumeToBePushedIn(volume[tpshin.r][tpshin.c], vol_in);
			}
		}
	}
	
	//cmdio::Print::padd("additions", additions);
	
	//std::cout << "-------combine_additions" << std::endl;
	return combine_additions(mnsc, velocity, radius, additions);
}

TMns func::Global::combine_additions(TMns mnsc, const Tfloat& velocity, const Tfloat& radius, const std::vector<std::vector<std::vector<float>>>& additions)
{
	for(int i = 0; i < mnsc.size(); ++ i)
	{
		auto& f = mnsc[i];
		for(int j = 0; j < f.size(); ++ j)
		{
			f[j].update(velocity[i][j], radius[i][j], additions[i][j]);
		}
	}
	
	return mnsc;
}

bool func::Global::Fcomparison_outflow(const func::TubeWhereFlowOut& first, const func::TubeWhereFlowOut& second)
{
	return first.radius < second.radius;
}

std::vector<float> func::Global::FAmountVolumeToBePushedIn(float volume, std::vector<float>& tank)
{
	auto v = tank;
	v.front() = std::min(tank.front(), volume);
	v.back() = volume - v.front();
	
	for(int i = 0; i < tank.size(); ++ i)
	{
		tank[i] -= v[i];
	}
	
	return v;
}

float func::Global::FMeasureWettingFluidProportion(const TMns& mnsc, const Tfloat& radius)
{
	float total = 0;
	float type_first = 0;
	for(int i = 0; i < radius.size(); ++ i)
	{
		for(int j = 0; j < radius[i].size(); ++ j)
		{
			const float rsq = std::pow(radius[i][j], 2);
			type_first += mnsc[i][j].sum_type_first() * rsq;
			total += rsq;
		}
	}
	
	return type_first / total;
}

TMns func::Global::trimmer(TMns mnsc, const Tfloat& velocity)
{
	return mnsc;
}
