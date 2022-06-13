#include "dat_distributions.h"

namespace dat
{
	
	cls_radius::cls_radius(const std::string& name_file, int n, double multiplier, double left_default, double right_default): cls_tube_frw(name_file, n, multiplier, left_default, right_default) {}
	cls_fill::cls_fill(const std::string& name_file, int n, double multiplier, double left_default, double right_default): cls_tube_frw(name_file, n, multiplier, left_default, right_default) {}
}
