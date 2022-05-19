#ifndef DEF_TYPE_RADIUS
#define DEF_TYPE_RADIUS

#include "project_includes.h"
namespace frw
{
	struct point
	{
		int row;
		int col;
		
		point(int, int);
		point up() const;
		point down() const;
		point left() const;
		point right() const;
		point rel(int u) const;
		int linear(int n) const;
		bool vertical(const point& other) const;
		bool horizontal(const point& other) const;
	};

	class type_radius
	{
		std::string name_file;
		int n;
		double leak_radius;
		matrix ver;
		matrix hor;
		
		
		double fourth(double x) const;
		void error(const std::string& message) const;
		void multiply(matrix& w, double m);
		bool outside_matrix(int m) const;
		bool outside_matrix(const point& node) const;
		int good_begin(int a, int b) const;
		double extract_radius(const point&, const point&) const;
		
	public:
		type_radius() = delete;
		type_radius(const std::string&);
		bool read_file();
		
		double operator () (const point&, const point&) const;
		int N() const;
	};
}

#endif
