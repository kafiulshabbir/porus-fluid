#ifndef DEF_PRS
#define DEF_PRS

#include "dec.h"


#include "dat_boundary.h"
#include "dat_radius.h"
#include "dat_fill.h"
#include "stc_node.h"

namespace prs
{
	//Generate a equation of size n * (n + 1)
	double sign_jump(double proportion, int direction);
	vector GaussElimination(matrix v);
	matrix GenerateGaussMatrixForUnknownPressure(const dat::cls_boundary&, const dat::cls_fill&, const dat::cls_radius&);
	matrix GeneratePressureDistributionFromVector(const vector& v, double left, double right, int n);
	


double fluid_viscosity_default(double fluid_type)
{
	if(fluid_type == 1)
	{
		return 1;
	}
	
	return -1;
}

struct flow_matrices
{
	matrix ver;
	matrix hor;

};

flow_matrices CalculateFlowMatrices(const matrix& pressure, const dat::cls_radius& radius, const dat::cls_fill& fill, const double l, const double u1, const double u2, const double sigma, const int n)
{
	matrix ver;
	ver.reserve(n - 1);
	for(int row = 0; row + 1 < n; ++ row)
	{
		vector r;
		r.reserve(n);
		for(int col = 0; col < n; ++ col)
		{
			stc::node node_i(row, col);
			stc::node node_j = node_i.down();
			
			double radius_tube = radius(node_i, node_j);
			double fill_proportion = fill(node_i, node_j);
			
			double l1 = std::abs(fill_proportion) * l;
			double l2 = l - l1;
			if(fill_proportion < 0)
			{
				std::swap(l1, l2);
			}
			
			const double K = dec::PI / 8.0 / (u1 * l1 + u2 * l2);
			const double pressure_diff = pressure[row][col + 1] - pressure[row + 1][col + 1];
			double flow = K * (   std::pow(radius_tube, 4) * pressure_diff  +   prs::sign_jump(fill_proportion, 2) * 2 * sigma * std::pow(radius_tube, 3)   );
			
			r.push_back(flow);
		}
		
		ver.push_back(r);
	}
	
	matrix hor;
	hor.reserve(n);
	for(int row = 0; row < n; ++ row)
	{
		vector r;
		r.reserve(n + 2);
		for(int col = 0; col < n + 1; ++ col)
		{
			int direction = 1;
			double pressure_diff = pressure[row][col] - pressure[row][col + 1];
			stc::node node_i(row, col - 1);
			if(col == 0)
			{
				direction = 3;
				pressure_diff *= -1;
				node_i = stc::node(row, 0);
			}
			
			
			stc::node node_j = node_i.rel(direction);
			
			double radius_tube = radius(node_i, node_j);
			double fill_proportion = fill(node_i, node_j);
			
			double l1 = std::abs(fill_proportion) * l;
			double l2 = l - l1;
			if(fill_proportion < 0)
			{
				std::swap(l1, l2);
			}
			
			const double K = dec::PI / 8.0 / (u1 * l1 + u2 * l2);
		
			double flow = K * (   std::pow(radius_tube, 4) * pressure_diff   +   prs::sign_jump(fill_proportion, direction) * 2 * sigma * std::pow(radius_tube, 3)   );
			if(col == 0)
			{
				flow *= -1;
			}
			
			r.push_back(flow);
		}
		
		hor.push_back(r);
	}
	
	
	
	flow_matrices out;
	out.ver = ver;
	out.hor = hor;
	return out;
}

}

#endif
