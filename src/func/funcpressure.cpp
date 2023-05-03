#include "func/funcpressure.h"

Tfloat func::Pressure::generate_equations_aug_matrix(const Tfloat& radius, const TMns& mnsc, const dst::Diamension& diamension)
{
	//std::cout << std::endl << "START-FGenEquForGauss" << std::endl;
	const int n = diamension.rows;
	const int m = diamension.cols;
	const int total_nodes = diamension.total_nodes();
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
			
			const int l = diamension.linear_node_from_coordinate(i, j);
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
			const std::vector<dst::Tube> connections = diamension.generate_tubes_connected_to_node(i, j);
			
			for(size_t i = 0; i < connections.size(); ++ i)
			{
				const dst::Tube& c = connections[i];
				//std::cout << "connection, a=" << c.a << " c=" << c.c << ", r=" << c.r << ", p=" << c.p << std::endl;
				if(c.a)
				{
					const float r = radius[c.r][c.c];
					const dst::Mns& f = mnsc[c.r][c.c];
					const float s = f.scontb(i);
					
					const float K = std::pow(r, 3) / f.mu(declconst::MU1, declconst::MU2);
					e[l] += r * K;
					e[c.p] -= r * K;
					e.back() -= declconst::SIGMA * 2 * s * K;
				}
			}
		}
	}
	
	//std::cout << "okay-FGenEquForGauss" << std::endl;
	//cmdio::Print::pmat("Gauss", equation);
	return equation;
}

std::vector<float> func::Pressure::calculate_pressure(const Tfloat& radius, const TMns& mnsc, const dst::Diamension& diamension)
{
	//std::cout << "okay-gauss Fclac pres" << std::endl;
	
	const Tfloat equations = func::Pressure::generate_equations_aug_matrix(radius, mnsc, diamension);
	const std::vector<float> solution_of_equation = math::Linear::gauss_elimination(equations);
	
	return solution_of_equation;
}

