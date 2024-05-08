#include "drw.h"

double color_trim(int val, const int height)
{
	val = std::max(val, 0);
	val = std::min(val, height);

	return val;
}

drw::colour rainbow_scale(const double val, const double min, const double max)
{
	const double height = 255;
	const double val_dash = val - (max + min) / 2.0;
	const double slope = 4.0 * height / (max - min);

	int red = slope * val_dash;
	int green = 2.0 * height - slope * std::abs(val_dash);
	int blue =-red;

	red = color_trim(red, height);
	green = color_trim(green, height);
	blue = color_trim(blue, height);

	//std::cout << "val= " << val << ", min= " << min << ", max= " << max << ", rgb= (" << red << ", " << green << ", " << blue << std::endl;
	return drw::colour(red, green, blue);
}

void plotstrip()
{
	drw::bmp a(1200, 1200);
	a.setFg(drw::black);

	const int cmin = 0;
	const int cmax = 99;

	int startx = 5;
	for(int i = 0; i < 100; ++ i)
	{
		const drw::colour c = rainbow_scale(i, cmin, cmax);
		a.setFg(c);
		a.drawRectangle(startx, 200, startx + 7, 800);
		startx += 10;
	}

	a.save("a.bmp");
}

int main()
{
	plotstrip();

	return 0;
}
