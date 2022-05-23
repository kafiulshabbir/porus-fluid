#include "dat_boundary.h"

namespace dat
{
	cls_boundary::cls_boundary(const std::string& name_file): name_file(name_file) {}
	

	int cls_boundary::read_int(std::ifstream& fin)
	{
		std::string s;
		int ipt;
		
		fin >> s >> ipt;
		
		return ipt;
	}
	
	double cls_boundary::read_double(std::ifstream& fin)
	{
		std::string s;
		double ipt;
		
		fin >> s >> ipt;
		return ipt;
	}
	
	bool cls_boundary::read_file()
	{
		std::ifstream fin(name_file);
		
		if(!fin.is_open())
		{
			std::cout << "EROOR: READ FILE " << name_file << '\n';
			return true;
		}
		
		nxn_grid_size = read_int(fin);
		fluid_type_input = read_int(fin);
		fluid_type_output = read_int(fin);

		length_tube = read_double(fin);
		multiplier_for_radius = read_double(fin);
		leak_radius = read_double(fin);

		pressure_input = read_double(fin);
		pressure_output = read_double(fin);
		viscosity_fluid_1 = read_double(fin);
		viscosity_fluid_2 = read_double(fin);

		density_fluid_1 = read_double(fin);
		density_fluid_2 = read_double(fin);
		sigma_in_tube = read_double(fin);
		
		std::cout << "Feedback: " << name_file << " successfully read!";
		return false;
	}
}
