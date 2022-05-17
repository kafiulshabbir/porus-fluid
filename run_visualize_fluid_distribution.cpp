
	

	
class coordinate
{
	int x;
	int y;
};


colour Sign(double x)
{
	if(x < 0)
	{
		return black;
	}
	
	return grey;
}

colour SignInv(double x)
{
	if(x < 0)
	{
		return grey;
	}
	
	return black;
}

int main()
{
	auto file_radius = ReadRadiusDistribution();
	const int N = file_radius.n;
	
	auto file_fill = ReadFillDistribution(N);
	
	const int WIDTH = 4000;
	const int HEIGHT = 3000;
	const double margin = 0.1;
	const int tube_thickness = 100;
	
	const int x_start = HEIGHT * margin;
	const int y_start = WIDTH * margin;
	
	int between = (HEIGHT * (1.0 - 2 * margin)) / (N - 1);
	
	bmp a(WIDTH, HEIGHT, white);
	
	/*
	int x = x_start;
	int y = y_start;
	for(int i = 0; i < N; ++ i)
	{
		a.drawRectangle(x, y, x + 100, y + 100);
		x += between;
		//y += between;
	}
	*/
	
	int y_ver = y_start;
	for(int i = 0; i + 1 < N; ++ i)
	{
		int x_ver = x_start;
		for(int j = 0; j < N; ++ j)
		{
			double proportion = file_fill.ver[i][j];
			colour fill1 = Sign(proportion);
			colour fill2 = SignInv(proportion);
			a.drawRectangle(x_ver, y_ver, x_ver + tube_thickness, y_ver + between, fill1);
			int length = std::abs(proportion) * between;
			a.drawRectangle(x_ver, y_ver, x_ver + tube_thickness, y_ver + length, fill2);
			x_ver += between;
		}
		y_ver += between;
	}
	
	int x_hor = x_start;
	for(int j = 0; j + 1 < N; ++ j)
	{
		int y_hor = y_start;
		for(int i = 0; i < N; ++ i)
		{
			double proportion = file_fill.hor[i][j];
			colour fill1 = Sign(proportion);
			colour fill2 = SignInv(proportion);
			
			a.drawRectangle(x_hor, y_hor, x_hor + between, y_hor + tube_thickness, fill1);
			int length = std::abs(proportion) * between;
			
			a.drawRectangle(x_hor, y_hor, x_hor + length, y_hor + tube_thickness, fill2);
			y_hor += between;
		}
		x_hor += between;
	}
		
	return 0;
}
