#ifndef FUNCGLOBAL_H
#define FUNCGLOBAL_H

#include "cmdio/cmdioprint.h"
#include "fileio/fileioplot.h"
#include "dst/dstdiamension.h"

#include "decl/decltypedef.h"
#include "decl/declconst.h"

namespace func
{
	class Global
	{	
		static void makeplot(const TMns& mnsc, const Tfloat& radius,
			const float clock, const int count);
			
		static bool within_limits_fluid_first_type(float& proportion,
			const TMns& mnsc, const Tfloat& radius);
				
	public:
		static void simulate(const Tfloat& radius,
			TMns& mnsc, const dst::Diamension& diamension);
	};
}

#endif
