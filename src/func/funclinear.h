#ifndef FUNCLINEAR_H
#define FUNCLINEAR_H

#include "decl/decltypedef.h"
#include <vector>

namespace func
{
	class Linear
	{
	public:

		static std::vector<float> FGaussElimination(Tfloat M);
	};
}

#endif
