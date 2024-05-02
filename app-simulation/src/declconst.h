#ifndef DECLCONST_H
#define DECLCONST_H

#include <cmath>
#include <limits>

namespace declconst
{
	//GENERAL CONSTANTS
	const double PI = std::acos(-1);
	const double HUGE = std::numeric_limits<double>::max();

	//Physical Characterstics of the experiment
	const double PRESSURE_LEFT = 110;
	const double PRESSURE_RIGHT = 10;
	const double SIGMA = 10; //7.56e-2;	// 7.56e-2 FOR water at 20C in SI units, produces 75Pa pressure difference for tube of radius 1mm
	const double TUBE_LENGTH = 10;	// 0.1;
	const double MU1 = 0.1; // 1e-3;			// viscosity of the invading liquid: water
	const double MU2 = 0.1; // 1e-5;			// viscosity of defending liquid: air

	//Parameters of simulation
	const double MAX_WETTING_PROPORTION = 0.90;
	//const double THRESHOLD_FILL = 1e-15;		//if any meniscus is smaller than this proportion, then it is destroyed
	const double TRIMMER_PRECISION = 1e-6;
	const double TIME_DIV = 10;				// if the nearest meniscus by time is further, then L / TIME_DIV is prefered: CODE HAVE CHANGED -  AND I AM NOT SURE IF MY CODE WORKS LIKE THIS ANYMORE
	const int IMAGE_SIZE = 1000;
	const double FINE_RADIUS_RANDOMNESS = 1e2;
	const int PLOT_EACH_N = 100;
	const double IGNORE_VEL = 1e8; // If any velocity in the porous body is IGNORE_VEL times less than the max velocity then it is forced to be 0

/*
	const int total_number_tubes = 30;
	const int leave_tubes_outer = 2;
	const int ROW_THIN_B = leave_tubes_outer;
	const int ROW_THIN_E = total_number_tubes - leave_tubes_outer - 1;
	const int COL_THIN_B = leave_tubes_outer;
	const int COL_THIN_E = total_number_tubes - leave_tubes_outer - 1;
*/
}
#endif
