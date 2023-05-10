#ifndef FUNCVELOCITY_H
#define FUNCVELOCITY_H

#include "decl/decltypedef.h"
#include "dst/dstdiamension.h"

namespace func
{
	class Velocity
	{
		
	public:
		static Tfloat calculate_velocity
		(
			const Tfloat& radius,
			const TMns& mnsc,
			const std::vector<float>& pressure,
			const dst::Diamension& diamension
		);
	};
}

#endif
