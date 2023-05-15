#ifndef FUNCLINEAR_H
#define FUNCLINEAR_H

#include "decl/decltypedef.h"
#include <vector>

namespace math
{
	class Linear
	{
	public:

		static std::vector<float> gauss_elimination(Tfloat M);
	};
}

#endif