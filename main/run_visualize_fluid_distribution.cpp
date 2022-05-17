#include <iostream>
#include <fstream>
#include <vector>
#include <string>

const std::string FILE_IN_NAME_FILL_DISTRIBUTION = "in_fill_distribution.txt";
const std::string FILE_IN_NAME_RADIUS_DISTRIBUTION = "in_radius_distribution.txt";
typedef std::vector<double> vec;
typedef std::vector<vec> matrix;

template <typename T>
T ReadNamedData(std::ifstream& fin)
{
	std::string s;
	T ipt;
	
	fin >> s >> ipt;
	
	return ipt;
}

matrix ReadMatrix(int n, int m, std::ifstream& fin)
{
	matrix M(n, vec(m));
	
	for(auto& row: M)
	{
		for(auto& x: row)
		{
			fin >> x;
		}
	}
	
	return M;
}

struct RadiusDistribution
{
	int n;
	double multiplier;
	double leak_radius;
	
	matrix ver;
	matrix hor;
};

struct FillDistribution
{
	matrix ver;
	matrix hor;
};

void FileOpenFailure(const std::string& file_name)
{
	std::cout << file_name << " file_open_failed and new created, restart program! \n";
	std::ofstream fout(file_name);
}

RadiusDistribution ReadRadiusDistribution()
{
	std::ifstream fin(FILE_IN_NAME_RADIUS_DISTRIBUTION);
	if(!fin.is_open())
	{
		FileOpenFailure(FILE_IN_NAME_RADIUS_DISTRIBUTION);
	}
	
	RadiusDistribution file;
	
	file.n = ReadNamedData<int>(fin);
	file.multiplier = ReadNamedData<double>(fin);
	file.leak_radius = ReadNamedData<double>(fin);
	
	file.ver = ReadMatrix(file.n - 1, file.n, fin);
	file.hor = ReadMatrix(file.n, file.n - 1, fin);
	
	return file;
}

FillDistribution ReadFillDistribution(int n)
{
	FillDistribution file;
	
	std::ifstream fin(FILE_IN_NAME_FILL_DISTRIBUTION);
	if(!fin.is_open())
	{
		FileOpenFailure(FILE_IN_NAME_FILL_DISTRIBUTION);
	}
	
	file.ver = ReadMatrix(n - 1, n, fin);
	file.hor = ReadMatrix(n, n - 1, fin);
	
	return file;
}
	
struct colour
{
	int r;
	int g;
	int b;

	static const int SIZE = 3;
	
	colour()
	{
		r = 0;
		g = 0;
		b = 0;
	}
	
	colour(int red, int green, int blue): r(red), g(green), b(blue) {}
	
	std::string binary() const
	{
		std::string s(SIZE, '\0');
		s[0] = b;
		s[1] = g;
		s[2] = r;
		
		return s;
	}
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
		
		int& operator() ()
		{
			return info;
		}
		
		int operator() () const
		{
			return info;
		}
		
		std::string binary() const
		{
			std::string s(size, '\0');
			for(int i = 0; i < size; ++ i)
			{
				s[i] = (info >> (8 * i));
			}
			
			return s;
		}
	};

	
	

	
	static const int SIZE_HEADER = 54;
	static const int SIZE_DIB_HEADER = 40;
	static const int PADDING_FACTOR = 4;
	
	int width;
	int height;
	int padding;
	colour colour_bg;
	colour colour_fg;
	
	std::vector<std::vector<colour>> matrix;
	
	int size_bitmap() const
	{
		return height * (width * colour::SIZE + padding);
	}
	
	
	int size_file() const
	{
		return SIZE_HEADER + size_bitmap();
	}
	
	
	static int pad(int a, int b)
	{
		return ((a + b - 1) / b ) * b - a;
	}
	
	
	int determine_padding() const
	{
		return pad(colour::SIZE * width, PADDING_FACTOR);
	}
	
	
	template <class T>
	static std::string binary(const std::vector<T>& vector)
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

	
	std::vector<hex> head() const
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
	
	
	std::string binary_bitmap() const
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
	
	std::string binary_file() const
	{
		return binary(head()) + binary_bitmap();
	}
	
	void draw_rectangle(int x_from, int y_from, int x_to, int y_to, const colour& draw_colour)
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
	
