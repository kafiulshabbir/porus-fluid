#ifndef DEF_DEC
#define DEF_DEC

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>
#include <algorithm>

typedef std::vector<double> vector;
typedef std::vector<vector> matrix;


namespace dec
{
	const std::string FILE_IN_BOUNDARY_CONDITIONS = "in-boundary-conditions.txt";
	const std::string FILE_IN_RADIUS_DISTRIBUTION = "in-radius-distribution.txt";
	const std::string FILE_IN_FILL_DISTRIBUTION = "in-fill-distribution.txt";
	
	const std::string FILE_OUT_PRESSURES = "out-pressures.txt";
	const std::string FILE_OUT_FLOW_VER = "out-flow-ver.txt";
	const std::string FILE_OUT_FLOW_HOR= "out-flow-hor.txt";
	
	const int N_DIRECTIONS = 4;
	const double PI = std::acos(-1);
	
	void PrintMatrix(const matrix& v, const std::string& file_name)
}

#endif
