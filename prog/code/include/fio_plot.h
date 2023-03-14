#ifndef DEF_FIO_PLOT
#define DEF_FIO_PLOT

void FPlotWithoutRadius(Tmns mns, int count)
{
	std::reverse(mns.begin(), mns.end());

	const int image_size = IMAGE_SIZE;
	const int n_cols = mns.front().size();
	const int n_rows = mns.size();
	
	const int length = image_size / (std::max(n_rows, n_cols) + 2);
	
	drw::bmp a(image_size, image_size);
	
	const int start_y = length;
	const int start_x = length;
	const int thick = length / 10;
	
	
	int y = start_y;
	for(int row = 0; row < mns.size(); ++ row)
	{
		const auto& w = mns[row];
		int x = start_x + length * (row % 2);
		for(int col = 0; col < w.size(); ++ col)
		{
			int sign = ((row % 2) ^ (col % 2) ? -1 : 1);
			a.drawStrip(x, y, length, thick, sign, w[col].gen_pos_long(), w[col].type);
			x += 2 * length * (sign > 0);
		}
		y += length;
	}
	
	//a.save(FOLDER_SAVE_PIC + "pic-" + std::to_string(count) + "_t-" + std::to_string(clock) + ".bmp");
	a.save(FOLDER_SAVE_PIC + "stp-" + std::to_string(count) + ".bmp");
}



//Tested works Correctly
void FPlot(Tmns mns, Tfloat radius, float clock, int count)
{
	std::reverse(mns.begin(), mns.end());
	std::reverse(radius.begin(), radius.end());
	
	float max_radius = -1;
	float min_radius = 1e12;
	
	for(const auto& x: radius)
	{
		for(auto y: x)
		{
			max_radius = std::max(max_radius, y);
			min_radius = std::min(min_radius, y);
		}
	}

	const int image_size = IMAGE_SIZE;
	const int length = mns.front().size();
	const int height = mns.size();
	
	const int effective_length = image_size / (std::max(length, height) + 2);
	
	drw::bmp a(image_size, image_size);
	
	const int start_y = effective_length;
	const int start_x = effective_length;
	const float max_thick = effective_length;
	const float min_thick = effective_length / 6.0;
	
	
	int y = start_y;
	for(int row = 0; row < mns.size(); ++ row)
	{
		const auto& w = mns[row];
		int x = start_x + effective_length * (row % 2);
		for(int col = 0; col < w.size(); ++ col)
		{
			int sign = (1 - 2 * (row % 2)) * (1 - 2 * (col % 2));
			const float r = radius[row][col];
			float thick = min_thick;
			if(max_radius != min_radius)
			{
				thick  += (r - min_radius) * (max_thick - min_thick) / (max_radius - min_radius);
			}
			a.drawVector(x, y, effective_length, thick, sign, w[col].n, w[col].pos, w[col].type);
			x += 2 * effective_length * (sign > 0);
		}
		
		y += effective_length;
	}
	
	//a.save(FOLDER_SAVE_PIC + "pic-" + std::to_string(count) + "_t-" + std::to_string(clock) + ".bmp");
	a.save(FOLDER_SAVE_PIC + "pic-" + std::to_string(count) + ".bmp");
}

#endif
