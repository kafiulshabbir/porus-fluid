#ifndef FUNCGLOBAL_H
#define FUNCGLOBAL_H

#include "cmdioprint.h"
#include "fileioplot.h"
#include "fileiowrite.h"
#include "dstdiamension.h"

#include "funcdetermine.h"
#include "funcintegration.h"
#include "funcmeasure.h"
#include "funcpressure.h"
#include "functimestep.h"
#include "funcvelocity.h"

#include "decltypedef.h"
#include "declconst.h"

namespace func
{
	class Global
	{
		static void makeplot
		(
			const Tdouble& radius,
			const TMns& mnsc,
			const int count,
			const double clock
		);

		static bool within_limits_fluid_first_type
		(
			const Tdouble& radius,
			const TMns& mnsc,
			double& proportion
		);

	public:
		static void simulate(const Tdouble& radius,
			TMns& mnsc, const dst::Diamension& diamension);
	};
}

#endif
