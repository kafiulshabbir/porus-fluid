#ifndef DEF_PROCESS_PRESSURE
#define DEF_PROCESS_PRESSURE

#include "project_includes.h"

#include "type_boundary.h"
#include "type_radius.h"
#include "type_fill.h"

namespace prs
{
	// Generate a equation of size n * (n + 1)
	
	matrix CalculatePressureDistribution(const typ::type_boundary& boundary, const typ::type_fill& fill, const typ::type_radius& radius)
	{
		const int n = radius.N();
		const vec clean_row_in_equation(n * n + 1);
		matrix gauss;
		gauss.reserve(n * n);
		
		for(int row = 0; row < n; ++ row)
		{
			for(int col = 0; col < n; ++ col)
			{
				auto equation = clean_row_in_equation;
				const typ::node node_i(row, col);
				
				for(int direction = 0; direction < 4; ++ direction)
				{
					const typ::node node_j = node_i.rel(direction);
					
					
					
					//q_ij = PI / 8 / (u1 * l1 + u2 * l2) * r4_ij (Pi - Pj) + PI * 2 * sigma / 8 / (u1 * l1 + u2 * l2) * r3_ij
					// K = PI / 8 / (u1 * l1 + u2 * l2)
					
					const double u1 = boundary.viscosity_fluid_0;
					const double u2 = boundary.viscosity_fluid_1;
					
					const double l1 = fill(node_i, node_j);
					const double l2 = boundary.length_tube - l1;
					const double K = PI / 8 / (u1 * l1 + u2 * l2);
					
					const double r4_ij = radius(node_i, node_j, 4);
					
					const double coeff_P = K * r4_ij;
					euqation[node_i.linear(n)] += coeff_P;
					euqation[node_j.linear(n)] -= coeff_P;
					
					const double r3_ij = radius(node_i, node_j, 3);
					const double constant = fill.sign(node_i, node_j) * K * 2.0 * boundary.sigma * r3_ij; // sign is positive, if the first fluid is located first
					equation.back() -= constant;
				}
				
				gauss.push_back(equation);
			}
		}
		return matrix(0);
	}
	
	
}

#endif
