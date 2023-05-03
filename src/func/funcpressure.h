#ifndef FUNCPRESSURE_H
#define FUNCPRESSURE_H

#include "decl/decltypedef.h"
#include "dst/dstdiamension.h"
#include "math/mathlinear.h"
#include <vector>

namespace func
{
	class Pressure
	{
		static Tfloat generate_equations_aug_matrix(const Tfloat& radius, const TMns& mnsc, const dst::Diamension& diamension);
		
	public:
		static std::vector<float> calculate_pressure(const Tfloat& radius, const TMns& mnsc, const dst::Diamension& diamension);
	};
}

#endif
