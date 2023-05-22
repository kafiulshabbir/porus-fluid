#include "func/funcmeasure.h"

float func::Measure::measure_wetting_fluid_proportion(const Tfloat& radius, const TMns& mnsc)
{
	const int n = radius.size();
	const int m = radius.front().size();
	float total = 0;
	float type_first = 0;
	for(int i = 0; i < n; ++ i)
	{
		for(int j = 0; j < m; ++ j)
		{
			const float rsq = std::pow(radius[i][j], 2);
			type_first += mnsc[i][j].sum_type_first() * rsq;
			total += rsq;
		}
	}
	
	return type_first / total;
}
