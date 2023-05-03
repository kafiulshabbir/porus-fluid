#ifndef FUNCTIMESTEP_H
#define FUNCTIMESTEP_H

#include "decl/decltypedef.h"
#include "dst/dstdiamension.h"

namespace func
{
	struct TimeStepResult
	{
		int row;
		int col;
		Tfloat timev;
	};
	
	class TimeStep
	{
		
	public:
		static TimeStepResult decide_time_step(const TMns& mnsc, const Tfloat& velocity, const dst::Diamension& diamension);
	};
}

#endif
