#ifndef DECLCONST_H
#define DECLCONST_H

#include <cmath>
#include <limits>

namespace declconst
{
	//GENERAL CONSTANTS
	const float PI = std::acos(-1);
	const float HUGE = std::numeric_limits<float>::max();

	//Physical Characterstics of the experiment
	const float PRESSURE_BOTTOM = 100;
	const float PRESSURE_TOP = 2;
	const float SIGMA = 0; //7.56e-2;	// 7.56e-2 FOR water at 20C in SI units, produces 75Pa pressure difference for tube of radius 1mm
	const float TUBE_LENGTH = 1;	// 0.1;
	const float MU1 = 1; // 1e-3;			// viscosity of the invading liquid: water
	const float MU2 = 1; // 1e-5;			// viscosity of defending liquid: air

	//Parameters of simulation
	const float MAX_WETTING_PROPORTION = 0.90;
	//const float THRESHOLD_FILL = 1e-15;		//if any meniscus is smaller than this proportion, then it is destroyed
	const float TRIMMER_PRECISION = 1e-6;
	const float TIME_DIV = 5;				// if the nearest meniscus by time is further, then L / TIME_DIV is prefered
	const int IMAGE_SIZE = 1000;
	const float FINE_RADIUS_RANDOMNESS = 1e5;
	const int PLOT_EACH_N = 1;
}
#endif