public:
	bmp() = delete;
	bmp(int width_in_pixels, int height_in_pixels, const colour& background_fill = white)
	{
		width = width_in_pixels;
		height = height_in_pixels;
		padding = determine_padding();
		colour_bg = background_fill;
		colour_fg = black;
		matrix.resize(height, std::vector<colour>(width, colour_bg));
	}
	
	
	~bmp()
	{
		save();
	}
	
	
	void save(const std::string& name_file_to_save = "out_cpp_bitmap.bmp") const
	{
		std::ofstream fout(name_file_to_save, std::ios::binary);
		auto s = binary_file();
		fout.write(s.data(), s.size());
	}
	
	void setBgColour(const colour& bg_colour_new)
	{
		colour_bg = bg_colour_new;
	}
	
	void setFgColour(const colour& fg_colour_new)
	{
		colour_fg = fg_colour_new;
	}
	
	void drawRectangle(int x_from, int y_from, int x_to, int y_to)
	{
		draw_rectangle(x_from, y_from, x_to, y_to, colour_fg);
	}
	
	void drawRectangle(int x_from, int y_from, int x_to, int y_to, const colour& draw_colour)
	{
		draw_rectangle(x_from, y_from, x_to, y_to, draw_colour);
	}
};
	
class coordinate
{
	int x;
	int y;
};


colour Sign(double x)
{
	if(x < 0)
	{
		return black;
	}
	
	return grey;
}

colour SignInv(double x)
{
	if(x < 0)
	{
		return grey;
	}
	
	return black;
}

int main()
{
	auto file_radius = ReadRadiusDistribution();
	const int N = file_radius.n;
	
	auto file_fill = ReadFillDistribution(N);
	
	const int WIDTH = 4000;
	const int HEIGHT = 3000;
	const double margin = 0.1;
	const int tube_thickness = 100;
	
	const int x_start = HEIGHT * margin;
	const int y_start = WIDTH * margin;
	
	int between = (HEIGHT * (1.0 - 2 * margin)) / (N - 1);
	
	bmp a(WIDTH, HEIGHT, white);
	
	/*
	int x = x_start;
	int y = y_start;
	for(int i = 0; i < N; ++ i)
	{
		a.drawRectangle(x, y, x + 100, y + 100);
		x += between;
		//y += between;
	}
	*/
	
	int y_ver = y_start;
	for(int i = 0; i + 1 < N; ++ i)
	{
		int x_ver = x_start;
		for(int j = 0; j < N; ++ j)
		{
			double proportion = file_fill.ver[i][j];
			colour fill1 = Sign(proportion);
			colour fill2 = SignInv(proportion);
			a.drawRectangle(x_ver, y_ver, x_ver + tube_thickness, y_ver + between, fill1);
			int length = std::abs(proportion) * between;
			a.drawRectangle(x_ver, y_ver, x_ver + tube_thickness, y_ver + length, fill2);
			x_ver += between;
		}
		y_ver += between;
	}
	
	int x_hor = x_start;
	for(int j = 0; j + 1 < N; ++ j)
	{
		int y_hor = y_start;
		for(int i = 0; i < N; ++ i)
		{
			double proportion = file_fill.hor[i][j];
			colour fill1 = Sign(proportion);
			colour fill2 = SignInv(proportion);
			
			a.drawRectangle(x_hor, y_hor, x_hor + between, y_hor + tube_thickness, fill1);
			int length = std::abs(proportion) * between;
			
			a.drawRectangle(x_hor, y_hor, x_hor + length, y_hor + tube_thickness, fill2);
			y_hor += between;
		}
		x_hor += between;
	}
		
	return 0;
}
