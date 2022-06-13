#ifndef DEF_DAT_DISTRIBUTIONS
#define DEF_DAT_DISTRIBUTIONS

#include "dec.h"
#include "dat_tube.h"

namespace dat
{
	class cls_fill: public cls_tube_frw
	{
		double fluid_viscosity_default() // EDIT REQUIRED
	public:
		cls_fill() = delete;
		cls_fill(const std::string& name_file, int n, double fluid_type_input, double fluid_type_output);
	};
	
	
	class cls_radius: public cls_tube_frw
	{
	public:
		cls_radius() = delete;
		cls_radius(const std::string& name_file, int n, double multiplier, double left_default, double right_default);
	};
	
	class cls_sigma: public cls_tube_frw
	{
	public:
		cls_sigma = delete;
	};
}
#endif
