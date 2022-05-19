#include "bmp.h"

namespace drw
{
	colour::colour()
	{
		r = 0;
		g = 0;
		b = 0;
	}

	colour::colour(int red, int green, int blue): r(red), g(green), b(blue) {}

	std::string colour::binary() const
	{
		std::string s(SIZE, '\0');
		s[0] = b;
		s[1] = g;
		s[2] = r;
		
		return s;
	}

	int& bmp::hex::operator() ()
	{
		return info;
	}

	int bmp::hex::operator() () const
	{
		return info;
	}

	std::string bmp::hex::binary() const
	{
		std::string s(size, '\0');
		for(int i = 0; i < size; ++ i)
		{
			s[i] = (info >> (8 * i));
		}
		
		return s;
	}


	int bmp::size_bitmap() const
	{
		return height * (width * colour::SIZE + padding);
	}


	int bmp::size_file() const
	{
		return SIZE_HEADER + size_bitmap();
	}


	int bmp::pad(int a, int b) const
	{
		return ((a + b - 1) / b ) * b - a;
	}


	int bmp::determine_padding() const
	{
		return pad(colour::SIZE * width, PADDING_FACTOR);
	}


	template <class T>
	std::string bmp::binary(const std::vector<T>& vector) const
	{
		std::string s;
		for(const auto& unit: vector)
		{
			s += unit.binary();
		}
		
		return s;
	}

	/*
	template <class T>
	std::string binary(const std::vector< std::vector<T> >& vector)
	{
		std::string s;
		for(const auto& row: vector)
		{
			for(const auto& cell: row)
			{
				s += unit.binary();
			}
		}
		return s;
	}
	*/


	std::vector<bmp::hex> bmp::head() const
	{
		// BMP Header, size  = 14
		const hex initial_char_first{'B', 1};
		const hex initial_char_second{'M', 1};
		const hex total_file_size{size_file(), 4}; //[SIZE]
		const hex application_specific_gap{0, 4};
		const hex offset_start_image{SIZE_HEADER, 4};
		
		//Dip Header, size = 40
		const hex dib_header_size{SIZE_DIB_HEADER, 4};
		const hex width_in_pixels{width, 4};
		const hex height_in_pixels{height, 4};
		
		const hex number_of_colour_planes_used{1, 2};
		const hex bits{24, 2};
		const hex compression{0, 4};
		
		const hex size_raw_bitmap{0, 4}; //[SIZE] if raw bit map data including padding
		const hex resolution_horizontal{2835, 4}; //print resolution in pixel/meter
		const hex resolution_vertical{2835, 4};

		const hex number_of_colours_in_palette{0, 4};
		const hex important_colours{0, 4}; //0 implies that all col;ours are important
		
		std::vector<hex> v(16);
		
		v[0] = initial_char_first;
		v[1] = initial_char_second;
		v[2] = total_file_size;
		v[3] = application_specific_gap;
		v[4] = offset_start_image;
			
		v[5] = dib_header_size;
		v[6] = width_in_pixels;
		v[7] = height_in_pixels;
		
		v[8] = number_of_colour_planes_used;
		v[9] = bits;
		v[10] = compression;
		
		v[11] = size_raw_bitmap;
		v[12] = resolution_horizontal;
		v[13] = resolution_vertical;

		v[14] = number_of_colours_in_palette;
		v[15] = important_colours;

		return v;
	}


	std::string bmp::binary_bitmap() const
	{
		std::string s;
		s.reserve(size_bitmap());
		for(const auto& row: matrix)
		{
			for(const auto& x: row)
			{
				s += x.binary();
			}
			
			s += std::string(padding, '\0');
		}
		
		return s;
	}

	std::string bmp::binary_file() const
	{
		return binary(head()) + binary_bitmap();
	}

	void bmp::draw_rectangle(int x_from, int y_from, int x_to, int y_to, const colour& draw_colour)
	{
		int x1 = std::min(x_from, x_to);
		int x2 = std::max(x_from, x_to);
		
		int y1 = std::min(y_from, y_to);
		int y2 = std::max(y_from, y_to);
		
		for(int y = y1; y <= y2; ++ y)
		{
			for(int x = x1; x <= x2; ++ x)
			{
				matrix[y][x] = draw_colour;
			}
		}
	}

	bmp::bmp(int width_in_pixels, int height_in_pixels, const colour& background_fill)
	{
		width = width_in_pixels;
		height = height_in_pixels;
		padding = determine_padding();
		colour_bg = background_fill;
		colour_fg = black;
		matrix.resize(height, std::vector<colour>(width, colour_bg));
	}


	void bmp::save(const std::string& file_save)
	{
		name_file = file_save;
		
		std::cout << name_file << " successfully saved\n";
		std::ofstream fout(name_file, std::ios::binary);
		auto s = binary_file();
		fout.write(s.data(), s.size());
	}

	void bmp::setBgColour(const colour& bg_colour_new)
	{
		colour_bg = bg_colour_new;
	}

	void bmp::setFgColour(const colour& fg_colour_new)
	{
		colour_fg = fg_colour_new;
	}

	void bmp::drawRectangle(int x_from, int y_from, int x_to, int y_to)
	{
		drawRectangle(x_from, y_from, x_to, y_to, colour_fg);
	}

	void bmp::drawRectangle(int x_from, int y_from, int x_to, int y_to, const colour& draw_colour)
	{
		draw_rectangle(x_from, y_from, x_to, y_to, draw_colour);
	}

	void bmp::drawCentreRectangle(int x, int y, int diamension_x, int diamension_y, const colour& draw_colour)
	{
		diamension_x /= 2;
		diamension_y /= 2;
		draw_rectangle(x - diamension_x, y - diamension_y, x + diamension_x, y + diamension_y, draw_colour);
	}

	void bmp::drawCentreRectangle(int x, int y, int diamension_x, int diamension_y)
	{
		drawCentreRectangle(x, y, diamension_x, diamension_y, colour_fg);
	}

	void bmp::drawCentreSquare(int x, int y, int diamension, const colour& draw_colour)
	{
		drawCentreRectangle(x, y, diamension, diamension, draw_colour);
	}

	void bmp::drawCentreSquare(int x, int y, int diamension)
	{
		drawCentreSquare(x, y, diamension, colour_fg);
	}
}
