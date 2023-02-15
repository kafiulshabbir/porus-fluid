#ifndef DEF_DRW
#define DEF_DRW

#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cmath>

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
	
	std::string _make_binary(const std::vector<hex>& vector_hex);
	
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
	
	
	struct _matrix_colours
	{
		std::vector<std::vector<colour>> matrix;
		_matrix_colours() = delete;
		_matrix_colours(int width, int height, const colour& bg_colour);
		
		colour& operator () (int x, int y);
		colour operator () (int x, int y) const;
	};
	
	
	class bmp
	{
		const int width;
		const int height;
		const int padding;
		colour bg_colour;
		colour fg_colour;
		_matrix_colours data;
		
		static const int SIZE_HEADER_BMP = 54;
		static const int SIZE_DIB_HEADER_BMP = 40;
		static const int PADDING_FACTOR_BMP = 4;
		
		int padding_calculate() const;
		int size_bitmap() const;
		int size_file() const;

		std::string binary_head() const;
		std::string binary_bitmap() const;
		std::string binary() const;
		
	public:
		bmp() = delete;
		bmp(int width, int height);
		
		void save(const std::string& file_name) const;
		
		void setBg(const colour& bg_colour_new);
		void setFg(const colour& fg_colour_new);
		
		void drawRectangle(int x1, int y1, int x2, int y2);
		void drawCentreRectangle(int x, int y, int width, int height);
		void drawCentreSquare(int x, int y, int diamension);
		void drawVector(int x, int y, int effective_length, int thick, int sign, int n_mns, const std::vector<real>& pos, bool type);
	};
}
#endif
