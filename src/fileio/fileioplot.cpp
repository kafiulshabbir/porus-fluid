#include "fileio/fileioplot.h"

void fileio::Plot::without_radius(TMns mnsc, int count)
{
	//std::cout << "crashed here 1002 - without radius " << std::endl;
	
	std::reverse(mnsc.begin(), mnsc.end());

	const int image_size = declconst::IMAGE_SIZE;
	const int n_cols = mnsc.front().size();
	const int n_rows = mnsc.size();
	
	const int length = image_size / (std::max(n_rows, n_cols) + 2);
	
	drw::bmp a(image_size, image_size);
	
	const int start_y = length;
	const int start_x = length;
	const int thick = length / 10;
	
	
	int y = start_y;
	for(int row = 0; row < n_rows; ++ row)
	{
		const auto& w = mnsc[row];
		int x = start_x + length * (row % 2);
		for(int col = 0; col < n_cols; ++ col)
		{
			int sign = ((row % 2) ^ (col % 2) ? -1 : 1);
			a.drawStrip(x, y, length, thick, sign, w[col].gen_pos_long(), w[col].type);
			x += 2 * length * (sign > 0);
		}
		y += length;
	}
	
	//a.save(FOLDER_SAVE_PIC + "pic-" + std::to_string(count) + "_t-" + std::to_string(clock) + ".bmp");
	a.save(declfilename::FOLDER_PLOTS + "stp-" + std::to_string(count) + ".bmp");
}



//Tested works Correctly
void fileio::Plot::with_radius(TMns mnsc, Tfloat radius, float clock, int count)
{
	//std::cout << "crashed here 1001" << std::endl;
	const int rows = radius.size();
	const int cols = radius.front().size();
	std::reverse(mnsc.begin(), mnsc.end());
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
	
	//std::cout << "crashed here 1002" << std::endl;
	
	const int image_size = declconst::IMAGE_SIZE;
	const int length = mnsc.front().size();
	const int height = mnsc.size();
	
	const int effective_length = image_size / (std::max(length, height) + 2);
	
	drw::bmp a(image_size, image_size);
	
	const int start_y = effective_length;
	const int start_x = effective_length;
	const float max_thick = effective_length / 2.0;
	const float min_thick = effective_length / 6.0;
	
	//std::cout << "crashed here 1003" << std::endl;
	
	int y = start_y;
	for(int row = 0; row < rows; ++ row)
	{
		const auto& w = mnsc[row];
		int x = start_x + effective_length * (row % 2);
		for(int col = 0; col < cols; ++ col)
		{
			int sign = (1 - 2 * (row % 2)) * (1 - 2 * (col % 2));
			const float r = radius[row][col];
			float thick = min_thick;
			if(max_radius != min_radius)
			{
				thick  += (r - min_radius) * (max_thick - min_thick) / (max_radius - min_radius);
			}
			//std::cout << std::endl << "#####crashed here 1005" << std::endl;
			//std::cout << "row=" << row << " col=" << col << std::endl;
			a.drawVector(x, y, effective_length, thick, sign, w[col].n, w[col].pos, w[col].type);
			//std::cout << "crashed here 1006" << std::endl;
			x += 2 * effective_length * (sign > 0);
		}
		
		y += effective_length;
	}
	
	//std::cout << "crashed here 1004" << std::endl;
	
	//a.save(FOLDER_SAVE_PIC + "pic-" + std::to_string(count) + "_t-" + std::to_string(clock) + ".bmp");
	a.save(declfilename::FOLDER_PLOTS + "pic-" + std::to_string(count) + ".bmp");
}
