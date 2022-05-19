#include "type_boundary.h"

namespace frw
{
	void type_boundary::create_file() const
	{	
		std::ofstream fout(name_file);
		for(int i = 0; i < contents.size(); ++ i)
		{
			fout << contents[i] << '\n';
			
			if(i % 2)
			{
				fout << '\n';
			}
		}
	}
		
	void type_boundary::read_file_error() const
	{
		std::cout << "Fill Data correctly in " << name_file << "!\n";
		create_file();	
	}
		
	bool type_boundary::read_file_try()
	{
		std::ifstream fin(name_file);
		if(!fin.is_open())
		{
			return true;
		}
		
		vec v;
		for(const auto& s: contents)
		{
			std::string c;
			double d;
			fin >> c >> d;
			if(s != c)
			{
				return true;
			}
			
			v.push_back(d);
		}
		
		fluid_type_input = v[0];
		fluid_type_output = v[1];

		pressure_input = v[2];
		pressure_output = v[3];

		viscosity_fluid_0 = v[4];
		viscosity_fluid_1 = v[5];

		density_fluid_0 = v[6];
		density_fluid_1 = v[7];

		length_tube = v[8];
		
		return false;
	}

		
	bool type_boundary::read_file()
	{
		if(read_file_try())
		{
			read_file_error();
			return true;
		}
		
		std::cout << name_file << " read correctly!\n";
		return false;
	}
}
