#include "func_global.h"


void GlobalFunctions::simulate(const Tfloat& radius, Tmns& mns)
{
	TFPrintMatrix("radius", radius);
	
	float clock = 0;
	int count = 10000;
	
	FPlot(mns, radius, clock, count);
	FPlotWithoutRadius(mns, count);
	
	float wetting_fluid_proportion;
	while((wetting_fluid_proportion = FMeasureWettingFluidProportion(mns, radius)) <= MAX_WETTING_PROPORTION)
	{
		std::cout << "PRS-" << count << ", clock=" << clock << ", proportion=" << wetting_fluid_proportion << std::endl;
		const auto pressure = FCalcPressure(radius, mns);
		//TFPrintMatrix("pressure", pressure, radius.size(), radius.front().size());
		
		const auto velocity = FCalcVelocity(pressure, radius, mns);
		TFPrintMatrix("velocity", velocity);

		const auto time_step = FDetermineTimeStep(mns, velocity);
		const auto volume = FCalcVolume(velocity, radius, time_step);
		
		TFPrintMatrix("volume", volume);
		
		mns = FPerformIntegration(mns, volume, velocity, radius);
		clock += time_step;
		++ count;
		FPlot(mns, radius, clock, count);
		FPlotWithoutRadius(mns, count);
	}
}
