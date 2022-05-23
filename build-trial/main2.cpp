#include "dec.h"

#include "dat_boundary.h"
#include "dat_radius.h"
#include "dat_fill.h"

#include "prs_pressure.h"
// g++ -Wall -std=c++17 main2.cpp stc_node.cpp dat_tube.cpp dat_fill.cpp dat_radius.cpp dat_boundary.cpp prs -o main2

// There are 6 namespaces:

// 1- dat:: DATa classes: dat::radius, dat::boundary, dat::fill: (radius-distribution), (boundary-conditions), (fill-distribution)
// 2- dec:: DEClarations of constants 
// 3- drw:: DRaW library: drw::colour and drw::bmp
// 4- prs:: PRocesS data, calculation from well defined inputs
// 5- std:: STanDard c++ library
// 6- stc:: STruCtures which help in the project stc::node

int main()
{
	// STEP-1: READ THE THREE INPUT FILES
	dat::cls_boundary boundary(dec::FILE_IN_BOUNDARY_CONDITIONS);
	if(boundary.read_file()) return 1;
	
	const int n = boundary.nxn_grid_size;
	
	dat::cls_radius radius(dec::FILE_IN_RADIUS_DISTRIBUTION, n, boundary.multiplier_for_radius, boundary.leak_radius, boundary.leak_radius);
	if(radius.read_file()) return 1;
	
	dat::cls_fill fill(dec::FILE_IN_FILL_DISTRIBUTION, n, 1, boundary.fluid_type_input, boundary.fluid_type_output);
	if(fill.read_file()) return 1;
	
	radius.print_data();
	fill.print_data();
	
	// STEP-2: GENERATE EQUATIONS RELATING THE PRESSURES AT EACH POINT
	matrix gauss_matrix_for_pressure = prs::GaussMatrixForPressure(boundary, fill, radius);
	
	// STEP-3: CALCULATE PRESSURES
	vector calculated_pressures = prs::GaussElimination(a);
	
	// STEP-4: CALCULATE VELOCITIES
	
	// STEP-5: PLOT CUSTOM [COLOURED] PRESSURE DISTRIBUTION, [COLOURED] WITH TUBE VELOCITY, [ARROWED] TUBE VELOVITY
	
	// STEP-6: PLOT THICKNESS OF TUBE AND THEIR FILL DISTRIBUTION
	return 0;
}
