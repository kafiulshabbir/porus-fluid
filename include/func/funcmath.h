#ifndef DEF_FMA
#define DEF_FMA
//Functions of Math and Algorithm

int linear_nodes(int i, int j, int m)
{
	return (i * (m + 1) + (i % 2)) / 2 + j;
}

std::pair<int, int> connection_ends(int r, int c, int m)
{
	return {linear_nodes(r, c / 2 + (c % 2) * ((r + 1) % 2), m), linear_nodes(r + 1, c / 2 + (c % 2) * (r % 2), m)};
}

int total_nodes(int n, int m)
{
	return ((n + 1) * (m + 1) + 1) / 2;
}

struct Connections
{
	bool a = true;
	int r;
	int c;
	int p;	
};

std::vector<Connections> FGenConnectionsEqu(int r, int c, int n, int m)
{
	const auto p = linear_nodes(r, c, m);
	std::vector<Connections> v
	{
		{true, r - 1, 2 * c - 1 + r % 2, p - m / 2 - 1},
		{true, r - 1, 2 * c - 0 + r % 2, p - m / 2 - 0},
		{true, r - 0, 2 * c - 0 + r % 2, p + m / 2 + 1},
		{true, r - 0, 2 * c - 1 + r % 2, p + m / 2 + 0}
	};
	
	if(r % 2)
	{
		return v;
	}
	
	if(r == 0)
	{
		v[0].a = false;
		v[1].a = false;
	}
	if(c == 0)
	{
		v[0].a = false;
		v[3].a = false;
	}
	if(2 * c == m)
	{
		v[1].a = false;
		v[2].a = false;
	}
	if(r == n)
	{
		v[2].a = false;
		v[3].a = false;
	}
	
	return v;
}

std::vector<float> FGaussElimination(Tfloat M)
{
	//std::cout << "okay-gauss gaussian eleimination" << std::endl;
	const int n = M.front().size() - 1;
	for(int i = 0; i < n; ++ i)
	{
		float divider = M[i][i];
		for(int j = 0; j <= n; ++ j)
		{
			M[i][j] /= divider;
		}
		
		for(int j = 0; j < n; ++ j)
		{
			if(i == j)
			{
				continue;
			}
			
			float coeff = M[j][i];
			
			for(int k = 0; k <= n; ++ k)
			{
				M[j][k] -= M[i][k] * coeff;
			}
		}
	}
	std::vector<float> v;

	for(auto& row: M)
	{
		v.push_back(row.back());
	}
	
	return v;
}



Tfloat FGenEquForGauss(const Tfloat& radius, const Tmns& mns)
{
	//std::cout << "okay-FGenEquForGauss" << std::endl;
	const int n = radius.size();
	const int m = radius.front().size();
	const int total_nodes = total_nodes(n, m);
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
			
			const int l = linear_nodes(i, j, m);
			auto& e = equation[l];
			if(i == 0)
			{
				e[l] = 1;
				e.back() = PRESSURE_TOP;
				continue;
			}
			if(i == n)
			{
				e[l] = 1;
				e.back() = PRESSURE_BOTTOM;
				continue;
			}
			
			//derection: 0-topleft, 1-topright, 2-bottomright, 3-bottomleft
			const auto connections = FGenConnectionsEqu(i, j, n, m);
			
			for(int i = 0; i < connections.size(); ++ i)
			{
				const auto& c = connections[i];
				//std::cout << "connection, a=" << c.a << " c=" << c.c << ", r=" << c.r << ", p=" << c.p << std::endl;
				if(c.a)
				{
					const auto& r = radius[c.r][c.c];
					const auto& f = mns[c.r][c.c];
					const auto& s = f.scontb(i);
					
					const float K = std::pow(r, 3) / f.mu(MU1, MU2);
					e[l] += r * K;
					e[c.p] -= r * K;
					e.back() -= SIGMA * 2 * s * K;
				}
			}
		}
	}
	
	//std::cout << "okay-FGenEquForGauss" << std::endl;
	//TFPrintMatrix("Gauss", equation);
	return equation;
}
	
