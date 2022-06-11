#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "drw.h"
typedef std::vector<double> vector;
typedef std::vector<vector> matrix;

const std::vector<drw::colour> colours{drw::brown, drw::red, drw::green_dark, drw::blue, drw::purple};
const std::vector<std::string> file_names
{
	"in-fill-distribution.txt",
	"in-radius-distribution.txt",
	"in-sigma-distribution.txt",
};

std::string MainCommandTextFileNames()
{
	std::string main_command_text_file_names;
	int index = 1;
	for(const auto& file_name: file_names)
	{
		main_command_text_file_names += std::to_string(index) + ")" + file_name + ", ";
		++ index;
	}
	
	return main_command_text_file_names;
}

bool MainCommand(int& main_command)
{
	while(true)
	{
		std::cout << "Choose distribution to generate- " << MainCommandTextFileNames() << " " << std::to_string(file_names.size() + 1) << ")other, e)xit:\n";
		
		std::string command_text;
		std::cin >> command_text;
		if(command_text.size() > 1)
		{
			std::cout << "!!!Command not recognized\n";
			continue;
		}
		char command = command_text.front();
		if(command == 'e')
		{
			return false;
		}
		command -= '1';
		if(command < 0 || command > file_names.size())
		{
			std::cout << "!!!Command not recognized\n";
			continue;
		}
		
		main_command = command;
		return true;
	}
}

class cls_data
{
	void print(const matrix& matrix_to_print, std::ostream& out) const
	{
		for(const auto& row: matrix_to_print)
		{
			for(const auto& cell: row)
			{
				out << cell << ' ';
			}
			out << '\n';
		}
	}
	
	bool between(double a, double b, double c)
	{
		return b >= a && b <= c;
	}
	
public:
	int n;
	matrix ver;
	matrix hor;
	double multiplier;
	
	matrix ver_old;
	matrix hor_old;
	cls_data(int n, double default_fill, double multiplier): n(n), ver(n - 1, vector(n, default_fill)), hor(n, vector(n + 1, default_fill)), multiplier(multiplier), hor_old(hor), ver_old(ver) {}
	void print() const
	{
		std::cout << "Vertical:\n";
		print(ver, std::cout);
		
		std::cout << "Horizontal:\n";
		print(hor, std::cout);
	}
	
	void print(const std::string& file_name) const
	{
		std::ofstream out(file_name);
		out << multiplier << "\n\n";
		
		print(ver, out);
		
		out << '\n';
		print(hor, out);
	}
	
	void ModifyByRectangle(double length)
	{
		ver_old = ver;
		hor_old = hor;
		double x_from, x_to, y_from, y_to, replace;
		
		std::cout << "Enter x_from, y_from, x_to, y_to, replace_value: ";
		std::cin >> x_from >> y_from >> x_to >> y_to >> replace;
		
		double x1 = std::min(x_from, x_to);
		double x2 = std::max(x_from, x_to);
		double y1 = std::min(y_from, y_to);
		double y2 = std::max(y_from, y_to);
		
		//Loop for vertical:
		for(int row = 0; row < ver.size(); ++ row)
		{
			for(int col = 0; col < ver[row].size(); ++ col)
			{
				double x = (col + 1) * length;
				double y = (n - row - 1.50) * length;
				
				if(between(x1, x, x2) && between(y1, y, y2))
				{
					ver[row][col] = replace;
				}
			}
		}
		
		//Loop for Horizontal:
		for(int row = 0; row < hor.size(); ++ row)
		{
			for(int col = 0; col < hor[row].size(); ++ col)
			{
				double x = (col + 0.5) * length;
				double y = (n - row - 1) * length;
				
				if(between(x1, x, x2) && between(y1, y, y2))
				{
					hor[row][col] = replace;
				}
			}
		}
	}
	
	void plot(const std::string& file_name, int count_modify);
};

