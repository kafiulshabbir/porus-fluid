#include "prs_pressure.h"

namespace prs
{
	matrix CalculatePressureDistribution(const dat::cls_boundary& boundary, const dat::cls_fill& fill, const dat::cls_radius& radius)
	{
		const int n = radius.N();
		const vector clean_row_in_equation(n * n + 1);
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
					
					std::cout << "equation blank size: " << equation.size() << " row: " << row << " col: " << col << " d: " << direction << "\n";
					//std::cout << "here\n\n\n";
					//q_ij = PI / 8 / (u1 * l1 + u2 * l2) * r4_ij (Pi - Pj) + PI * 2 * sigma / 8 / (u1 * l1 + u2 * l2) * r3_ij
					// K = PI / 8 / (u1 * l1 + u2 * l2)
					
					const double u1 = boundary.viscosity_fluid_0;
					const double u2 = boundary.viscosity_fluid_1;
					//std::cout << "here";
					const double l1 = fill(node_i, node_j);
					//const double l1 = 0.1;
					const double l2 = boundary.length_tube - l1;
					const double K = PI / 8 / (u1 * l1 + u2 * l2);
					
					//std::cout << "here";
					const double r4_ij = radius(node_i, node_j, 4);
					
					const double coeff_P = K * r4_ij;
					equation[node_i.linear(n)] += coeff_P;
					
					//std::cout << "linear: " << node_i.linear(n) << ", " << node_j.linear(n) << '\n';
					
					if(col == 0 && direction == 3)
					{
						//std::cout << "1";
						equation.back() += coeff_P * boundary.pressure_input;
					}
					else if(col == (n - 1) && direction == 1)
					{
						//std::cout << "2";
						equation.back() += coeff_P * boundary.pressure_output;
					}
					else
					{
						std::cout << "MODIFY: " << node_j.linear(n) << '\n';
						equation[node_j.linear(n)] -= coeff_P;
					}
					
					const double r3_ij = radius(node_i, node_j, 3);
					const double constant = fill.sign(node_i, node_j) * K * 2.0 * boundary.sigma * r3_ij; // sign is positive, if the first fluid is located first
					equation.back() -= constant;
				}
				
				gauss.push_back(equation);
				
				for(int i = 0; i < equation.size(); ++ i)
				{
					std::cout << equation[i] << ", ";
				}
				
			}
			//std::cin.get();
		}
		return gauss;
	}
	
	std::vector<double> GaussElimination(matrix v)
	{
		const int N = v.front().size() - 1;
		for(int i = 0; i < N; ++ i)
		{
			double divider = v[i][i];
			for(int j = 0; j <= N; ++ j)
			{
				v[i][j] /= divider;
			}
			
			for(int j = 0; j < N; ++ j)
			{
				if(i == j)
				{
					continue;
				}
				
				double coeff = v[j][i];
				
				for(int k = 0; k <= N; ++ k)
				{
					v[j][k] -= v[i][k] * coeff;
				}
			}
			//std::cout << "\n\n----------------\n";
			//print(v);
		}
		
		std::vector<double> pressures;
		
		for(auto& row: v)
		{
			pressures.push_back(row.back());
		}
		
		return pressures;
	}
	
	void print(const matrix& v)
	{
		for(const auto& row: v)
		{
			for(auto& cell: row)
			{
				std::cout << cell << ", ";
			}
			
			std::cout << "\n";
		}
	}
}
