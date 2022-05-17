#include "project_includes.h"
#include "type_boundary.h"
#include "type_radius.h"
#include "type_fill.h"

int main()
{
	const std::string FILE_IN_BOUNDARY_CONDITIONS = "in-boundary-conditions.txt";
	const std::string FILE_IN_RADIUS_DISTRIBUTION = "in-radius-distribution.txt";
	const std::string FILE_IN_FILL_DISTRIBUTION = "in-fill-distribution.txt";
	
	
	type_boundary boundary(FILE_IN_BOUNDARY_CONDITIONS);
	if(boundary.read_file()) return 1;

	type_radius radius(FILE_IN_RADIUS_DISTRIBUTION);
	if(radius.read_file()) return 1;
	
	const int N = radius.N();
	type_fill fill(FILE_IN_FILL_DISTRIBUTION, N);
	if(fill.read_file()) return 1;
	
	std::cout << boundary.density_fluid_1 << '\n';
	
	return 0;
}

