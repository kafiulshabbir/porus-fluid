#ifndef FUNCINTEGRATION_H
#define FUNCINTEGRATION_H

namespace func
{
	struct TubeWhereFlowOut
	{
		float radius;
		int r;
		int c;
	};
	
	struct IntegrationResult
	{
		TMns mnsc;
		float fluid1in;
		float fluid1out;
		float fluid2in;
		float fluid2out;
	};
		
		
	class Integration
	{
		
	public:
		static func::IntegrationResult integration(
			const Tfloat& radius,
			const TMns& mnsc,
			const Tfloat& velocity,
			const Tfloat& volume,
			const dst::Diamension& diamension
		);
		
	};
}

#endif
