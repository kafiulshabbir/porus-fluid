#ifndef FUNCFORSIM_H
#define FUNCFORSIM_H



namespace func
{
	
	class ForSim
	{	
	public:
		
		static TMns integration(const TMns& mnsc, const Tfloat& volume, const Tfloat& velocity, const Tfloat& radius);
		static TMns combine_additions(TMns mnsc, const Tfloat& velocity, const Tfloat& radius, const std::vector<std::vector<std::vector<float>>>& additions);
		static bool Fcomparison_outflow(const TubeWhereFlowOut& first, const TubeWhereFlowOut& second);
		static std::vector<float> FAmountVolumeToBePushedIn(float volume, std::vector<float>& tank);
		static float FMeasureWettingFluidProportion(const TMns& mnsc, const Tfloat& radius);
	};
}
#endif
