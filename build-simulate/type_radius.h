#ifndef DEF_TYPE_RADIUS
#define DEF_TYPE_RADIUS

#include "project_includes.h"
#include "type_node.h"
namespace typ
{

	class type_radius
	{
		std::string name_file;
		int n;
		double leak_radius;
		matrix ver;
		matrix hor;
		
		void error(const std::string& message) const;
		void multiply(matrix& w, double m);
		bool outside_matrix(int m) const;
		bool outside_matrix(const node& node) const;
		int good_begin(int a, int b) const;
		double extract_radius(const node&, const node&) const;
		
	public:
		type_radius() = delete;
		type_radius(const std::string&);
		bool read_file();
		
		double operator () (const node&, const node&) const;
		double operator () (const node&, const node&, int) const;
		int N() const;
	};
}

#endif
