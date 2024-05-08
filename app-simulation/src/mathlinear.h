#ifndef MATHLINEAR_H
#define MATHLINEAR_H

#include "decltypedef.h"
#include <vector>
#include "cmdioprint.h"

namespace math
{
	class Linear
	{
	public:

		static std::vector<double> gauss_elimination(Tdouble M);
	};
}

#endif
