#include "func/funcglobal.h"
	
void func::Global::simulate(const Tdouble& radius, TMns& mnsc, const dst::Diamension& diamension)
{
	double clock = 0;
	int count = 10000;
	const int steps = 5000;
	
	std::vector<std::vector<double>> fluid_ppr_vec;
	
	//func::Global::makeplot(radius, mnsc, count, clock);
	//cmdio::Print::pmat("radius", radius);
	
	/*
	double wetting_fluid_proportion;
	while(within_limits_fluid_first_type(radius, mnsc, wetting_fluid_proportion))
	*/
	
	for(int step = 0; step < steps; ++ step)
	{
		const func::Measure::FluidPpr fluid_ppr
			= func::Measure::fluid_ppr(
				radius,
				mnsc,
				clock,
				declconst::ROW_THIN_B,
				declconst::COL_THIN_B,
				declconst::ROW_THIN_E,
				declconst::COL_THIN_E,
				diamension
			);
			
		
		
		//std::cout << std::endl << std::endl << std::string(50, '-') << std::endl;
		
		// step-0 Generate add_msn table
		
		const std::vector<std::vector<int>> add_msn
			= func::Determine::gen_add_mnsc(mnsc, diamension);
			
		// step-1 PRESSURE
		const std::vector<double> pressure 
			= func::Pressure::calculate_pressure(
				radius, mnsc, add_msn, diamension);
		
		//cmdio::Print::pmat("pressure", pressure, radius.size(), radius.front().size());
		
		// step-2 VELOCITY
		const Tdouble velocity
			= func::Velocity::calculate_velocity(
				radius, mnsc, add_msn, pressure, diamension);
		
		//cmdio::Print::pmat("velocity", velocity);
			
		// step-3 TIME STEP
		const double time_step
			= func::TimeStep::decide_time_step(
				velocity, diamension);
		
		// step-4 VOLUME
		const auto volume
			= func::Determine::determine_volume(radius,
				velocity, time_step, diamension);
		
		//cmdio::Print::pmat("volume", volume);
		
		// step-5 INTEGRATION

		mnsc = func::Integration::integrate(radius,
			mnsc, velocity, volume, diamension, time_step);
		
		/*
		std::cout << "[count-frame=" << count << "], [clock="
			<< clock << "], [ppr=" << wetting_fluid_proportion
			<< "]" << std::endl;
		*/
			
		clock += time_step;
		
		if((count++) % declconst::PLOT_EACH_N)
		{
			continue;
		}

		//cmdio::Print::pmnsc(mnsc);
		func::Global::makeplot(radius, mnsc, count, clock);
		fluid_ppr_vec.push_back(fluid_ppr.val_vec());
		
	}
	
	fileio::Write::fluid_ppr(func::Measure::FluidPpr::header(), fluid_ppr_vec);
}

void func::Global::makeplot(const Tdouble& radius, const TMns& mnsc, const int count, const double clock)
{
	fileio::Plot::with_radius(mnsc, radius, clock, count);
	fileio::Plot::without_radius(mnsc, count);
}
		
bool func::Global::within_limits_fluid_first_type(const Tdouble& radius, const TMns& mnsc, double& proportion)
{
	proportion = func::Measure::measure_wetting_fluid_proportion(radius, mnsc);
	return proportion <= declconst::MAX_WETTING_PROPORTION;
}
		




