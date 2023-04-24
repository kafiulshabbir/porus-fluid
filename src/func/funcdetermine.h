#ifndef FUNCDETERMINE_H
#define FUNCDETERMINE_H

#include "decl/decltypedef.h"
#include "decl/declconst.h"
#include "func/funcdetermine.h"
#include "func/funclinear.h"
#include "func/funcconnection.h"
#include "cmdio/cmdioprint.h"

namespace func
{
	class Determine
	{
	public:
		static Tfloat FGenEquForGauss(const Tfloat& radius, const TMns& mnsc);
		static std::vector<float> FCalcPressure(const Tfloat& radius, const TMns& mnsc);
		static Tfloat FCalcVelocity(const std::vector<float>& pressure, const Tfloat& radius, const TMns& mnsc);
		static Tfloat FCalcVolume(Tfloat velocity, const Tfloat& radius, const float time_step);
		static float FDetermineTimeStep(const TMns& mnsc, const Tfloat& velocity);
	};
}

#endif
