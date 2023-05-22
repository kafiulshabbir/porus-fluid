#include "math/mathlinear.h"

std::vector<float> math::Linear::gauss_elimination(Tfloat M)
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
