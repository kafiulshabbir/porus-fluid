#ifndef FUNCGLOBAL_H
#define FUNCGLOBAL_H


#include "cmdio/cmdioprint.h"
#include "fileio/fileioplot.h"
#include "dst/dstdiamension.h"
#include "func/funcdetermine.h"

#include "decl/decltypedef.h"
#include "func/funcconnection.h"
#include "decl/declconst.h"

#include <cstdio>
#include <ctime>
#include <algorithm>

namespace func
{
	struct IntegrationResult
	{
		TMns mnsc;
		float fluid1in;
		float fluid1out;
		float fluid2in;
		float fluid2out;
	};
		
	struct TubeWhereFlowOut
	{
		float radius;
		int r;
		int c;
	};
		
	class Global
	{
		static TMns integration(const TMns& mnsc, const Tfloat& volume,
			const Tfloat& velocity, const Tfloat& radius);
			
		static TMns combine_additions(TMns mnsc, const Tfloat& velocity,
			const Tfloat& radius, const std::vector<Tfloat>& additions);
			
		static bool Fcomparison_outflow(const TubeWhereFlowOut& first,
			const TubeWhereFlowOut& second);
		 
		static std::vector<float> FAmountVolumeToBePushedIn(
			float volume, std::vector<float>& tank);
			
		static float FMeasureWettingFluidProportion(const TMns& mnsc,
			const Tfloat& radius);
		
	public:
		static void SeedRandomGenerator();
		static void simulate(const Tfloat& radius,
			TMns& mnsc, const dst::Diamension& diamension);
	};
}

#endif // #define DEF_GLOBAL_FUNCTIONS
