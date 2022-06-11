#include "drw.h"

namespace drw
{
	std::vector<std::vector<bool>> ascii_matrix(int index)
	{
		std::vector<std::vector<bool>> a(128);
	
		a['0'] =
		{
			1, 1, 1,
			1, 0, 1,
			1, 0, 1,
			1, 0, 1,
			1, 1, 1
		};
		
		a['1'] =
		{
			0, 0, 1,
			0, 0, 1,
			0, 0, 1,
			0, 0, 1,
			0, 0, 1
		};
		
		a['2'] =
		{
			1, 1, 1,
			0, 0, 1,
			1, 1, 1,
			1, 0, 0,
			1, 1, 1
		};
		
		a['3'] =
		{
			1, 1, 1,
			0, 0, 1,
			1, 1, 1,
			0, 0, 1,
			1, 1, 1
		};
		
		a['4'] =
		{
			1, 0, 1,
			1, 0, 1,
			1, 1, 1,
			0, 0, 1,
			0, 0, 1
		};
		
		a['5'] =
		{
			1, 1, 1,
			1, 0, 0,
			1, 1, 1,
			0, 0, 1,
			1, 1, 1
		};
				
		a['6'] =
		{
			1, 1, 1,
			1, 0, 0,
			1, 1, 1,
			1, 0, 1,
			1, 1, 1
		};
		
				
		a['7'] =
		{
			1, 1, 1,
			0, 0, 1,
			0, 0, 1,
			0, 0, 1,
			0, 0, 1
		};
			
		a['8'] =
		{
			1, 1, 1,
			1, 0, 1,
			1, 1, 1,
			1, 0, 1,
			1, 1, 1
		};
		
		a['9'] =
		{
			1, 1, 1,
			1, 0, 1,
			1, 1, 1,
			0, 0, 1,
			1, 1, 1
		};
		
		std::vector<bool> b = a[index];
		
		std::vector<std::vector<bool>> c;
		for(int i = 0; i < 5; ++ i)
		{
			std::vector<bool> row;
			for(int j = 0; j < 3; ++ j)
			{
				row.push_back(b[i*3 + j]);
			}
			c.push_back(row);
		}
		
		return c;
	}
	
	
	
	std::string hex::binary() const noexcept
	{
		std::string s(size, '\0');
		
		for(int i = 0; i < size; ++ i)
		{
			s[i] = (value >> (length_bytes * i));
		}
		
		return s;
	}

	hex::hex() noexcept
	{
		value = 0;
		size = 0;
	}

	hex::hex(int value, int size): value(value), size(size)
	{
		if(value < 0)
		{
			throw std::invalid_argument("Value < 0");
		}
		if(size < 0 || size > 4)
		{
			throw std::invalid_argument("size of hexadecimal is invalid");
		}
	}

	vector_hex::vector_hex() = default;

	vector_hex::vector_hex(int length): v(length) {}

	hex& vector_hex::operator [] (int index)
	{
		if(index < 0 || index >= v.size())
		{
			throw std::out_of_range("index not valid");
		}
		
		return v[index];
	}

	hex vector_hex::operator [] (int index) const
	{
		if(index < 0 || index >= v.size())
		{
			throw std::out_of_range("index not valid");
		}
		
		return v[index];
	}

		
	void vector_hex::push_back(const hex& e)
	{
		v.push_back(e);
	}

	std::string vector_hex::binary() const
	{
		std::string s;
		for(const auto& x: v)
		{
			s += x.binary();
		}
		
		return s;
	}

	colour::colour()
	{
		red = 0;
		green = 0;
		blue = 0;
	}

	bool colour::invalid_colour(int rgb) const noexcept
	{
		return rgb < 0 || rgb > max_rgb_val;
	}

	colour::colour(int red, int green, int blue): red(red), green(green), blue(blue)
	{
		if(invalid_colour(red) || invalid_colour(green) || invalid_colour(blue))
		{
			throw std::invalid_argument("0 <= rgb < 256");
		}
	}

	std::string colour::binary() const
	{
		vector_hex v;
		v.push_back(hex(blue, 1));
		v.push_back(hex(green, 1));
		v.push_back(hex(red, 1));
		
		return v.binary();
	}


	int bmp::padding() const
	{
		return ((colour::size_bytes * width + PADDING_FACTOR - 1) / PADDING_FACTOR) * PADDING_FACTOR - colour::size_bytes * width;
	}

	int bmp::size_bitmap() const
	{
		return height * (width * colour::size_bytes + padding());
	}


	int bmp::size_file() const
	{
		return SIZE_HEADER + size_bitmap();
	}



