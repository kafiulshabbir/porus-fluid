#ifndef FUNCINTEGRATION_H
#define FUNCINTEGRATION_H

#include "decl/decltypedef.h"
#include "dst/dstdiamension.h"
#include <algorithm>

namespace func
{
	struct FluidDisplacements
	{
		std::vector<float> in;
		std::vector<float> out;
		FluidDisplacements();
	};
	
	struct IntegrationResult
	{
		TMns mnsc;
		FluidDisplacements fluid_displacements;
	};
	
	
	class Integration
	{
		struct FluidAdditions
		{
			std::vector<float> fluid;
			FluidAdditions();
			void remove_fluid(const FluidAdditions& fluid_addition_table);
		};
		
		typedef std::vector<std::vector<FluidAdditions>> TFluidAdditions;
		
		struct FluidAdditionResult
		{
			TFluidAdditions fluid_addition_table;
			FluidDisplacements fluid_displacements;
		};
		
		struct Tube_FromNode
		{
			float rad;
			int row;
			int col;
		};
		
		static FluidAdditionResult calculate_fluid_addition_result
		(
			const Tfloat& radius,
			const TMns& mnsc,
			const Tfloat& velocity,
			const Tfloat& volume,
			const dst::Diamension& diamension
		);
		
		static FluidAdditions add_fluid_from_tank(const float vol, FluidAdditions& tank);
		
		static bool compare_where_wetting_fluid_go_first(const Tube_FromNode& first, const Tube_FromNode& second);
		
		static TMns combine_fluid_additions
		(
			const Tfloat& radius,
			TMns mnsc,
			const Tfloat& velocity,
			const dst::Diamension& diamension,
			const TFluidAdditions& fluid_addition_table,
			const int row_mimimum_time,
			const int col_minimum_time
		);
		
		static TMns trimmer(TMns mnsc, const Tfloat& velocity);
		
	public:
		static func::IntegrationResult integrate
		(
			const Tfloat& radius,
			const TMns& mnsc,
			const Tfloat& velocity,
			const Tfloat& volume,
			const dst::Diamension& diamension,
			const int row_mimimum_time,
			const int col_minimum_time
		);
		
	};
}

#endif
