#ifndef DEF_FUNCGLOBAL
#define DEF_FUNCGLOBAL

#include "decs/simtypedefs.h"
#include "fileio/printdata.h"
#include "fileio/fileplot.h"
#include "dst/diamension.h"

namespace func
{
	class Global
	{
	public:
		static void seedRandomGenerator();
		static void simulate(const Tfloat& radius, TMns& dst::Mns, const Diamension& diamension);
	};
}

#endif // #define DEF_GLOBAL_FUNCTIONS
