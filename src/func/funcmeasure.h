#ifndef FUNCMEASURE_H
#define FUNCMEASURE_H

#include "decl/decltypedef.h"
#include "dst/dstdiamension.h"

namespace func
{
	class Measure
	{
		
	public:
		static float measure_wetting_fluid_proportion(const Tfloat& radius, const TMns& mnsc);
	};
}

#endif
