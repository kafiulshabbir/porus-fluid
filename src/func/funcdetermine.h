#ifndef FUNCDETERMINE_H
#define FUNCDETERMINE_H

#include "decl/decltypedef.h"
#include "dst/dstdiamension.h"

namespace func
{
	class Determine
	{
	public:
		
		static Tdouble determine_volume(const Tdouble& radius, const Tdouble& velocity, const double time_step, const dst::Diamension& diamension);
	};
}

#endif