std::vector<float> FCalcPressure(const Tfloat& radius, const Tmns& mns)
{
	//std::cout << "okay-gauss Fclac pres" << std::endl;
	return FGaussElimination(FGenEquForGauss(radius, mns));
}



Tfloat FCalcVelocity(const std::vector<float>& pressure, const Tfloat& radius, const Tmns& mns)
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
			const auto& mu = mns[i][j].mu(MU1, MU2);
			const auto& s = mns[i][j].scontb(0);
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

float FDetermineTimeStep(const Tmns& mns, const Tfloat& velocity)
{
	float min_time = HUGE;
	for(int i = 0; i < mns.size(); ++ i)
	{
		for(int j = 0; j < mns[i].size(); ++ j)
		{
			
			const float temp_time = mns[i][j].time(velocity[i][j], TUBE_LENGTH, TIME_DIV);
			min_time = std::min(temp_time, min_time);
		}
	}
	
	return min_time;
}

struct IntegrationResult
{
	Tmns mns;
	float fluid1in;
	float fluid1out;
	float fluid2in;
	float fluid2out;
};

int FCountConnections(const std::vector<Connections>& connections)
{
	int count = 0;
	
	for(const auto& connection: connections)
	{
		count += connection.a;
	}
	
	return count;
}

struct TubeWhereFlowOut
{
	float radius;
	int r;
	int c;
};

bool Fcomparison_outflow(const TubeWhereFlowOut& first, const TubeWhereFlowOut& second)
{
	return first.radius < second.radius;
}

std::vector<float> FAmountVolumeToBePushedIn(float volume, std::vector<float>& tank)
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
	

Tmns FCombineFillAndAdditions(Tmns mns, const Tfloat& velocity, const Tfloat& radius, const std::vector<std::vector<std::vector<float>>>& additions)
{
	for(int i = 0; i < mns.size(); ++ i)
	{
		auto& f = mns[i];
		for(int j = 0; j < f.size(); ++ j)
		{
			f[j].update(velocity[i][j], radius[i][j], additions[i][j], THRESHOLD_FILL);
		}
	}
	
	return mns;
}

Tmns FPerformIntegration(const Tmns& mns, const Tfloat& volume, const Tfloat& velocity, const Tfloat& radius)
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
			const auto connections = FGenConnectionsEqu(i, j, n, m);
			
			/*
			for(const auto& connection: connections)
			{
				std::cout << "connection, a=" << connection.a << " c=" << connection.c << ", r=" << connection.r << ", p=" << connection.p << std::endl;
			}
			*/
			std::vector<float> vol_in(2);
			std::vector<TubeWhereFlowOut> tubes_flow_out;
			for(int direction = 0; direction < connections.size(); ++ direction)
			{
				const auto& c = connections[direction];
				if(c.a)
				{
					const auto& f = mns[c.r][c.c];
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
			
			std::sort(tubes_flow_out.begin(), tubes_flow_out.end(), *Fcomparison_outflow);
			for(const auto& tpshin: tubes_flow_out)
			{
				//std::cout << "tube_push_out after sort: radius=" << tpshin.radius << ", r=" << tpshin.r << ", c=" << tpshin.c << std::endl;
				
				additions[tpshin.r][tpshin.c] = FAmountVolumeToBePushedIn(volume[tpshin.r][tpshin.c], vol_in);
			}
		}
	}
	//std::cout << "-------FCombineFillAndAdditions" << std::endl;
	return FCombineFillAndAdditions(mns, velocity, radius, additions);
}




float FMeasureWettingFluidProportion(const Tmns& mns, const Tfloat& radius)
{
	float total = 0;
	float type_first = 0;
	for(int i = 0; i < radius.size(); ++ i)
	{
		for(int j = 0; j < radius[i].size(); ++ j)
		{
			const float rsq = std::pow(radius[i][j], 2);
			type_first += mns[i][j].sum_type_first() * rsq;
			total += rsq;
		}
	}
	
	return type_first / total;
}

void FSeedRandomGenerator()
{
	std::srand((unsigned)std::time(nullptr));
}




#endif
