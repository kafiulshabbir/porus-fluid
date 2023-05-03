#include "func/funcintegration.h"

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



TMns func::Global::trimmer(TMns mnsc, const Tfloat& velocity)
{
	return mnsc;
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


