#include "func/funcglobal.h"

void FuncGlobal::simulate(const Tfloat& radius, TMns& dst::Mns, const Diamension& diamension)
{
	PrintData::print_matrix("radius", radius);
	
	float clock = 0;
	int count = 10000;
	
	FilePlot::with_radius(dst::Mns, radius, clock, count);
	FilePlot::without_radius(dst::Mns, count);
	
	float wetting_fluid_proportion;
	while((wetting_fluid_proportion = FMeasureWettingFluidProportion(dst::Mns, radius)) <= MAX_WETTING_PROPORTION)
	{
		std::cout << "PRS-" << count << ", clock=" << clock << ", proportion=" << wetting_fluid_proportion << std::endl;
		const auto pressure = FCalcPressure(radius, dst::Mns);
		//PrintData::print_matrix("pressure", pressure, radius.size(), radius.front().size());
		
		const auto velocity = FCalcVelocity(pressure, radius, dst::Mns);
		PrintData::print_matrix("velocity", velocity);

		const auto time_step = FDetermineTimeStep(dst::Mns, velocity);
		const auto volume = FCalcVolume(velocity, radius, time_step);
		
		PrintData::print_matrix("volume", volume);
		
		dst::Mns = FPerformIntegration(dst::Mns, volume, velocity, radius);
		clock += time_step;
		++ count;
		FPlot(dst::Mns, radius, clock, count);
		FPlotWithoutRadius(dst::Mns, count);
	}
}

void FSeedRandomGenerator()
{
	std::srand((unsigned)std::time(nullptr));
}
