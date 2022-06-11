#ifndef DEF_DAT_BOUNDARY
#define DEF_DAT_BOUNDARY

#include "dec.h"

namespace dat
{
	class cls_boundary
	{
		std::string name_file;
		int read_int(std::ifstream& fin);
		double read_double(std::ifstream& fin);
		bool read_file();
		
	public:
		int nxn_grid_size;
		int fluid_type_input;
		int fluid_type_output;

		double length_tube;
		double multiplier_for_radius;
		double leak_radius;

		double pressure_input;
		double pressure_output;
		double viscosity_fluid_1;
		double viscosity_fluid_2;

		double density_fluid_1;
		double density_fluid_2;
		double sigma_input;
		double sigma_output;
		
		cls_boundary() = delete;
		cls_boundary(const std::string& name_file);
	};
}
#endif
