#ifndef FUNCDETERMINE_H
#define FUNCDETERMINE_H

#include "decl/decltypedef.h"
#include "dst/dstdiamension.h"

namespace func
{
	class Determine
	{
		static int cap_press_contb_from_direc(const int direction);
		
	public:
		
		static Tdouble determine_volume(const Tdouble& radius, const Tdouble& velocity, const double time_step, const dst::Diamension& diamension);
		
		static std::vector<std::vector<int>> gen_add_mnsc(
			const TMns& mnsc,
			const dst::Diamension& diamension
		);
	};
}

#endif
