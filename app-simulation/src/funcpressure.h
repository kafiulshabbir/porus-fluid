#ifndef FUNCPRESSURE_H
#define FUNCPRESSURE_H

#include "decltypedef.h"
#include "declconst.h"
#include "dstdiamension.h"
#include "cmdioprint.h"
#include "mathlinear.h"
#include <vector>

namespace func
{
	class Pressure
	{
		static Tdouble generate_equations_aug_matrix(
			const Tdouble& radius,
			const TMns& mnsc,
			const std::vector<std::vector<int>>& add_mnsc,
			const dst::Diamension& diamension);

		static int calc_sign_capll_pressure(
			const int internal,
			const int add_table,
			const int direction
		);

	public:
		static std::vector<double> calculate_pressure(
			const Tdouble& radius,
			const TMns& mnsc,
			const std::vector<std::vector<int>>& add_mnsc,
			const dst::Diamension& diamension
		);
	};
}

#endif
