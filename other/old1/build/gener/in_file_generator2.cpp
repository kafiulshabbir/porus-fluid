#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <utility>
#include <string>
#include <stdexcept>

struct Diamension
{
	int n_row;
	int m_col;
	
	Diamension(): n_row(0), m_col(0) {}
	Diamension(int n_row, int m_col): n_row(n_row), m_col(m_col) {}
	Diamension(const Diamension& other): n_row(other.n_row), m_col(other.m_col) {}
	size_t size() const
	{
		return n_row * m_col;
	}
};

struct GroupDiamension
{
	Diamension ver;
	Diamension hor;
	
	GroupDiamension() = default;
	GroupDiamension(const Diamension& ver, const Diamension& hor): ver(ver), hor(hor) {}
	size_t size() const
	{
		return ver.size() + hor.size();
	}
};

struct DefaultValue
{
	double multiplier;
	double value;
	
	DefaultValue(): multiplier(-1), value(-1) {}
	DefaultValue(double multiplier, double value): multiplier(multiplier), value(value) {}
};

struct FileProperty
{
	DefaultValue default_value;
	GroupDiamension group_diamension;
	
	FileProperty() = default;
	FileProperty(const DefaultValue& default_value, const GroupDiamension& group_diamension): default_value(default_value), group_diamension(group_diamension) {}
	size_t size() const
	{
		return group_diamension.size() + 1;
	}
};

void PrintMatrix(std::ostream& out, const std::vector<std::vector<double>>& matrix)
{
	out << '\n';
	for(const auto& row: matrix)
	{
		for(const auto& cell: row)
		{
			out << cell << ' ';
		}
		
		out << '\n';
	}
}

struct File
{
	double multiplier;
	std::vector<std::vector<double>> matrix1;
	std::vector<std::vector<double>> matrix2;
	
	File() = default;
	File(const FileProperty& file_property):
		multiplier(file_property.default_value.multiplier),
		matrix1(file_property.group_diamension.ver.n_row, std::vector<double>(file_property.group_diamension.ver.m_col, file_property.default_value.value)),
		matrix2(file_property.group_diamension.hor.n_row, std::vector<double>(file_property.group_diamension.hor.m_col, file_property.default_value.value))
		{}
	
	void update(const std::vector<double>& file_vector)
	{
		multiplier = file_vector.front();
		
		int count = 0;
		for(auto& row: matrix1)
		{
			for(auto& col: row)
			{
				col = file_vector[++ count];
			}
		}
		
		for(auto& row: matrix2)
		{
			for(auto& col: row)
			{
				col = file_vector[++ count];
			}
		}
		
	}
		
	std::vector<double> linear_matrix(const std::vector<std::vector<double>>& matrix) const
	{
		std::vector<double> linear_vector;
		for(const auto& row: matrix)
		{
			linear_vector.insert(linear_vector.end(), row.cbegin(), row.cend());
		}
		
		return linear_vector;
	}
	
	std::vector<double> linear() const
	{
		std::vector<double> file_vector(multiplier);
		auto matrix1l = linear_matrix(matrix1);
		file_vector.insert(file_vector.end(), matrix1l.cbegin(), matrix1l.cend());
		auto matrix2l = linear_matrix(matrix2);
		file_vector.insert(file_vector.end(), matrix2l.cbegin(), matrix2l.cend());
		return file_vector;
	}
	
	bool operator == (const std::vector<double>& file_vector) const
	{
		auto vec = linear();
		
		if(vec.size() != file_vector.size())
		{
			return false;
		}
		
		for(size_t i = 0; i < vec.size(); ++ i)
		{
			if(vec[i] != file_vector[i])
			{
				return false;
			}
		}
		
		return true;
	}
	
	
	void write(const std::string& file_name) const
	{
		std::ofstream fout(file_name + ".txt");
		
		fout << multiplier << '\n';
		
		PrintMatrix(fout, matrix1);
		PrintMatrix(fout, matrix2);
	}
};

template <typename T>
T Input(const std::string& hint_text_for_input)
{
	std::cout << "Enter " << hint_text_for_input << ": ";
	T input_value;
	std::cin >> input_value;
	return input_value;
}
Diamension ReadDiamension();
void Print(const std::string& text_to_print, int tab_size = 0);
void PrintStatusOfInFiles(const std::map<std::string, std::string>& status_of_files);
void PrintListOfFiles(const std::vector<std::string>& in_file_names);
std::map<std::string, std::string> StatusOfInFiles(const std::map<std::string, FileProperty>& file_name_and_property, std::map<std::string, File>& file_images);

