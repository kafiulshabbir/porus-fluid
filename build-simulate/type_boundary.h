#ifndef DEF_TYPE_BOUNDARY
#define DEF_TYPE_BOUNDARY

#include "project_includes.h"
namespace frw
{
	class type_boundary
	{
		std::string name_file;
		
		const std::vector<std::string> contents
		{
			"fluid_type_input",
			"fluid_type_output",

			"pressure_input",
			"pressure_output",

			"viscosity_fluid_0",
			"viscosity_fluid_1",

			"density_fluid_0",
			"density_fluid_1",

			"length_tube"
		};
		
		void create_file() const;
		
		void read_file_error() const;
		
		bool read_file_try();
		
	public:
		int fluid_type_input;
		int fluid_type_output;

		double pressure_input;
		double pressure_output;

		double viscosity_fluid_0;
		double viscosity_fluid_1;

		double density_fluid_0;
		double density_fluid_1;

		double length_tube;
		
		type_boundary() = delete;
		type_boundary(const std::string& name_file): name_file(name_file) {}
		
		bool read_file();
		
	};
}
#endif
