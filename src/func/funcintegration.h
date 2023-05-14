#ifndef FUNCINTEGRATION_H
#define FUNCINTEGRATION_H

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
	
	typedef std::vector<std::vector<FluidAdditionToTube>> TFluidAdditionToTube;
	
	class Integration
	{
		struct FluidAdditionToTube
		{
			std::vector<float> fluid;
			FluidAdditionToTube();
			void remove(const FluidAdditionToTube& add_fluid);
		};
	
		struct FluidAdditionResult
		{
			TFluidAdditionToTube fluid_addition_to_tube_table;
			FluidDisplacements fluid_displacements;
		};
		
		struct TubeFlowIntoFromNode
		{
			float rad;
			int row;
			int col;
		};
		
	public:
		static func::IntegrationResult integrate
		(
			const Tfloat& radius,
			const TMns& mnsc,
			const Tfloat& velocity,
			const Tfloat& volume,
			const dst::Diamension& diamension
		);
		
	};
}

#endif
