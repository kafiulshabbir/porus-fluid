#include "funcglobal.h"



void PrintInjectionVsTime(
	const std::vector<double>& time,
	const std::vector<double>& vol
	)
{
	std::ofstream fout("output_files/vol-vs-t.txt");
	fout << "time" << "\t" << "volume" << '\n';

	const int n = std::min<int>(time.size(), vol.size());
	for(int i = 0; i < n; ++ i)
	{
		fout << time[i] << '\t' << vol[i] << '\n';
	}
}

void PrintSatVSsX(
	const std::vector<double>& time,
	const std::vector<std::vector<double>>& sat
	)
{
	std::ofstream fout("output_files/sat-vs-x.txt");
	const int time_n = std::min<int>(time.size(), sat.size());
	const int x_n = sat.front().size();

	fout << "x";
	for(int i = 0; i < time_n; ++ i)
	{
		fout << '\t' << "t=" << time[i];
	}

	for(int i = 0; i < x_n; ++ i)
	{
		fout << '\n' << i;
		for(int j = 0; j < time_n; ++ j)
		{
			fout << '\t' << sat[j][i];
		}
	}
}


std::vector<double> SaturationsForEachX(const Tdouble& radius, const TMns& mnsc)
{
	const int y_total = radius.size();
	const int x_total = radius.front().size();

	std::vector<double> s_for_that_x_vec;
	for(int x_i = 0; x_i < x_total; ++ x_i)
	{
		double total = 0;
		double type_first = 0;
		for(int y_i = 0; y_i < y_total; ++ y_i)
		{
			const double r = radius[y_i][x_i];
			const dst::Mns& mns = mnsc[y_i][x_i];

			const double rsq = std::pow(r, 2);
			type_first += mns.sum_type_first() * rsq;
			total += rsq;
		}
		const double sat = type_first / total;
		s_for_that_x_vec.push_back(sat);
	}

	return s_for_that_x_vec;
}

void func::Global::simulate(const Tdouble& radius, TMns& mnsc, const dst::Diamension& diamension)
{
	double clock = 0;
	int count = 10000;
	int plot_count = 1000;
	const int steps = 1600; // 1600

	std::vector<std::vector<double>> fluid_ppr_vec;

	func::Global::makeplot(radius, mnsc, count, clock);
	//cmdio::Print::pmat("radius", radius);

	/*
	double wetting_fluid_proportion;
	while(within_limits_fluid_first_type(radius, mnsc, wetting_fluid_proportion))
	*/

	double injection_sum = 0;

	std::vector<double> fil_time;
	std::vector<std::vector<double>> fil_saturations;
	std::vector<double> fil_total_fluid_injected;

	for(int step = 0; step < steps; ++ step)
	{
		/*
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
		*/

		//std::cout << std::endl << std::endl << std::string(50, '-') << std::endl;
		//std::cout << "clock=" << clock << std::endl;

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

		// old one used to model imbibition
		//mnsc = func::Integration::integrate(radius,
		//	mnsc, velocity, volume, diamension, time_step);

		const auto integration_result = func::Integration::integrate(radius,
			mnsc, velocity, volume, diamension, time_step);

		mnsc = integration_result.new_mnsc;
		const auto fluid_injected = integration_result.fluid_injected;
		const auto fluid_expelled = integration_result.fluid_expelled;
		/*
		std::cout << "injection-table:" << '\n';
		for(size_t i = 1; i < fluid_injected.size(); i += 2)
		{
			std::cout << fluid_injected[i - 1] << ' ' << fluid_injected[i] << '\n';
		}
		std::cout << std::endl;

		std::cout << "expulsion-table:" << '\n';
		for(size_t i = 1; i < fluid_expelled.size(); i += 2)
		{
			std::cout << fluid_expelled[i - 1] << ' ' << fluid_expelled[i] << '\n';
		}
		std::cout << std::endl;
		*/

		/*
		std::cout << "[count-frame=" << count << "], [clock="
			<< clock << "], [ppr=" << wetting_fluid_proportion
			<< "]" << std::endl;
		*/

		clock += time_step;

		for(size_t i = 1; i < fluid_injected.size(); i += 2)
		{
			injection_sum += fluid_injected[i - 1];
		}

		if((count++) % declconst::PLOT_EACH_N)
		{
			continue;
		}

		//cmdio::Print::pmnsc(mnsc);
		//func::Global::makeplot(radius, mnsc, plot_count++, clock);
		//fluid_ppr_vec.push_back(fluid_ppr.val_vec());
		//fileio::Write::fluid_ppr(func::Measure::FluidPpr::header(), fluid_ppr_vec);

		const std::vector<double>& saturations_for_each_x = SaturationsForEachX(radius, mnsc);
		fil_saturations.push_back(saturations_for_each_x);
		fil_time.push_back(clock);
		fil_total_fluid_injected.push_back(injection_sum);

	}

	PrintSatVSsX(fil_time, fil_saturations);
	//PrintInjectionVsTime(fil_time, fil_total_fluid_injected);


	//fileio::Write::fluid_ppr(func::Measure::FluidPpr::header(), fluid_ppr_vec);
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





