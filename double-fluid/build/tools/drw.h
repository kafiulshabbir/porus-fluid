#ifndef DEF_DRW
#define DEF_DRW

#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>

namespace drw
{
	class hex
	{
		int value;
		int size;
		static const int length_bytes = 8;
		
	public:
		hex() noexcept;
		hex(int value, int size);
		std::string binary() const noexcept;
	};
	
	class vector_hex
	{
		std::vector<hex> v;
		
	public:
		vector_hex();
		vector_hex(int length);
		hex& operator [] (int index);
		hex operator [] (int index) const;
		void push_back(const hex&);
		std::string binary() const;
	};
	

	class colour
	{
		int red;
		int green;
		int blue;
		
		static const int max_rgb_val = 255;
		
		bool invalid_colour(int rgb) const noexcept;
		
	public:
		static const int size_bytes = 3;
		
		colour();
		colour(int red, int green, int blue);
		
		std::string binary() const;
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
		
	std::vector<std::vector<bool>> ascii_matrix(int index);
	
	const int SIZE_HEADER = 54;
	const int SIZE_DIB_HEADER = 40;
	const int PADDING_FACTOR = 4;
	class bmp
	{
		std::string name_file;
		
		int width;
		int height;
		colour colour_bg;
		colour colour_fg;
		std::vector<std::vector<colour>> matrix;
		
		int padding() const;
		int size_bitmap() const;
		int size_file() const;

		std::string binary_head() const;
		std::string binary_bitmap() const;
		std::string binary() const;
		void check_coordinate(int coordinate, int max) const;
		void check_width(int coordinate) const;
		void check_height(int coordinate) const;
		int invert_coordinate(int coordinate) const;
		void draw_rectangle(int x_from, int y_from, int x_to, int y_to, const colour& draw_colour);
		std::vector<std::vector<bool>> scale_char(const std::vector<std::vector<bool>>& v, int scale);
		void writeChar(int x, int y, int scale, int charecter, const colour& colour_text);
	public:
		bmp() = delete;
		bmp(const std::string& name_file, int width, int height, const colour& bg = white);
		~bmp();
		
		void save() const;
		void setBgColour(const colour& bg_colour_new);
		void setFgColour(const colour& fg_colour_new);
		void drawRectangle(int x_from, int y_from, int x_to, int y_to, const colour&);
		void drawRectangle(int x_from, int y_from, int x_to, int y_to);
		void drawCentreRectangle(int, int, int, int, const colour&);
		void drawCentreRectangle(int, int, int, int);
		void drawCentreSquare(int, int, int, const colour&);
		void drawCentreSquare(int, int, int);
		void writeText(int x, int y, int h, const std::string& s, const colour& colour_text);
		void writeText(int x, int y, int h, const std::string& s);
	};
}
#endif