std::vector<std::pair<std::string, double>> OtherParametersDefaultValues(const Diamension& diamension);
std::map<std::string, GroupDiamension> FileTypeAndGroupDiamension(const Diamension& diamension);
std::map<std::string, DefaultValue> FileTypeAndDefaultValues();
std::map<std::string, FileProperty> FileNameAndProperty(const std::vector<std::string>& file_names, const std::map<std::string, GroupDiamension>& file_type_and_group_diamension, const std::map<std::string, DefaultValue>& file_type_and_default_value);
std::map<std::string, File> FileImagesDefault(const std::map<std::string, FileProperty>& file_name_and_property);

template <class T>
T AssignProperty(const std::string& file_name, const std::map<std::string, T>& type_to_property)
{
	for(const auto& [type, property]: type_to_property)
	{
		if(file_name.find(type) != std::string::npos)
		{
			return property;
		}
	}
	
	throw std::invalid_argument("Unrecognized file name");
	return T();
}


const std::string file_status_bad_nonexistent = "BAD=NONEXISTENT";
const std::string file_status_bad_corrupted = "BAD=CORRUPTED";
const std::string file_status_okay_default = "OKAY=DEFAULT";
const std::string file_status_okay_edited = "OKAY=EDITED";
const std::string file_status_okay_custom = "OKAY=CUSTOM";

void GenerateOtherParametersDefault(const std::string& name_of_file)
{
	std::cout << "other parameters!";
}


void SetDefaultAll(const std::map<std::string, FileProperty>& file_name_and_property)
{
	for(const auto& [file_name, property]: file_name_and_property)
	{
		if(file_name.find("other_parameters") != std::string::npos)
		{
			Print("shhiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiit");
		}
		else
		{
			File file_print(property);
			file_print.write(file_name);
		}
	}
}


int main()
{
	const std::vector<std::string> in_file_names
	{
		"in_01_other_parameters",
		"in_02_pressure_input",
		"in_03_pressure_output",
		"in_04_radius_distribution",
		"in_05_fill_distribution",
		"in_06_sigma_distribution"
	};
	
	
	Print("Running program: in_file_generator");
	
	const Diamension diamension(ReadDiamension());
	const auto file_type_and_group_diamensions = FileTypeAndGroupDiamension(diamension);
	const auto file_type_and_default_values = FileTypeAndDefaultValues();
	const auto file_name_and_property = FileNameAndProperty(in_file_names, file_type_and_group_diamensions, file_type_and_default_values);
	auto file_images = FileImagesDefault(file_name_and_property);
	
	bool stay_in_edit_menu = true;
	while(stay_in_edit_menu)
	{
		auto status_of_in_files = StatusOfInFiles(file_name_and_property, file_images);
		PrintStatusOfInFiles(status_of_in_files);
		char type_edit = Input<char>("e)dit_single_file, g)enerate_single_file, d)efault_all_files, r)eload, q)uit");
		
		switch(type_edit)
		{
			case 'e':
			{
				PrintListOfFiles(in_file_names);
				size_t file_number_to_edit = Input<int>("the number of the file to edit") - 1;
				if(file_number_to_edit < 0 || file_number_to_edit >= in_file_names.size())
				{
					Print("Error, index number not valid");
					break;
				}
				auto name_of_file_to_edit = in_file_names[file_number_to_edit];
				auto status_of_file = status_of_in_files[name_of_file_to_edit];
				
				if(status_of_file == file_status_bad_corrupted || status_of_file == file_status_bad_nonexistent)
				{
					Print("Error, this file cannot be edited as it is bad. You need to generate first");
					break;
				}
				if(file_number_to_edit == 0)
				{
					Print("This file can only be manually edited or generated");
					break;
				}
				
				
				// implement the edit of the file
				break;
			}
			case 'g':
			{
				PrintListOfFiles(in_file_names);
				size_t file_number_to_generate = Input<int>("the number of the file to generate") - 1;
				if(file_number_to_generate  < 0 || file_number_to_generate  >= in_file_names.size())
				{
					Print("Error, index number not valid");
					break;
				}
				
				auto name_of_file_to_generate = in_file_names[file_number_to_generate];
				
				if(file_number_to_generate == 0)
				{
					GenerateOtherParametersDefault(name_of_file_to_generate);
				}
				
				
				Print("Editing " + name_of_file_to_generate);
				double multiplier = Input<double>("multiplier");
				double value = Input<double>("value: ");
				
				file_images[name_of_file_to_generate] = File(FileProperty(DefaultValue(multiplier, value), AssignProperty(name_of_file_to_generate, file_type_and_group_diamensions)));
				file_images[name_of_file_to_generate].write(name_of_file_to_generate);
				break;
			}
			case 'd':
			{
				SetDefaultAll(file_name_and_property);
				break;
			}	
			case 'q':
			{
				stay_in_edit_menu = false;
				break;
			}
			case 'r':
			{
				break;
			}	
			default:
			{
				Print("\n!!Error - input not recognized.");
				break;
			}
		}
	}
			
	return 0;
}




