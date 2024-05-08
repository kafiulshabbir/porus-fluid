#include "drw.h"

namespace drw
{
	//--Class::hex------------------------------------------------------
	hex::hex(unsigned int value, unsigned int size):
		value(value), size(size) {}

	std::string hex::binary() const
	{
		std::string s(size, '\0');

		for(unsigned int i = 0; i < size; ++ i)
		{
			s[i] = (value >> (size_of_byte * i));
		}

		return s;
	}

	//--Function-vector<hex> -> binary----------------------------------
	std::string _make_binary(const std::vector<hex>& vector_hex)
	{
		std::string string_binary;
		for(const auto& elem_vector_hex: vector_hex)
		{
			string_binary += elem_vector_hex.binary();
		}

		return string_binary;
	}

	//--Class::colour---------------------------------------------------

	colour::colour(): red(0), green(0), blue(0) {}

	colour::colour(unsigned char red, unsigned char green, unsigned char blue):
	 red(red), green(green), blue(blue) {}

	std::string colour::binary() const
	{
		return _make_binary(std::vector<hex>{hex(blue, 1), hex(green, 1), hex(red, 1)});
	}

	bool colour::operator== (const colour& other) const
	{
		return red == other.red && blue == other.blue && green == other.green;
	}


	//--Class::_matrix_colours-------------------------------------------
	_matrix_colours::_matrix_colours(int width, int height, const colour& bg_colour):
		matrix(height, std::vector<colour>(width, bg_colour)) {}
	colour& _matrix_colours::operator () (int x, int y)
	{
		return matrix[y][x];
	}
	colour _matrix_colours::operator () (int x, int y) const
	{
		return matrix[y][x];
	}


	//--Class::bmp------------------------------------------------------
	///---Constructors--------------------------------------------------
	bmp::bmp(int width, int height): width(width), height(height), padding(padding_calculate()), bg_colour(white), fg_colour(black), data(width, height, bg_colour){}

	///---Binary-generators---------------------------------------------
	std::string bmp::binary_head() const
	{
		// BMP Header, size  = 14
		const hex initial_char_first('B', 1);
		const hex initial_char_second('M', 1);
		const hex total_file_size(size_file(), 4); //[SIZE]
		const hex application_specific_gap(0, 4);
		const hex offset_start_image(SIZE_HEADER_BMP, 4);

		//Dip Header, size = 40
		const hex dib_header_size(SIZE_DIB_HEADER_BMP, 4);
		const hex width_in_pixels(width, 4);
		const hex height_in_pixels(height, 4);

		const hex number_of_colour_planes_used(1, 2);
		const hex bits(24, 2);
		const hex compression(0, 4);

		const hex size_raw_bitmap(size_bitmap(), 4); //[SIZE] if raw bit map data including padding
		const hex resolution_horizontal(2835, 4); //print resolution in pixel/meter
		const hex resolution_vertical(2835, 4);

		const hex number_of_colours_in_palette(0, 4);
		const hex important_colours(0, 4); //0 implies that all col;ours are important

		return _make_binary(std::vector<hex>{initial_char_first, initial_char_second, total_file_size, application_specific_gap, offset_start_image,
			dib_header_size, width_in_pixels, height_in_pixels,	number_of_colour_planes_used, bits,	compression,
			size_raw_bitmap, resolution_horizontal, resolution_vertical, number_of_colours_in_palette, important_colours});

	}
	std::string bmp::binary_bitmap() const
	{
		std::string bitmap;
		bitmap.reserve(size_bitmap());
		for(const auto& row: data.matrix)
		{
			for(const auto& col: row)
			{
				bitmap += col.binary();
			}

			bitmap += std::string(padding, '\0');
		}

		return bitmap;
	}
	std::string bmp::binary() const
	{
		return binary_head() + binary_bitmap();
	}
	void bmp::save(const std::string& file_name) const
	{
		std::ofstream fout(file_name, std::ios::binary);
		auto file = binary();
		fout.write(file.data(), file.size());
	}

	///---Size-generators-----------------------------------------------
	int bmp::size_bitmap() const
	{
		return height * (width * colour::size_colour_bytes + padding);
	}
	int bmp::size_file() const
	{
		return SIZE_HEADER_BMP + size_bitmap();
	}
	int bmp::padding_calculate() const
	{
		return ((colour::size_colour_bytes * width + PADDING_FACTOR_BMP - 1) / PADDING_FACTOR_BMP) * PADDING_FACTOR_BMP - colour::size_colour_bytes * width;
	}

	///---Tools---------------------------------------------------------

	void bmp::setBg(const colour& bg_colour_new)
	{
		bg_colour = bg_colour_new;
	}
	void bmp::setFg(const colour& fg_colour_new)
	{
		fg_colour = fg_colour_new;
	}

	///---Drawing-------------------------------------------------------

	void bmp::drawRectangle(int x1, int y1, int x2, int y2)
	{
		int xmin = std::min(x1, x2);
		int xmax = std::max(x1, x2);

		int ymin = std::min(y1, y2);
		int ymax = std::max(y1, y2);

		for(int x = xmin; x <= xmax; ++ x)
		{
			for(int y = ymin; y <= ymax; ++ y)
			{
				data(x, y) = fg_colour;
			}
		}
	}

	void bmp::drawCentreRectangle(int x, int y, int width, int height)
	{
		drawRectangle(x - width / 2, y - width / 2, x + width / 2, y + height / 2);
	}
	void bmp::drawCentreSquare(int x, int y, int diamension)
	{
		drawCentreRectangle(x, y, diamension, diamension);
	}

	void bmp::drawVector(int x, int y, int effective_length, int thick, int sign, int number_of_meniscus, const std::vector<double>& coordinates_of_meniscus, bool type)
	{
		int displacement = effective_length;
		int lateral_length = thick;

		std::vector<colour> colour_pipe{grey, grey_light};

		std::vector<int> coordinates(number_of_meniscus + 2);

		for(int i = 1; i <= number_of_meniscus; ++ i)
		{
			coordinates[i] = coordinates_of_meniscus[i - 1] * displacement;
		}

		coordinates.back() = displacement;

		int current_colour = type;

		for(size_t k = 1; k < coordinates.size(); ++ k)
		{
			int start_x = coordinates[k - 1];
			int end_x = coordinates[k];
			for(int i = start_x; i <= end_x; ++ i)
			{
				for(int j = -lateral_length / 2; 2 * j <= lateral_length; ++ j)
				{
					data(x + (sign * i) + j, y + i) = colour_pipe[current_colour];
				}
			}

			current_colour = (current_colour + 1) % 2;
		}
	}

	void bmp::drawStrip(const int x, const int y, int length, const int thick, const int sign, std::vector<double> pos_mns, const bool type)
	{
		std::vector<colour> colour_pipe{grey, grey_light};
		length -= thick;

		for(auto& cell: pos_mns)
		{
			cell *= length;
		}

		for(size_t k = 1; k < pos_mns.size(); ++ k)
		{
			int start_x = pos_mns[k - 1];
			int end_x = pos_mns[k];

			for(int i = start_x; i <= end_x; ++ i)
			{
				for(int j = -thick / 2; 2 * j <= thick; ++ j)
				{
					data(x + sign * (i + thick/2) + j, y + i + thick/2 - sign * j) = colour_pipe[(k + 1 + type) % 2];
					data(1 + x + sign * (i + thick/2) + j, y + i + thick/2 - sign * j) = colour_pipe[(k + 1 + type) % 2];
				}
			}
		}
	}
}
