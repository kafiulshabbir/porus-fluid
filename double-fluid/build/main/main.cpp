#include "dec.h"

#include "dat_boundary.h"
#include "dat_radius.h"
#include "dat_fill.h"

#include "prs.h"
// g++ -Wall -std=c++17 main2.cpp stc_node.cpp dat_tube.cpp dat_fill.cpp dat_radius.cpp dat_boundary.cpp prs -o main2

// There are 6 namespaces:

// 1- dat:: DATa classes: dat::radius, dat::boundary, dat::fill: (radius-distribution), (boundary-conditions), (fill-distribution)
// 2- dec:: DEClarations of constants 
// 3- drw:: DRaW library: drw::colour and drw::bmp
// 4- prs:: PRocesS data, calculation from well defined inputs
// 5- std:: STanDard c++ library
// 6- stc:: STruCtures which help in the project stc::node

matrix PressureDistributionMatrix(const vector& v, double left, double right, int n)
{
	matrix M;
	M.reserve(n);
	
	for(int i = 0; i < n; ++ i)
	{
		vector x;
		x.reserve(n + 2);
		x.push_back(left);
		for(int j = 0; j < n; ++ j)
		{
			x.push_back(v[i * n + j]);
		}
		x.push_back(right);
		
		M.push_back(x);
	}
	
	return M;
}

void Print(const matrix& v, const std::string& file_name)
{
	std::ofstream fout(file_name);
	
	for(const auto& row: v)
	{
		for(const auto& x: row)
		{
			fout << x << ' ';
		}
		fout << '\n';
	}
}

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

int main()
{
	// STEP-1: READ THE THREE INPUT FILES
	dat::cls_boundary boundary(dec::FILE_IN_BOUNDARY_CONDITIONS);
	if(boundary.read_file()) return 1;
	
	const int n = boundary.nxn_grid_size;
	
	dat::cls_radius radius(dec::FILE_IN_RADIUS_DISTRIBUTION, n, boundary.multiplier_for_radius, boundary.leak_radius, boundary.leak_radius);
	if(radius.read_file()) return 1;
	
	dat::cls_fill fill(dec::FILE_IN_FILL_DISTRIBUTION, n, 1, fluid_viscosity_default(boundary.fluid_type_input), fluid_viscosity_default(boundary.fluid_type_output));
	if(fill.read_file()) return 1;
	
	/*
	radius.print_data();
	fill.print_data();
	*/
	
	// STEP-2: GENERATE EQUATIONS RELATING THE PRESSURES AT EACH POINT
	matrix gauss_matrix_for_pressure = prs::GaussMatrixForPressure(boundary, fill, radius);
	//Print(gauss_matrix_for_pressure, "out-matrix-elem.txt");
	
	// STEP-3: CALCULATE PRESSURES
	vector calculated_pressures = prs::GaussElimination(gauss_matrix_for_pressure);
	matrix pressure_distribution = PressureDistributionMatrix(calculated_pressures, boundary.pressure_input, boundary.pressure_output, n);
	
	Print(pressure_distribution, dec::FILE_OUT_PRESSURES);
	
	// STEP-4: CALCULATE VELOCITIES
	flow_matrices flow = CalculateFlowMatrices(pressure_distribution, radius, fill, boundary.length_tube, boundary.viscosity_fluid_1, boundary.viscosity_fluid_2, boundary.sigma_in_tube, n);
	
	Print(flow.hor, dec::FILE_OUT_FLOW_HOR);
	Print(flow.ver, dec::FILE_OUT_FLOW_VER);
	
	// STEP-5: PLOT CUSTOM [COLOURED] PRESSURE DISTRIBUTION, [COLOURED] WITH TUBE VELOCITY, [ARROWED] TUBE VELOVITY
	
	// STEP-6: PLOT THICKNESS OF TUBE AND THEIR FILL DISTRIBUTION
	return 0;
}
