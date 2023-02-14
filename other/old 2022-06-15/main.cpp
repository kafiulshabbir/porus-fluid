#include "dec.h"

#include "dat_boundary.h"
#include "dat_radius.h"
#include "dat_fill.h"

#include "prs.h"
// g++ -Wall -std=c++17 -o calculate dat_boundary.cpp dat_fill.cpp dat_radius.cpp dat_tube.cpp dec.cpp main.cpp prs.cpp stc_node.cpp 


// DEFINITION: NAMESPACES
// 1- dat:: DATa classes: dat::radius, dat::boundary, dat::fill: (radius-distribution), (boundary-conditions), (fill-distribution)
// 2- dec:: DEClarations of constants, other functions used in the program
// 3- drw:: DRaW library: drw::colour and drw::bmp
// 4- prs:: PRocesS data, calculation from well defined inputs
// 5- std:: STanDard c++ library
// 6- stc:: STruCtures which help in the project stc::node


int main()
{
	// STEP-1: Read the 4 input files: 1-boundary-conditions, 2-radius-distribution, 3-fill-distribution, 4-sigma-distribution
	// STEP-2: Create n^2 equations for pressures at each point
	// STEP-3: Solve for pressures at each point by using gaussian elimination
	// STEP-4: Calculate flow rates
	
// STEP-1: Read
	dat::cls_boundary boundary(dec::FILE_IN_BOUNDARY_CONDITIONS);
	const int n = boundary.nxn_grid_size;
	
	dat::cls_radius radius(dec::FILE_IN_RADIUS_DISTRIBUTION, n, boundary.multiplier_for_radius, boundary.leak_radius);
	dat::cls_fill fill(dec::FILE_IN_FILL_DISTRIBUTION, n, boundary.fluid_type_input, boundary.fluid_type_output);
	dat::cls_sigma sigma(dec::FILE_IN_SIGMA_DISTRIBUTION, n, boundary.sigma_input, boundary.sigma_output);
	
// STEP-2: Generate pressure equations
	matrix gauss_matrix_for_pressure = prs::GenerateGaussMatrixForUnknownPressure(boundary, fill, radius);
		//PrintMatrix(gauss_matrix_for_pressure, "out-matrix-elem.txt");
	
// STEP-3: Solve pressures
	vector calculated_pressures = prs::GaussElimination(gauss_matrix_for_pressure);
	matrix pressure_distribution = GeneratePressureDistributionFromVector(calculated_pressures, boundary.pressure_input, boundary.pressure_output, n);
		//PrintMatrix(pressure_distribution, dec::FILE_OUT_PRESSURES);
	
//STEP-4: Calculate flow rate
	flow_matrices flow = CalculateFlowMatrices(pressure_distribution, radius, fill, boundary.length_tube, boundary.viscosity_fluid_1, boundary.viscosity_fluid_2, boundary.sigma_in_tube, n);
	PrintMatrix(flow.hor, dec::FILE_OUT_FLOW_HOR);
	PrintMatrix(flow.ver, dec::FILE_OUT_FLOW_VER);
	
// STEP-5: Plot node-pressure(coloured, texted), tube-velocity(coloured, texted, arrowed)
	
// STEP-6: Plot tube(thickness, fill distribution)
	return 0;
}
