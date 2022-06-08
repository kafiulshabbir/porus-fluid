#include "prs.h"

namespace prs
{
	//Generate a equation of size n * (n + 1)
	
	vector GaussElimination(matrix M)
	{
		const int n = M.front().size() - 1;
		for(int i = 0; i < n; ++ i)
		{
			double divider = M[i][i];
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
				
				double coeff = M[j][i];
				
				for(int k = 0; k <= n; ++ k)
				{
					M[j][k] -= M[i][k] * coeff;
				}
			}
			//std::cout << "\n\n----------------\n";
			//print(v);
		}
		vector v;
	
		for(auto& row: M)
		{
			v.push_back(row.back());
		}
		
		return v;
	}
	
	double sign_proportion(double n)
	{
		if(n < 0)
		{
			return -1;
		}
		
		return 1;
	}
	
	// For jump
	/*
	 * derection-0, sign_proportion * +1
	 * derection-1, sign_proportion * -1
	 * derection-2, sign_proportion * -1
	 * derection-3, sign_proportion * +1
	 */
					 
					 
	double sign_direction(int n)
	{
		switch (n)
		{
			case 0:
			return -1;
			
			case 1:
			return 1;
			
			case 2:
			return 1;
			
			case 3:
			return -1;
		}
		
		std::cout << "Error in sign_direction.";
		
		return 0;
	}
	
	double sign_jump(double proportion, int direction)
	{
		if(std::abs(proportion) == 1)
		{
			return 0;
		}
		
		return sign_direction(direction) * sign_proportion(proportion);
	}
	
	matrix GaussMatrixForPressure(const dat::cls_boundary& boundary, const dat::cls_fill& fill, const dat::cls_radius& radius)
	{
		const int n = boundary.nxn_grid_size;
		const double u1 = boundary.viscosity_fluid_1;
		const double u2 = boundary.viscosity_fluid_2;
		const double sigma2 = 2.0 * boundary.sigma_in_tube;
		const double l = boundary.length_tube;
		
		matrix gauss_matrix;
		gauss_matrix.reserve(n * n);
		
		for(int row = 0; row < n; ++ row)
		{
			for(int col = 0; col < n; ++ col)
			{
				const stc::node node_i(row, col);
				vector equation(n * n + 1);
				
				for(int direction = 0; direction < dec::N_DIRECTIONS; ++ direction)
				{
					const stc::node node_j = node_i.rel(direction);
					const double r_ij = radius(node_i, node_j);
					
					if(r_ij == 0)
					{
						continue;
					}
					
					// K = PI / 8 / (u1 * l1 + u2 * l2)
					
					const double proportion = fill(node_i, node_j);
					double l1 = std::abs(proportion) * l;
					double l2 = l - l1;
					
					if(proportion < 0)
					{
						std::swap(l1, l2);
					}
					
					const double K = dec::PI / 8.0 / (u1 * l1 + u2 * l2);
					
					
					const double pressure_contribution = K * std::pow(r_ij, 4);
					// For Pressures;
					equation[node_i.linear(n)] += pressure_contribution;
					
					if(col == 0 && direction == 3)
					{
						equation.back() += pressure_contribution * boundary.pressure_input;
						continue; // during entry and exit the fluid is assumened without a miniscus
					}
					if(col == n - 1 && direction == 1)
					{
						equation.back() += pressure_contribution * boundary.pressure_output;
						continue;
					}

					equation[node_j.linear(n)] -= pressure_contribution;
					
					const double jump_contribution = sign_jump(proportion, direction) * K * sigma2 * std::pow(r_ij, 3);
					std::cout << "row: " << row << ", col: " << col << ", direction: " << direction << ", pressure contib: " << pressure_contribution << ", jump: " << jump_contribution << ", proportion: " << proportion << ", radius: " << r_ij << '\n';
					
					if(std::abs(proportion) == 1)
					{
						continue;
					}
					
					equation.back() -= jump_contribution;
					
					
				}
				
				gauss_matrix.push_back(equation);
			}
			
			std::cout << "\n\n\n";
		}
		
		return gauss_matrix;
	}
					
}
