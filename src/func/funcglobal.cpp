#include "func/funcglobal.h"
	
void func::Global::simulate(const Tfloat& radius, TMns& mnsc, const dst::Diamension& diamension)
{
	cmdio::Print::pmat("radius", radius);
	
	float clock = 0;
	int count = 10000;
	
	func::Global::makeplot(radius, mnsc, count, clock);
	
	float wetting_fluid_proportion;
	while(within_limits_fluid_first_type(radius, mnsc, wetting_fluid_proportion))
	{
		// step-1 PRESSURE
		const std::vector<float> pressure = func::Pressure::calculate_pressure(radius, mnsc, diamension);

		// step-2 VELOCITY
		const Tfloat velocity = func::Velocity::calculate_velocity(radius, mnsc, pressure, diamension);
			
		// step-3 TIME STEP
		const func::TimeStepResult time_step_result = func::TimeStep::decide_time_step(mnsc, velocity, diamension);
		const float time_step = time_step_result.timev[time_step_result.row][time_step_result.col];
		
		// step-4 VOLUME
		const auto volume = func::Determine::determine_volume(radius, velocity, time_step, diamension);
			
		// step-5 INTEGRATION
		const func::IntegrationResult integration_result = func::Integration::integration(radius, mnsc, velocity, volume, diamension);
		mnsc = integration_result.mnsc;
		
		// step-6 TRIMMING
		
		clock += time_step;
		
		if((count++) % declconst::PLOT_EACH_N)
		{
			continue;
		}
		
		cmdio::Print::pmat("pressure", pressure, radius.size(), radius.front().size());
		cmdio::Print::pmat("velocity", velocity);
		cmdio::Print::pmat("volume", volume);
		
		//cmdio::Print::pmnsc(mnsc);
		
		func::Global::makeplot(mnsc, radius, clock, count);
		std::cout << "PRS-###### " << count << ", clock=" << clock << ", proportion=" << wetting_fluid_proportion << std::endl <<std::endl;
	}
}

void func::Global::makeplot(const Tfloat& radius, const TMns& mnsc, const int count, const float clock)
{
	fileio::Plot::with_radius(mnsc, radius, clock, count);
	fileio::Plot::without_radius(mnsc, count);
}
		
bool func::Global::within_limits_fluid_first_type(float& proportion, const TMns& mnsc, const Tfloat& radius)
{
	proportion = func::Measure::FMeasureWettingFluidProportion(mnsc, radius);
	return proportion <= declconst::MAX_WETTING_PROPORTION;
}
		




