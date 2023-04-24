#ifndef UTILITY_RANDOM_H
#define UTILITY_RANDOM_H

#include <cstdlib>
#include <ctime>

namespace utility
{
	class Random
	{

	public:
		static void seed();
		static int integer();
		static int integer(const int max);
		static int integer(const int min, const int max);
		static float fraction(const float decimal_shift);
		
	};
}

#endif
