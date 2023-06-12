#include "math/mathlinear.h"
typedef long double lld;

std::vector<double> math::Linear::gauss_elimination(Tdouble M)
{
	const int n = M.size();
	
	//cmdio::Print::pmat("Initial gauss", M);
	/*
	std::vector<double> sums(n + 1);
	
	for(int i = 0; i < n; ++ i)
	{
		for(int j = 0; j <= n; ++ j)
		{
			sums[j] += M[i][j];
		}
	}
	
	std::cout << "sums of rows in gauss = {";
	for(size_t i = 0; i < sums.size(); ++ i)
	{
		std::cout << sums[i] << ", ";
	}
	
	std::cout << "}" << std::endl;
	*/
	
	for(int i = 0; i < n; ++ i)
	{
		std::vector<double>& eq_row = M[i];
		const double divider = eq_row[i];
		// this sets the target coefficient as 1.0000 for the equation
		for(int j = i; j <= n; ++ j)
		{
			eq_row[j] /= divider;
		}
		
		for(int j = i + 1; j < n; ++ j)
		{
			std::vector<double>& eq_target_row = M[j];
			
			const double coeff = eq_target_row[i];
			
			if(coeff == 0)
			{
				continue;
			}

			for(int k = i; k <= n; ++ k)
			{
				eq_target_row[k] -= eq_row[k] * coeff;
			}
		}
		

	}
	
	for(int i = n - 1; i > 0; -- i)
	{
		for(int j = 0; j < i; ++ j)
		{
			M[j].back() -= M[j][i] * M[i].back();
			M[j][i] = 0;
		}
	}
	
	// cmdio::Print::pmat("After gauss", M);
	
	std::vector<double> v;

	for(auto& row: M)
	{
		v.push_back(row.back());
	}
	
	return v;
}
