#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string>
#include <stdexcept>
const std::string in_file_names_txt = "in_file_names.txt";

struct Diamension
{
	int n_row;
	int m_col;
};

struct GroupDiamension
{
	Diamension ver;
	Diamension hor;
};

struct DefaultValue
{
	double multiplier;
	double value;
};

struct File
{
	double multiplier;
	std::vector<std::vector<double>> matrix1;
	std::vector<std::vector<double>> matrix2;
};

// determine the default <multiplier, value> of different file types
const std::vector<std::pair<std::string, DefaultValue>> type_to_default_values
{
	{"pressure_input", {1e3, 100}},
	{"pressure_output", {1e3, 1}},
	{"radius", {0.001, 1}},
	{"fill", {1, 1}},
	{"sigma", {0.001, 73}}
};

const std::vector<std::pair<std::string, double>> OtherParametersDefaultValues(const Diamension& diamension)
{
	return std::vector<std::pair<std::string, double>>
	{
		{"n_row", diamension.n_row},
		{"m_col", diamension.m_col},
		{"length_tube", 0.1},
		{"fluid_type_input", 1},
		{"fluid_type_output", 2},
		{"viscosity_fluid_1", 1e-3},
		{"viscosity_fluid_2", 1e-3}
	};
}



// DATA INPUT OUTPUT
	// input of data
	///  From Console
	template <typename T>
	T Input(const std::string& hint_text_for_input);
	Diamension ReadDiamension();

	///  From File
	std::vector<std::string> ReadFileNames();

	// output of data
	///  To console
	void PrintOpeningMessage();
	void Print(const std::string& text_to_print);

	///  To file
	void Print(const std::string& file_name, const GroupDiamension& group_diamension, const DefaultValue& default_value);
	void PrintMatrix(std::ofstream& fout, const Diamension& diamension, double value);

//TOOLS
	// string
	bool Contains(const std::string& string_contains, const std::string& substr);

	// linkers
	/// file_name -> diamensions
	GroupDiamension GroupDiamensionFromFileName(const std::string& file_name, const Diamension& diamension);

	/// file_name -> default_values
	DefaultValue FindDefaultForInputFileType(const std::string& file_name);

//MAIN
	void SetDefaultAll(const std::vector<std::string>& in_file_names, const Diamension& diamension);
	void GenerateParameters(const std::string& file_name, const Diamension& diamension);
	void GiveStatusOfInFiles(const std::string& in_file_names, const Diamension& diamension);
int main()
{
	PrintOpeningMessage();
	const Diamension diamension = ReadDiamension();
	const std::vector<std::string> in_file_names = ReadFileNames();
	
	bool stay_in_edit_menu = true;
	while(stay_in_edit_menu)
	{
		char type_edit = Input<char>("e)dit_single_file, g)enerate_single_file, d)efault_all, q)uit");
		GiveStatusOfInFiles(in_file_names, diamension);
		
		switch(type_edit)
		{
			case 'e':
			PrintListOfFiles();
			Input<int>("which file to edit?");
			// if status is good - allow edit
			// if status is bad, ask to be generated
			break;
			
			case 'g':
			PrintListOfFiles();
			Input<int>("which file to generate");
			// generate the file
			break;
			
			case 'd':
			SetDefaultAll(in_file_names, diamension);
			break;
			
			case 'q':
			stay_in_edit_menu = false;
			break;
				
			default:
			Print("\n!!Error - input not recognized.");
			break;
		}
	}
			
	return 0;
}

void PrintOpeningMessage()
{
	Print("Running in_file_generator");
}
	
void Print(const std::string& text_to_print)
{
	std::cout << text_to_print << '\n';
}

Diamension ReadDiamension()
{
	Diamension read_diamension;
	read_diamension.n_row = Input<int>("number of rows 'n_row'");
	read_diamension.m_col = Input<int>("number of columns 'm_col'");
	
	return read_diamension;
}



