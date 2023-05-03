#ifndef FUNCVELOCITY_H
#define FUNCVELOCITY_H

#include "decl/decltypedef.h"
#include "dst/dstdiamension.h"

namespace func
{
	class Velocity
	{
		static Tfloat calculate_velocity(const std::vector<float>& pressure, const Tfloat& radius, const TMns& mnsc, const dst::Diamension& diamension);	
	};
}

#endif
