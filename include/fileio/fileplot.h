#ifndef DEF_FILEPLOT
#define DEF_FILEPLOT

namespace fio
{
	class Plot
	{
		
	public:
		static void without_radius(TMns mnsc, int count);
		//Tested works Correctly
		static void with_radius(TMns mnsc, Tfloat radius, float clock, int count);
	};
}
#endif
