#ifndef FUNCLINEAR_H
#define FUNCLINEAR_H

namespace func
{
	class Linear
	{
	public:

		static std::vector<float> FGaussElimination(Tfloat M)
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



		static Tfloat FGenEquForGauss(const Tfloat& radius, const TMns& dst::Mns)
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
							const auto& f = dst::Mns[c.r][c.c];
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
	};
}

#endif
