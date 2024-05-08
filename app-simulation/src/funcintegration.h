#ifndef FUNCINTEGRATION_H
#define FUNCINTEGRATION_H

#include "decltypedef.h"
#include "dstdiamension.h"
#include <algorithm>

namespace func
{
	class Integration
	{
		struct Fluid
		{
			double blue = 0;
			double grey = 0;
		};

		typedef std::vector<std::vector<Fluid>> TFluid;

		struct Tube_FromNode
		{
			double rad;
			int row;
			int col;
		};

		class Tank
		{
			Fluid fluid;
			bool blue_present = false;
			bool grey_present = false;

		public:
			void add_fluid(const std::vector<double>& add);
			Fluid pour_out_fluid(const double vol);
			double show_blue() const;
			double show_grey() const;
			std::string yes_blue() const;
			std::string yes_grey() const;

		};

		struct CalculateFluidTableResult
		{
			TFluid fluid_table;
			std::vector<double> injection;
			std::vector<double> expulsion;
		};

		static CalculateFluidTableResult calculate_fluid_table(
			const Tdouble& radius,
			const TMns& mnsc,
			const Tdouble& velocity,
			const Tdouble& volume,
			const dst::Diamension& diamension,
			const double time_step
			);

		static bool compare_where_wetting_fluid_go_first(
			const Tube_FromNode& first,
			const Tube_FromNode& second
			);

		static TMns combine_fluid_additions(
			const Tdouble& radius,
			TMns mnsc,
			const Tdouble& velocity,
			const dst::Diamension& diamension,
			const TFluid& fluid_addition_table
			);


	public:
		struct IntegrationResult
		{
			TMns new_mnsc;
			std::vector<double> fluid_injected;
			std::vector<double> fluid_expelled;
		};

		static IntegrationResult integrate(
			const Tdouble& radius,
			const TMns& mnsc,
			const Tdouble& velocity,
			const Tdouble& volume,
			const dst::Diamension& diamension,
			const double time_step
			);

	};
}



#endif
