#ifndef MATHLINEAR_H
#define MATHLINEAR_H

#include "decl/decltypedef.h"
#include <vector>
#include "cmdio/cmdioprint.h"

namespace math
{
	class Linear
	{
	public:

		static std::vector<double> gauss_elimination(Tdouble M);
	};
}

#endif
