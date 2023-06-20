#ifndef FUNCVELOCITY_H
#define FUNCVELOCITY_H

#include "decl/decltypedef.h"
#include "dst/dstdiamension.h"

namespace func
{
	class Velocity
	{
		
	public:
		static Tdouble calculate_velocity(
			const Tdouble& radius,
			const TMns& mnsc,
			const std::vector<std::vector<int>>& add_mnsc,
			const std::vector<double>& pressure,
			const dst::Diamension& diamension
		);
		
		static Tdouble refine_velocity(Tdouble v);
	};
}

#endif
