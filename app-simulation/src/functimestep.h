#ifndef FUNCTIMESTEP_H
#define FUNCTIMESTEP_H

#include "decl/decltypedef.h"
#include "dst/dstdiamension.h"

namespace func
{
	class TimeStep
	{
		
	public:
		static double decide_time_step(const Tdouble& velocity, const dst::Diamension& diamension);
	};
}

#endif