void cls_data::plot(const std::string& file_name, int count_modify)
{
	const int width = 1500;
	const int height = 1000;
	const double margin = 0.1;
	drw::bmp image(file_name + ".bmp", width, height);
	image.setFgColour(drw::grey_light);
	
	
	
	const int effective_width = width * (1 - 2 * margin);
	const int effective_height = height * (1 - 2 * margin);
	const int length_tube = std::min(effective_width / (n + 1), effective_height / (n - 1));
	const double thickness_proportion = 0.1;
	const int tube_thickness = length_tube * thickness_proportion;
	
	
	const int start_y = height * (1 - margin);
	const int start_x = margin * width;
	
	
	drw::colour text_colour = colours[count_modify % colours.size()];
	drw::colour text_colour_prev = colours[(count_modify - 1)% colours.size()];
	//vertical:
	const int start_ver_x = start_x + length_tube;
	int ver_y = start_y;
	int i, j;
	
	i = 0; j = 0;
	for(const auto& row: ver)
	{
		int ver_x = start_ver_x;
		j = 0;
		for(const auto& col: row)
		{
			drw::colour text_colour_now = text_colour_prev;
			if(ver_old[i][j] != col)
			{
				text_colour_now = text_colour;
			}
			image.drawRectangle(ver_x, ver_y, ver_x + tube_thickness, ver_y - length_tube);
			image.writeText(ver_x , ver_y - length_tube / 2, 2 * tube_thickness, std::to_string(int(col)), text_colour_now);
			ver_x += length_tube;
			++ j;
		}
		
		ver_y -= length_tube;
		++ i;
	}
	
	//image.writeText(100, 100, 10, "12", drw::black);
	//horizontal:
	i = 0; j = 0;
	int hor_y = start_y;
	for(const auto& row: hor)
	{
		int hor_x = start_x;
		j = 0;
		for(const auto& col: row)
		{
			drw::colour text_colour_now = text_colour_prev;
			if(hor_old[i][j] != col)
			{
				text_colour_now = text_colour;
			}
			image.drawRectangle(hor_x, hor_y, hor_x + length_tube, hor_y + tube_thickness);
			image.writeText(hor_x + length_tube / 2, hor_y, 2 * tube_thickness, std::to_string(int(col)), text_colour_now);
			hor_x += length_tube;
			++ j;
		}
		//image.save();
		hor_y -= length_tube;
		++ i;
	}
		
			
}

bool ModifyCommand()
{
	while(true)
	{
		std::cout << "s)ave and quit, m)odify distribution: ";
		char modify_command_char;
		std::cin >> modify_command_char;
		if(modify_command_char == 's')
		{
			return false;
		}
		if(modify_command_char == 'm')
		{
			return true;
		}
		
		std::cout << "!!!Error command.\n";
	}
	return false;
}

const int modification_type_rectangle_id = 1;
const int modification_type_gauss_id = 2;
const int modification_type_circle_id = 3;

int SelectModification()
{
	while(true)
	{
		std::cout << "r)ectangle, g)auss, c)ircle: ";
		char modification_type;
		std::cin >> modification_type;
		
		switch(modification_type)
		{
			case 'r':
			return modification_type_rectangle_id;
			case 'g':
			return modification_type_gauss_id;
			case 'c':
			return modification_type_circle_id;
		}
		
		std::cout << "!!!Error command\n";
	}
	return -1;
}

void ModifyDistribution(cls_data& data)
{
	std::cout << "Enter length of a tube: ";
	double length;
	std::cin >> length;
	
	double width = length * (data.n + 1);
	double height = length * (data.n - 1);
	
	std::cout << "width = " << width << " x height = " << height << '\n';
	
	int modification_type = SelectModification();
	
	switch(modification_type)
	{
		case modification_type_rectangle_id:
			data.ModifyByRectangle(length);
			return;
		case modification_type_gauss_id:
			data.ModifyByRectangle(length);
			return;
		case modification_type_circle_id:
			data.ModifyByRectangle(length);
			return;
	}
}
	
void GenerateDistribution(const std::string& file_name)
{
	std::cout << "Enter nxn grid size: ";
	int n;
	std::cin >> n;
	
	std::cout << "Enter multiplier: ";
	double multiplier;
	std::cin >> multiplier;
	
	std::cout << "Enter default fill: ";
	double default_fill;
	std::cin >> default_fill;
	
	cls_data data(n, default_fill, multiplier);
	data.print();
	
	
	int count_modify = 0;
	data.plot(file_name, count_modify);
	
	while(ModifyCommand())
	{
		ModifyDistribution(data);
		data.print();
		data.plot(file_name, ++ count_modify);	
	}
	data.print(file_name);
	std::cout << file_name << " saved!\n\n\n";
}

int main()
{
	int main_command;
	while(MainCommand(main_command))
	{
		std::string file_name;
		if(main_command == file_names.size()) //4)other
		{
			std::cout << "Enter a .txt file name: ";
			std::cin >> file_name;
		}
		else
		{
			file_name = file_names[main_command];
		}
		GenerateDistribution(file_name);
		// Const fill, gauss fill, draw rectangle
	}
	return 0;
}
