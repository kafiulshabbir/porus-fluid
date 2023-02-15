#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <utility>
#include <limits>
#include <list>

#include "drw.h"

// Type Definitions
typedef float real; //float can be changed to double or long double to increase the precision, float is used to make the program faster
class Cmns; //Class of Meniscus
typedef real Cradius; //Class of Radius
typedef std::vector<std::vector<real>> Treal; //Table of real numbers
typedef std::vector<std::vector<Cmns>> Tmns;
typedef std::vector<std::vector<Cradius>> Tradius;

//GENERAL CONSTANTS
const real PI = std::acos(-1);
const real HUGE = std::numeric_limits<real>::max();

//Physical Characterstics of the experiment
const real PRESSURE_BOTTOM = 100;
const real PRESSURE_TOP = 2;
const real SIGMA = 0; //7.56e-2; // FOR water at 20C in SI units, produces 75Pa pressure difference for tube of radius 1mm
const real TUBE_LENGTH = 1;//0.1;
const real MU1 = 1;//1e-3; // viscosity of the invading liquid: water
const real MU2 = 1;//1e-5; // viscosity of defending liquid: air

//Parameters of simulation
const real THRESHOLD_FILL = 0.001; //if any meniscus is smaller than this proportion, then it is destroyed
const real TIME_DIV = 10; // if the nearest meniscus by time is further, then L / TIME_DIV is prefered
const int IMAGE_SIZE = 1000;

//Input Output of File names
const std::string FILE_NAME_RADIUS = "radius.txt";
const std::string FILE_NAME_FILL = "fill.txt";
const std::string FOLDER_SAVE_PIC = "pic/";




int main()
{
	Cmns A();
	Cmns B(2, 0, 0.25, 0.80);
	Cmns C(1, 1, 0, 0);
	
	return 0;
}