std::vector<std::string> ReadFileNames()
{
	std::ifstream fin(in_file_names_txt);
	if(!fin.is_open())
	{
		throw std::runtime_error(in_file_names_txt + " could not be opened!");
	}
	std::vector<std::string> in_file_names;
	
	std::string buffer_file_name_line;
	while(getline(fin, buffer_file_name_line))
	{
		in_file_names.push_back(buffer_file_name_line);
	}
	
	return in_file_names;
}

// [pressure] = multiplier + n x 1;
// [rest] = multiplier + (n - 1) x (m - 2) + n x (m - 1)
GroupDiamension GroupDiamensionFromFileName(const std::string& file_name, const Diamension& diamension)
{
	GroupDiamension group_diamension;
	group_diamension.ver.n_row = diamension.n_row - 1;
	group_diamension.ver.m_col = diamension.m_col - 2;
	group_diamension.hor.n_row = diamension.n_row;
	group_diamension.hor.m_col = diamension.m_col - 1;
	
	if(Contains(file_name, "pressure"))
	{
		group_diamension.ver.n_row = diamension.n_row;
		group_diamension.ver.m_col = 1;
		group_diamension.hor.n_row = diamension.n_row;
		group_diamension.hor.m_col = 1;
	}
	
	return group_diamension;
}

void SetDefaultAll(const std::vector<std::string>& in_file_names, const Diamension& diamension)
{
	for(const std::string& file_name: in_file_names)
	{
		if(Contains(file_name, "parameters"))
		{
			GenerateParameters(file_name, diamension);
		}
		else
		{
			DefaultValue default_value = FindDefaultForInputFileType(file_name);
			GroupDiamension group_diamension = GroupDiamensionFromFileName(file_name, diamension);
			
			Print(file_name, group_diamension, default_value);
		}
	}
}

bool Contains(const std::string& string_contains, const std::string& substr)
{
	return string_contains.find(substr) != std::string::npos;
}

void GenerateParameters(const std::string& file_name, const Diamension& diamension)
{
	std::ofstream fout(file_name);
	std::vector<std::pair<std::string, double>> default_values = OtherParametersDefaultValues(diamension);
	for(const auto& default_value: default_values)
	{
		fout << default_value.first << "= " << default_value.second << '\n';
	}
}

void PrintMatrix(std::ofstream& fout, const Diamension& diamension, double value)
{
	fout << '\n';
	for(int i = 0; i < diamension.n_row; ++ i)
	{
		for(int j = 0; j < diamension.m_col; ++ j)
		{
			fout << value << ' ';
		}
		
		fout << '\n';
	}
}
	
void Print(const std::string& file_name, const GroupDiamension& group_diamension, const DefaultValue& default_value)
{
	std::ofstream fout(file_name);
	fout << default_value.multiplier << '\n';
	PrintMatrix(fout, group_diamension.ver, default_value.value);
	PrintMatrix(fout, group_diamension.hor, default_value.value);
}

DefaultValue FindDefaultForInputFileType(const std::string& file_name)
{
	for(const auto& type_to_default_value: type_to_default_values)
	{
		if(Contains(file_name, type_to_default_value.first))
		{
			return type_to_default_value.second;
		}
	}
	
	return DefaultValue{-1, -1};
}

<existensial, content> CheckOtherParameters()
{
	// if read fails, decide corrupted
	// if read titles dont match decide corrupted
	// if file does not exist deep non existent
	
	// if all file values matched the default, decide default
	// if they are different, and matches with image - decide edidted
	// or else decide that they are custom and change the image
}
 
void GiveStatusOfInFiles(const std::string& in_file_names, const Diamension& diamension)
{
	const std::vector<std::string> existensial{"READABLE", "CORRUPTED", "NONEXISTENT"};
	const std::vector<std::string> content{"DEFAULT", "CUSTOM"};
	
	for(const std::string& file_name: in_file_names)
	{
		if(Contains(file_name, "other_parameters"))
		{
			CheckOtherParameters() // -> 
			
		}
		else
		{
			
			
			
		}
	}
}
