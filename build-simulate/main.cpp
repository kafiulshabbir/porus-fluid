#include "dec.h"

#include "dat_boundary.h"
#include "dat_radius.h"
#include "dat_fill.h"

#include "drw.h"

#include "prs_pressure.h"
// There are 6 namespaces:

// 1- dat:: DATa classes: dat::radius, dat::boundary, dat::fill: (radius-distribution), (boundary-conditions), (fill-distribution)
// 2- dec:: DEClarations of constants 
// 3- drw:: DRaW library: drw::colour and drw::bmp
// 4- prs:: PRocesS data, calculation from well defined inputs
// 5- std:: STanDard c++ library
// 6- stc:: STruCtures which help in the project stc::node

int main()
{
	// STEP-1: DEFINE FILE NAME FOR INPUTS
	const std::string FILE_IN_BOUNDARY_CONDITIONS = "in-boundary-conditions.txt";
	const std::string FILE_IN_RADIUS_DISTRIBUTION = "in-radius-distribution.txt";
	const std::string FILE_IN_FILL_DISTRIBUTION = "in-fill-distribution.txt";
	
	// STEP-2: READ THE THREE INPUT FILES
	dat::cls_boundary boundary(FILE_IN_BOUNDARY_CONDITIONS);
	if(boundary.read_file()) return 1;

	dat::cls_radius radius(FILE_IN_RADIUS_DISTRIBUTION);
	if(radius.read_file()) return 1;
	
	const int N = radius.N();
	dat::cls_fill fill(FILE_IN_FILL_DISTRIBUTION, N);
	if(fill.read_file()) return 1;
	
	
	// STEP-3A: GENERATE EQUATIONS RELATING THE PRESSURES AT EACH POINT
	auto a = prs::CalculatePressureDistribution(boundary, fill, radius);
	
	// STEP-3B: CALCULATE PRESSURES
	auto calculated_pressures = prs::GaussElimination(a);
	
	// STEP-3C: CALCULATE VELOCITIES
	
	
	// STEP-4A: PLOT CUSTOM [COLOURED] PRESSURE DISTRIBUTION, [COLOURED] WITH TUBE VELOCITY, [ARROWED] TUBE VELOVITY
	// STEP-4B: PLOT THICKNESS OF TUBE AND THEIR FILL DISTRIBUTION
	
	
	return 0;
}

