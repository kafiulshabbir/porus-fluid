#include "func/funcglobal.h"
	
void func::Global::simulate(const Tfloat& radius, TMns& mnsc, const dst::Diamension& diamension)
{
	cmdio::Print::pmat("radius", radius);
	
	float clock = 0;
	int count = 10000;
	
	func::Global::makeplot(mnsc, radius, clock, count);
	
	float wetting_fluid_proportion;
	while(within_limits_fluid_first_type(wetting_fluid_proportion, mnsc, radius))
	{
		// step-1 PRESSURE
		const std::vector<float> pressure = func::Pressure::calculate_pressure(radius, mnsc, diamension);
		cmdio::Print::pmat("pressure", pressure, radius.size(), radius.front().size());
		
		// step-2 VELOCITY
		const auto velocity = func::Determine::calculate_velocity(pressure, radius, mnsc);
		cmdio::Print::pmat("velocity", velocity);
		
		// step-3 TIME STEP
		const auto time_step = func::TimeStep::decide_time_step(mnsc, velocity);
		
		// step-4 VOLUME
		const auto volume = func::Determine::FCalcVolume(velocity, radius, time_step);
		cmdio::Print::pmat("volume", volume);
		//std::cout << "PRS-###### " << count << ", clock=" << clock << ", proportion=" << wetting_fluid_proportion << std::endl <<std::endl;
		
		// step-5 INTEGRATION
		mnsc = func::Global::integration(mnsc, volume, velocity, radius);
		clock += time_step;
		
		if((count++) % declconst::PLOT_EACH_N)
		{
			continue;
		}
		
		//cmdio::Print::pmnsc(mnsc);
		
		func::Global::makeplot(mnsc, radius, clock, count);
		std::cout << "PRS-###### " << count << ", clock=" << clock << ", proportion=" << wetting_fluid_proportion << std::endl <<std::endl;
	}
}

void func::Global::makeplot(const TMns& mnsc, const Tfloat& radius, const float clock, const int count)
{
	fileio::Plot::with_radius(mnsc, radius, clock, count);
	fileio::Plot::without_radius(mnsc, count);
}
		
bool func::Global::within_limits_fluid_first_type(float& proportion, const TMns& mnsc, const Tfloat& radius)
{
	proportion = func::Global::FMeasureWettingFluidProportion(mnsc, radius);
	return proportion <= declconst::MAX_WETTING_PROPORTION;
}
		




