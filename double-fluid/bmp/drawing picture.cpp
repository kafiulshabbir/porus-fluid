namespace garbage
{
	const int WIDTH = 2000;
	const int HEIGHT = 1500;
	const double MARGIN = 0.1; // the graph will be located leaving this margin around - it is proportion to the lease diamension
	const double PROPORTION_PRESSURE_BOX = 0.25; // the maximum correspnds to the diamension of the square being equal to 50% of the seperation between the nodes
	const double PROPORTION_TUBE_THICKNESS = 0.5; // The maximum 1.0 corresponds to the largest tube being eual to the size of the box
	
	
	drw::bmp a(WIDTH, HEIGHT);
	
	const int length_tube = std::min(((1.0 - 2.0 * MARGIN) * HEIGHT) / (N - 1.0), ((1.0 - 2.0 * MARGIN) * WIDTH) / (N + 1.0));
	const int size_pressure_box = PROPORTION_PRESSURE_BOX * length_tube;
	const int length_tube_effective = length_tube - size_pressure_box;
	
	//this section constructs pressure boxes through out the image
	const int x_begin = MARGIN * WIDTH;
	const int y_begin = (1 - MARGIN) * HEIGHT;
	
	a.setFgColour(drw::black);
	
	int y = y_begin;
	for(int i = 0; i < N; ++ i)
	{
		int x = x_begin;

		for(int j = 0; j < N + 2; ++ j)
		{
			a.drawCentreSquare(x, y, size_pressure_box);
			x += length_tube;
		}
		
		y -= length_tube;
	}
	
	// prints horizontal tubes
	a.setFgColour(drw::green);
	const int x_begin_tube = x_begin + length_tube / 2;
	const int thickness_tube = PROPORTION_TUBE_THICKNESS * size_pressure_box;
	y = y_begin;
	for(int i = 0; i < N; ++ i)
	{
		int x = x_begin_tube;

		for(int j = 0; j < N + 1; ++ j)
		{
			a.drawCentreRectangle(x, y, length_tube_effective, thickness_tube);
			x += length_tube;
		}
		
		y -= length_tube;
	}
	
	// prints vertical tubes
	a.setFgColour(drw::blue);
	const int x_begin_tube_2 = x_begin + length_tube;
	y = y_begin - length_tube/2;
	for(int i = 0; i + 1 < N; ++ i)
	{
		int x = x_begin_tube_2;

		for(int j = 0; j < N; ++ j)
		{
			a.drawCentreRectangle(x, y, thickness_tube, length_tube_effective);
			x += length_tube;
		}
		
		y -= length_tube;
	}
	a.save("hello.bmp");
	
	std::cout << boundary.density_fluid_1 << '\n';
	
	
std::ofstream foutp("out-pressure.txt");
	matrix pressure_matrix(N);
	int count = 0;
	for(int i = 0; i < N; ++ i)
	{
		pressure_matrix[i].push_back(boundary.pressure_input);
		for(int j = 0; j < N; ++ j)
		{
			pressure_matrix[i].push_back(calculated_pressures[count++]);
		}
		pressure_matrix[i].push_back(boundary.pressure_output);
	}
	
	for(const auto& v: pressure_matrix)
	{
		for(auto x: v)
		{
			foutp << x << ' ';
		}
		foutp << '\n';
	}
}
