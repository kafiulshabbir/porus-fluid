#ifndef FUNCGLOBAL_H
#define FUNCGLOBAL_H

#include "cmdio/cmdioprint.h"
#include "fileio/fileioplot.h"
#include "fileio/fileiowrite.h"
#include "dst/dstdiamension.h"

#include "func/funcdetermine.h"
#include "func/funcintegration.h"
#include "func/funcmeasure.h"
#include "func/funcpressure.h"
#include "func/functimestep.h"
#include "func/funcvelocity.h"

#include "decl/decltypedef.h"
#include "decl/declconst.h"

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
