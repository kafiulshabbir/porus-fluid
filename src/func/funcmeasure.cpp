float func::Global::FMeasureWettingFluidProportion(const TMns& mnsc, const Tfloat& radius)
{
	float total = 0;
	float type_first = 0;
	for(int i = 0; i < radius.size(); ++ i)
	{
		for(int j = 0; j < radius[i].size(); ++ j)
		{
			const float rsq = std::pow(radius[i][j], 2);
			type_first += mnsc[i][j].sum_type_first() * rsq;
			total += rsq;
		}
	}
	
	return type_first / total;
}
