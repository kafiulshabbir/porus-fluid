#ifndef FUNCDETERMINE_H
#define FUNCDETERMINE_H

#include "decl/decltypedef.h"
#include "dst/dstdiamension.h"

namespace func
{
	class Determine
	{
	public:
		
		static Tfloat determine_volume(Tfloat velocity, const Tfloat& radius, const float time_step, const dst::Diamension& diamension);
	};
}

#endif
