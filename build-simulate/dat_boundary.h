#ifndef DEF_DAT_BOUNDARY
#define DEF_DAT_BOUNDARY

#include "dec.h"

namespace dat
{
	class cls_boundary
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

			"length_tube",
			"sigma"
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
		double sigma;
		
		cls_boundary() = delete;
		cls_boundary(const std::string& name_file);
		
		bool read_file();
		
	};
}
#endif
