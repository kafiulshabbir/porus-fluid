#ifndef DEF_DRW
#define DEF_DRW

#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include "drw_text.h"

namespace drw
{
	class hex
	{
		unsigned int value;
		unsigned int size;
		static const unsigned int size_of_byte = 8;
		
	public:
		hex() = delete;
		hex(unsigned int value, unsigned int size);
		std::string binary() const;
	};
	
	std::string make_binary(const std::vector<hex>& vector_hex);
	
	class colour
	{
		unsigned char red;
		unsigned char green;
		unsigned char blue;
			
	public:
		static const int size_colour_bytes = 3;
		
		colour();
		colour(unsigned char red, unsigned char green, unsigned char blue);
		
		std::string binary() const;
		bool operator==(const colour& other) const;
	};

	const colour black(0, 0, 0);
	const colour grey_dark(64, 64, 64);
	const colour grey(128, 128, 128);
	const colour grey_light(180, 180, 180);
	const colour white(255, 255, 255);
	const colour brown(128, 0, 0);
	const colour red(255, 0, 0);
	const colour orange(255, 165, 0);
	const colour yellow(255, 255, 0);
	const colour green(0, 255, 0);
	const colour green_dark(0, 128, 0);
	const colour blue(0, 0, 255);
	const colour cyan(0, 255, 255);
	const colour magenta(255, 0, 255);
	const colour purple(128, 0, 128);
	
	
	struct matrix_colours
	{
		std::vector<std::vector<colour>> matrix;
		matrix_colours() = delete;
		matrix_colours(int height, int width, const colour& bg_colour);
		
		colour& operator () (int row, int col);
		colour operator () (int row, int col) const;
		
		int height() const;
		int width() const;
	};
	
	
	class bmp
	{
		const std::string file_name;
		const int height;
		const int width;
		const int padding;
		colour bg_colour;
		colour fg_colour;
		matrix_colours main_matrix;
		
		static const int SIZE_HEADER_BMP = 54;
		static const int SIZE_DIB_HEADER_BMP = 40;
		static const int PADDING_FACTOR_BMP = 4;
		
		int padding_calculate() const;
		int size_bitmap() const;
		int size_file() const;

		std::string binary_head() const;
		std::string binary_bitmap() const;
		std::string binary() const;
		
		void intercept(int row, int col, const matrix_colours& cover_matrix, bool transperency = true);
		matrix_colours make_char(int symbol, int scale, const colour& colour_text, const colour& colour_text_back);
		void write_char(int row, int col, int scale, int symbol, const colour& colour_text, const colour& colour_text_back, bool transperency);
		void write_text(int row, int col, int height, const std::string& text, const colour& colour_text, const colour& colour_text_back, bool transperency);
		
	public:
		bmp() = delete;
		bmp(const std::string& file_name, int height, int width, const colour& bg = white);
		~bmp();
		
		void save() const;
		
		void setBgColour(const colour& bg_colour_new);
		void setFgColour(const colour& fg_colour_new);
		
		void drawRectangle(int row_from, int col_from, int row_to, int col_to);
		void drawRectangle(int row_from, int col_from, int row_to, int col_to, const colour& colour_draw);
		void drawCentreRectangle(int row_centre, int col_centre, int height, int width);
		void drawCentreRectangle(int row_centre, int col_centre, int height, int width, const colour& colour_draw);
		
		void drawCentreSquare(int row_centre, int col_centre, int diamension);
		void drawCentreSquare(int row_centre, int col_centre, int diamension, const colour& colour_draw);
		
		void writeText(int row, int col, int height, const std::string& text);
		void writeText(int row, int col, int height, const std::string& text, const colour& colour_text);
		void writeTextSolid(int row, int col, int height, const std::string& text);
		void writeTextSolid(int row, int col, int height, const std::string& text, const colour& colour_text);
		void writeTextSolid(int row, int col, int height, const std::string& text, const colour& colour_text, const colour& colour_text_back);
	};
}
#endif
