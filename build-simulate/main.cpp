#include "project_includes.h"

#include "type_boundary.h"
#include "type_radius.h"
#include "type_fill.h"

#include "bmp.h"

#include "process_pressure.h"
// There are 4 namespaces:
// 1- std:: c++ STanDard library
// 2- drw:: DRaW library, which includes drw::colour and drw::bmp
// 3- typ:: TYPe File Read Write, classes such as radius_distribution, boundary_conditions, and fill_distribution
// 4- prs:: PRocesS data, calculation from well defined inputs
int main()
{
	// INPUT FILE NAME DECLERATIONS
	const std::string FILE_IN_BOUNDARY_CONDITIONS = "in-boundary-conditions.txt";
	const std::string FILE_IN_RADIUS_DISTRIBUTION = "in-radius-distribution.txt";
	const std::string FILE_IN_FILL_DISTRIBUTION = "in-fill-distribution.txt";
	
	/*
	 * Reads:
	 * 1 - Boundary Conditions
	 * 2 - Radius Distribution
	 * 3 - Initial filling distribution
	 */
	// Any error throws and exits the program with non-zero exit code
	typ::type_boundary boundary(FILE_IN_BOUNDARY_CONDITIONS);
	if(boundary.read_file()) return 1;

	typ::type_radius radius(FILE_IN_RADIUS_DISTRIBUTION);
	if(radius.read_file()) return 1;
	
	const int N = radius.N();
	typ::type_fill fill(FILE_IN_FILL_DISTRIBUTION, N);
	if(fill.read_file()) return 1;
	
	
	// Solve equations to find PRESSURE DISTRIBUTION
	auto a = prs::CalculatePressureDistribution(boundary, fill, radius);
	/*
	const int WIDTH = 2000;
	const int HEIGHT = 1500;
	const double MARGIN = 0.1; // the graph will be located leaving this margin around - it is proportion to the lease diamension
	const double PROPORTION_PRESSURE_BOX = 0.25; // the maximum correspnds to the diamension of the square being equal to 50% of the seperation between the nodes
	const double PROPORTION_TUBE_THICKNESS = 0.5; // The maximum 1.0 corresponds to the largest tube being eual to the size of the box
	
	
	drw::bmp a(WIDTH, HEIGHT);
	
	const int length_tube = std::min(((1.0 - 2.0 * MARGIN) * HEIGHT) / (N - 1.0), ((1.0 - 2.0 * MARGIN) * WIDTH) / (N + 1.0));
	const int size_pressure_box = PROPORTION_PRESSURE_BOX * length_tube;
	const int length_tube_effective = length_tube - size_pressure_box;
	
	//this section constructs pressure boxes through out the image
	const int x_begin = MARGIN * WIDTH;
	const int y_begin = (1 - MARGIN) * HEIGHT;
	
	a.setFgColour(drw::black);
	
	int y = y_begin;
	for(int i = 0; i < N; ++ i)
	{
		int x = x_begin;

		for(int j = 0; j < N + 2; ++ j)
		{
			a.drawCentreSquare(x, y, size_pressure_box);
			x += length_tube;
		}
		
		y -= length_tube;
	}
	
	// prints horizontal tubes
	a.setFgColour(drw::green);
	const int x_begin_tube = x_begin + length_tube / 2;
	const int thickness_tube = PROPORTION_TUBE_THICKNESS * size_pressure_box;
	y = y_begin;
	for(int i = 0; i < N; ++ i)
	{
		int x = x_begin_tube;

		for(int j = 0; j < N + 1; ++ j)
		{
			a.drawCentreRectangle(x, y, length_tube_effective, thickness_tube);
			x += length_tube;
		}
		
		y -= length_tube;
	}
	
	// prints vertical tubes
	a.setFgColour(drw::blue);
	const int x_begin_tube_2 = x_begin + length_tube;
	y = y_begin - length_tube/2;
	for(int i = 0; i + 1 < N; ++ i)
	{
		int x = x_begin_tube_2;

		for(int j = 0; j < N; ++ j)
		{
			a.drawCentreRectangle(x, y, thickness_tube, length_tube_effective);
			x += length_tube;
		}
		
		y -= length_tube;
	}
	a.save("hello.bmp");
	
	std::cout << boundary.density_fluid_1 << '\n';
	*/
	
	return 0;
}

