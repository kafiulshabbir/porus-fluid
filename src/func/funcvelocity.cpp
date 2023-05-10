#include "func/funcvelocity.h"


Tfloat func::Velocity::calculate_velocity
(
	const Tfloat& radius,
	const TMns& mnsc,
	const std::vector<float>& pressure,
	const dst::Diamension& diamension
)
{
	const int n = diamension.rows;
	const int m = diamension.cols;
	
	Tfloat velocity(n, std::vector<float>(m));
	for(int i = 0; i < n; ++ i)
	{
		for(int j = 0; j < m; ++ j)
		{
			const std::pair<int, int> locs = diamension.linear_node_at_ends_of_tube(i, j);
			const float delp = pressure[locs.second] - pressure[locs.first];
			const float r = radius[i][j];
			const float mu = mnsc[i][j].mu(declconst::MU1, declconst::MU2);
			const float s = mnsc[i][j].scontb(0);
			velocity[i][j] = r / 8 / mu / declconst::TUBE_LENGTH * (delp * r + s * 2 * declconst::SIGMA);
		}
	}
	
	return velocity;
}
