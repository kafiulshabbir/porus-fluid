#ifndef FILEPLOT_H
#define FILEPLOT_H

#include "decl/decltypedef.h"
#include "decl/declconst.h"
#include "decl/declfilename.h"
#include "drw/drw.h"

#include <algorithm>

namespace fileio
{
	class Plot
	{
		
	public:
		static void without_radius(TMns mnsc, int count);
		//Tested works Correctly
		static void with_radius(TMns mnsc, Tfloat radius, float clock, int count);
		static void with_radius2(TMns mnsc, Tfloat radius, float clock, int count);
	};
}
#endif
