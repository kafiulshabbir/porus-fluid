#include "func/funcglobal.h"
	
void func::Global::simulate(const Tfloat& radius, TMns& mnsc, const dst::Diamension& diamension)
{
	float clock = 0;
	int count = 10000;
	
	func::Global::makeplot(radius, mnsc, count, clock);
	
	float wetting_fluid_proportion;
	while(within_limits_fluid_first_type(radius, mnsc, wetting_fluid_proportion))
	{
		//std::cout << std::endl << std::endl << std::string(50, '-') << std::endl;
		std::cout << "-Rel-sat, [frm-n=" << count << "], [clk=" << clock << "], [ppr=" << wetting_fluid_proportion << "]" << std::endl;
			cmdio::Print::pmat("radius", radius);
			cmdio::Print::pmnsc(mnsc);
		
		// step-1 PRESSURE
		const std::vector<float> pressure = func::Pressure::calculate_pressure(radius, mnsc, diamension);
			//cmdio::Print::pmat("pressure", pressure, radius.size(), radius.front().size());
		
		// step-2 VELOCITY
		const Tfloat velocity = func::Velocity::calculate_velocity(radius, mnsc, pressure, diamension);
			cmdio::Print::pmat("velocity", velocity);
			
		// step-3 TIME STEP
		const func::TimeStepResult time_step_result = func::TimeStep::decide_time_step(mnsc, velocity, diamension);
		const float time_step = time_step_result.time_step();
		
			std::cout << "[min_time=" << time_step << "], at [(row, col)=(" << time_step_result.row << ", " << time_step_result.col << ")]" << std::endl;
			cmdio::Print::pmat("time", time_step_result.timev);
		
		// step-4 VOLUME
		const auto volume = func::Determine::determine_volume(radius, velocity, time_step, diamension);
			cmdio::Print::pmat("volume", volume);
		
		// step-5 INTEGRATION
		int special_row = -1;
		int special_col = -1;
		if(time_step_result.does_mns_reach_node)
		{
			special_row = time_step_result.row;
			special_col = time_step_result.col;
		}
		
		const func::IntegrationResult integration_result =
			func::Integration::integrate(radius,
			mnsc, velocity, volume, diamension,
			special_row, special_col);
			
		mnsc = integration_result.mnsc;
		
		// step-6 TRIMMING
		
		clock += time_step;
		
		if((count++) % declconst::PLOT_EACH_N)
		{
			continue;
		}

		cmdio::Print::pmnsc(mnsc);

		func::Global::makeplot(radius, mnsc, count, clock); 
		
	}
}

void func::Global::makeplot(const Tfloat& radius, const TMns& mnsc, const int count, const float clock)
{
	fileio::Plot::with_radius(mnsc, radius, clock, count);
	fileio::Plot::without_radius(mnsc, count);
}
		
bool func::Global::within_limits_fluid_first_type(const Tfloat& radius, const TMns& mnsc, float& proportion)
{
	proportion = func::Measure::measure_wetting_fluid_proportion(radius, mnsc);
	return proportion <= declconst::MAX_WETTING_PROPORTION;
}
		




