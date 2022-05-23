#ifndef DEF_TUBE
#define DEF_TUBE

#include "dec.h"
#include "stc_node.h"
 
namespace dat
{
	class cls_tube
	{
		std::string name_file;
		int n;
		double multiplier;
		double left_default;
		double right_default;
		
		matrix ver;
		matrix hor;
		
		matrix read_matrix(int R, int C, double multiplier, std::ifstream& fin);
		void print_matrix(const matrix& M) const;
		
	public:
		cls_tube() = delete;
		cls_tube(const std::string& name_file, int n, double multiplier, double left_default, double right_default);
		double operator () (const stc::node& first, const stc::node& second) const;
		bool read_file();
		void print_data() const;
	};


}
#endif
