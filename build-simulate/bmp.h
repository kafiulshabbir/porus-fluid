#ifndef DEF_BMP
#define DEF_BMP

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

struct colour
{
	int r;
	int g;
	int b;

	static const int SIZE = 3;
	
	colour();
	colour(int red, int green, int blue);
	
	std::string binary() const;
};

const colour black{0, 0, 0};
const colour grey_dark{64, 64, 64};
const colour grey{128, 128, 128};
const colour grey_light{180, 180, 180};;
const colour white{255, 255, 255};
const colour brown{128, 0, 0};
const colour red{255, 0, 0};
const colour orange{255, 165, 0};
const colour yellow{255, 255, 0};
const colour green{0, 255, 0};
const colour green_dark{0, 128, 0};
const colour blue{0, 0, 255};
const colour cyan{0, 255, 255};
const colour magenta{255, 0, 255};
const colour purple{128, 0, 128};
	
class bmp
{
	
	struct hex
	{
		int info;
		int size;
		
		int& operator() ();
		int operator() () const;
		std::string binary() const;
	};

	
	const int SIZE_HEADER = 54;
	const int SIZE_DIB_HEADER = 40;
	const int PADDING_FACTOR = 4;
	
	std::string name_file;
	
	int width;
	int height;
	int padding;
	colour colour_bg;
	colour colour_fg;
	std::vector<std::vector<colour>> matrix;
	
	int size_bitmap() const;
	int size_file() const;
	int pad(int a, int b) const;
	int determine_padding() const;
	
	template <class T>
	std::string binary(const std::vector<T>& vector) const;

	
	std::vector<hex> head() const;
	std::string binary_bitmap() const;
	std::string binary_file() const;
	void draw_rectangle(int x_from, int y_from, int x_to, int y_to, const colour& draw_colour);
	
public:
	bmp() = delete;
	bmp(int, int, const colour& bg = white);
	
	void save(const std::string& file_save_default = "out_cpp_bitmap.bmp");
	void setBgColour(const colour& bg_colour_new);
	void setFgColour(const colour& fg_colour_new);
	void drawRectangle(int x_from, int y_from, int x_to, int y_to, const colour&);
	void drawRectangle(int x_from, int y_from, int x_to, int y_to);
	void drawCentreRectangle(int, int, int, int, const colour&);
	void drawCentreRectangle(int, int, int, int);
	void drawCentreSquare(int, int, int, const colour&);
	void drawCentreSquare(int, int, int);
};

#endif
