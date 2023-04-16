#ifndef UTILITY_RANDOM_H
#define UTILITY_RANDOM_H
#include <cstdlib>

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

void utility::Random::seed()
{
	srand(std::time(nullptr));
}

int utility::Random::integer()
{
	return rand();
}

int utility::Random::integer(const int max)
{
	return integer() % max;
}

int utility::Random::integer(const int min, const int max)
{
	return integer(max - min) + min;	
}

float utility::Random::fraction(const float decimal_shift)
{
	float ans = 0;
	float divider = 1;
	for(int i = 0; i < 3; ++ i)
	{
		ans += integer(1, 10) / divider;
		divider *= 10;
	}
	
	if(integer(2))
	{
		ans *= -1;
	}
	
	return ans;
}
#endif
