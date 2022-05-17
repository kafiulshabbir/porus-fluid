#ifndef DEF_PROJECT_INCLUDES
#define DEF_PROJECT_INCLUDES

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>
#include <algorithm>

typedef std::vector<double> vec;
typedef std::vector<vec> matrix;
const std::string tool_generate_radius_distribution = "tool_generate_radius_distribution";
const std::string radius_distribution_txt_first_line = "nxn-grid-size";

double ReadNamedData(std::ifstream&);
matrix ReadMatrix(int, int, std::ifstream&);
void FileOpenFailure(const std::string&);
#endif
