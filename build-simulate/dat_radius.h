#ifndef DEF_DAT_RADIUS
#define DEF_DAT_RADIUS

#include "dec.h"
#include "dat_tube.h"

namespace dat
{

	class cls_radius: public cls_tube
	{
	public:
		cls_radius() = delete;
		cls_radius(const std::string& name_file, int n, double multiplier, double left_default, double right_default);
	};
}

#endif
