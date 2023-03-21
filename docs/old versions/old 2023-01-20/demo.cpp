#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include "drw.h"
#include <iostream>
#include <utility>

struct typefill
{
	int n; // number of meniscuses
	std::vector<float> cord; //coordinates of the meniscuses
	bool type; //false == black wetting fluid, true == grey nonwetting fluid
	
	typefill() = default;
	typefill(int n, float c1, float c2, float c3, float c4, bool type):
		n(n), cord{c1, c2, c3, c4}, type(type) {}
};

typedef std::vector<std::vector<float>> matrix_radius;
typedef std::vector<std::vector<typefill>> matrix_fill;

const int IMAGE_SIZE = 1000;

void Draw(matrix_radius radius, matrix_fill fill)
{
	std::reverse(fill.begin(), fill.end());
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
	const int length = radius.front().size();
	const int height = radius.size();
	
	const int effective_length = image_size / (std::max(length, height) + 2);
	
	drw::bmp a(image_size, image_size);
	
	const int start_y = effective_length;
	const int start_x = effective_length;
	const int length_vector = std::sqrt(2) * effective_length;
	const float max_thick = length_vector / 4.0;
	const float min_thick = length_vector / 20.0;
	
//	a.save("demo.bmp");
	int y = start_y;
	for(int row = 0; row < fill.size(); ++ row)
	{
		const auto& w = fill[row];
		int x = start_x + effective_length * (row % 2);
		for(int col = 0; col < w.size(); ++ col)
		{
			const auto& fill_cell = w[col];
			int sign = (1 - 2 * (row % 2)) * (1 - 2 * (col % 2));
			const float r = radius[row][col];
			float thick = min_thick;
			if(max_radius != min_radius)
			{
				thick  += (r - min_radius) * (max_thick - min_thick) / (max_radius - min_radius);
			}
			a.drawVector(x, y, length_vector, thick, sign, fill_cell.n, fill_cell.cord, fill_cell.type);
			x += 2 * effective_length * (sign > 0);
		}
		
		y += effective_length;
	}
	
	a.save("demo.bmp");
}



matrix_fill ReadFlileFill(int n, int m)
{
	std::ifstream fin("fill.txt");
	
	matrix_fill fill(n, std::vector<typefill>(m));
	for(auto& row: fill)
	{
		for(auto& cell: row)
		{
			int n;
			float c1, c2, c3, c4;
			bool type;
			
			fin >> n >> c1 >> c2 >> c3 >> c4 >> type;
			cell = typefill(n, c1, c2, c3, c4, type);
		}
	}
	
	return fill;
}


matrix_radius ReadFileRadius(int n, int m)
{
	std::ifstream fin("radius.txt");
	
	std::vector<std::vector<float>> v(n, std::vector<float>(m));
	for(auto& row: v)
	{
		for(auto& x: row)
		{
			fin >> x;
		}
	}
	
	return v;
}
		
int main()
{
	const int n = 2;
	const int m = 2;
	auto fill = ReadFlileFill(n, m);
	const auto radius = ReadFileRadius(n, m);
	Draw(radius, fill);
	return 0;
}
