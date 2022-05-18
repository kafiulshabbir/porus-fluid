#include "project_includes.h"
#include "type_boundary.h"
#include "type_radius.h"
#include "type_fill.h"
#include "bmp.h"

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


	const int WIDTH = 4000;
	const int HEIGHT = 3000;
	const double MARGIN = 0.1; // the graph will be located leaving this margin around - it is proportion to the lease diamension
	const double SIZE_PRESSURE_BOX = 0.5; // the maximum correspnds to the diamension of the square being equal to 50% of the seperation between the nodes
	const double SIZE_TUBE = 0.5; // The maximum 1.0 corresponds to the largest tube being eual to the size of the box
	
	
	bmp a(WIDTH, HEIGHT);
	
	std::cout << boundary.density_fluid_1 << '\n';
	
	return 0;
}

