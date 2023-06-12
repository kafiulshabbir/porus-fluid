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
		static void with_radius(TMns mnsc, Tdouble radius, double clock, int count);
	};
}
#endif