	std::string bmp::binary_head() const
	{
		// BMP Header, size  = 14
		const hex initial_char_first('B', 1);
		const hex initial_char_second('M', 1);
		const hex total_file_size(size_file(), 4); //[SIZE]
		const hex application_specific_gap(0, 4);
		const hex offset_start_image(SIZE_HEADER, 4);
		
		//Dip Header, size = 40
		const hex dib_header_size(SIZE_DIB_HEADER, 4);
		const hex width_in_pixels(width, 4);
		const hex height_in_pixels(height, 4);
		
		const hex number_of_colour_planes_used(1, 2);
		const hex bits(24, 2);
		const hex compression(0, 4);
		
		const hex size_raw_bitmap(0, 4); //[SIZE] if raw bit map data including padding
		const hex resolution_horizontal(2835, 4); //print resolution in pixel/meter
		const hex resolution_vertical(2835, 4);

		const hex number_of_colours_in_palette(0, 4);
		const hex important_colours(0, 4); //0 implies that all col;ours are important
		
		vector_hex v(16);
		
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

		return v.binary();
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
			
			s += std::string(padding(), '\0');
		}
		
		return s;
	}

	void bmp::check_coordinate(int coordinate, int max) const 
	{
		if(coordinate < 0)
		{
			throw std::invalid_argument("Negative coordinate.");
		}
		if(coordinate >= max)
		{
			throw std::invalid_argument("Coordinate out of picture.");
		}
	}

	void bmp::check_width(int coordinate) const
	{
		check_coordinate(coordinate, width);
	}

	void bmp::check_height(int coordinate) const
	{
		check_coordinate(coordinate, height);
	}

	std::string bmp::binary() const
	{
		return binary_head() + binary_bitmap();
	}

	int bmp::invert_coordinate(int coordinate) const
	{
		return height - coordinate - 1;
	}

	void bmp::draw_rectangle(int x_from, int y_from, int x_to, int y_to, const colour& draw_colour)
	{
		check_width(x_from);
		check_width(x_to);
		check_height(y_from);
		check_height(y_to);
		
		int x1 = std::min(x_from, x_to);
		int x2 = std::max(x_from, x_to);
		
		int y1 = std::min(y_from, y_to);
		int y2 = std::max(y_from, y_to);
		
		//y1 = invert_coordinate(y1);
		//y2 = invert_coordinate(y2);
		
		for(int y = y1; y <= y2; ++ y)
		{
			for(int x = x1; x <= x2; ++ x)
			{
				matrix[y][x] = draw_colour;
			}
		}
	}

	bmp::bmp(const std::string& name_file, int width, int height, const colour& colour_bg): name_file(name_file), width(width), height(height), colour_bg(colour_bg), colour_fg(black), matrix(height, std::vector<colour>(width, colour_bg)){}

	bmp::~bmp()
	{
		save();
	}

	void bmp::save() const
	{
		std::ofstream fout(name_file, std::ios::binary);
		auto s = binary();
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
		draw_rectangle(x_from, y_from, x_to, y_to, colour_fg);
	}

	void bmp::drawRectangle(int x_from, int y_from, int x_to, int y_to, const colour& draw_colour)
	{
		draw_rectangle(x_from, y_from, x_to, y_to, draw_colour);
	}
	
	std::vector<std::vector<bool>> bmp::scale_char(const std::vector<std::vector<bool>>& v, int scale)
	{
		std::vector<std::vector<bool>> matrix_out;
		matrix_out.reserve(scale * v.size());
		
		for(const auto& row: v)
		{
			std::vector<bool> add_row;
			add_row.reserve(scale * v.front().size());
			
			for(auto cell: row)
			{
				for(int i = 0; i < scale; ++ i)
				{
					add_row.push_back(cell);
				}
			}
			
			for(int i = 0; i < scale; ++ i)
			{
				matrix_out.push_back(add_row);
			}
		}
		
		std::reverse(matrix_out.begin(), matrix_out.end());
		
		return matrix_out;
	}
	
	void bmp::writeChar(int x, int y, int scale, int character, const colour& colour_text)
	{
		std::vector<std::vector<bool>> matrix_char = ascii_matrix(character);
		std::vector<std::vector<bool>> w = scale_char(matrix_char, scale);
		
		for(const auto& row_char: w)
		{
			int col = x;
			for(auto cell: row_char)
			{
				if(cell)
				{
					matrix[y][col] = colour_text;
				}
				++ col;
			}
			++ y;
		}
	}
		
	
	void bmp::writeText(int x, int y, int h, const std::string& s)
	{
		writeText(x, y, h, s, colour_fg);
	}
	
	void bmp::writeText(int x, int y, int h, const std::string& s, const colour& colour_text)
	{
		int scale = h / 5;
		for(char c: s)
		{
			writeChar(x, y, scale, c, colour_text);
			x += 4 * scale;
		}
	}
}