void Print(const std::string& text_to_print, int tab_size)
{
	std::cout << std::string(tab_size, '\t') << text_to_print << '\n';
}

Diamension ReadDiamension()
{
	int n_row = Input<int>("number of rows 'n_row'");
	int m_col= Input<int>("number of columns 'm_col'");
	
	return Diamension(n_row, m_col);
}

std::vector<double> ReadFile(std::ifstream& fin)
{
	std::vector<double> file_vector;
	double value;
	while(fin >> value)
	{
		file_vector.push_back(value);
	}
	
	return file_vector;
}


std::string StatusOfInFile(const std::string& file_name, const FileProperty& file_property, File& file)
{
	std::ifstream fin(file_name + ".txt");
	if(!fin.is_open())
	{
		return file_status_bad_nonexistent;
	}
	std::vector<double> file_vector = ReadFile(fin);
	
	if(file_property.size() != file_vector.size())
	{
		return file_status_bad_corrupted;
	}
	
	File default_file(file_property);
	if(default_file == file_vector)
	{
		file = default_file;
		return file_status_okay_default;
	}
	
	if(file == file_vector)
	{
		return file_status_okay_edited;
	}
	
	file.update(file_vector);
	
	return file_status_okay_custom;
}

std::map<std::string, std::string> StatusOfInFiles(const std::map<std::string, FileProperty>& file_name_and_property, std::map<std::string, File>& file_images)
{
	std::map<std::string, std::string> status_of_files;
	for(const auto& [file_name, property]: file_name_and_property)
	{
		status_of_files[file_name] = StatusOfInFile(file_name,  property, file_images[file_name]);
	}
	
	return status_of_files;
}
	
void PrintStatusOfInFiles(const std::map<std::string, std::string>& status_of_files)
{
	Print("Status of files:");
	for(const auto& [file_name, status]: status_of_files)
	{
		Print("[" + status + "], " + file_name + ".txt", 1);
	}
}
	
		


std::vector<std::pair<std::string, double>> OtherParametersDefaultValues(const Diamension& diamension)
{
	return std::vector<std::pair<std::string, double>> 
	{
		{"n_row", double(diamension.n_row)},
		{"m_col", double(diamension.m_col)},
		{"length_tube", 0.1},
		{"fluid_type_input", 1},
		{"fluid_type_output", 2},
		{"viscosity_fluid_1", 1e-3},
		{"viscosity_fluid_2", 1e-3}
	};
}

std::map<std::string, GroupDiamension> FileTypeAndGroupDiamension(const Diamension& diamension)
{
	return std::map<std::string, GroupDiamension>
	{
		{"other_parameters", GroupDiamension()},
		{"pressure", {{diamension.n_row, 1}, {0, 0}}},
		{"distribution", {{diamension.n_row - 1, diamension.m_col - 2}, {diamension.n_row, diamension.m_col - 1}}}
	};
}

std::map<std::string, DefaultValue> FileTypeAndDefaultValues()
{
	return std::map<std::string, DefaultValue>
	{
		{"other_parameters", DefaultValue()},
		{"pressure_input", {1e3, 100}},
		{"pressure_output", {1e3, 1}},
		{"radius", {0.001, 1}},
		{"fill", {1, 1}},
		{"sigma", {0.001, 73}}
	};
}


std::map<std::string, FileProperty> FileNameAndProperty(const std::vector<std::string>& file_names, const std::map<std::string, GroupDiamension>& file_type_and_group_diamension,  const std::map<std::string, DefaultValue>& file_type_and_default_value)
{
	std::map<std::string, FileProperty> file_name_and_property;
	for(const auto& file_name: file_names)
	{
		file_name_and_property[file_name] = FileProperty(AssignProperty(file_name, file_type_and_default_value), AssignProperty(file_name, file_type_and_group_diamension));
	}
	
	return file_name_and_property;
}

File FileDefault(const std::string& file_name, const std::map<std::string, FileProperty>& file_name_and_property)
{
	if(file_name.find("other") != std::string::npos)
	{
		return File();
	}
	
	return File(file_name_and_property.at(file_name));
}

std::map<std::string, File> FileImagesDefault(const std::map<std::string, FileProperty>& file_name_and_property)
{
	std::map<std::string, File> file_images_default;
	for(const auto& [file_name, property]: file_name_and_property)
	{
		file_images_default[file_name] = FileDefault(file_name, file_name_and_property);
	}
	
	return file_images_default;
}

void PrintListOfFiles(const std::vector<std::string>& in_file_names)
{
	int count = 1;
	for(const auto& file_name: in_file_names)
	{
		Print(std::to_string(count) + ")" + file_name + ".txt", 1);
		++ count;
	}
}
		 
		
