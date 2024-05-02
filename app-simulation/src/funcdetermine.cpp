#include "func/funcdetermine.h"

Tdouble func::Determine::determine_volume
(
	const Tdouble& radius,
	const Tdouble& velocity,
	const double time_step,
	const dst::Diamension& diamension
)
{
	Tdouble volume = diamension.empty_table();
	
	for(int row = 0; row < diamension.rows; ++ row)
	{
		for(int col = 0; col < diamension.cols; ++ col)
		{
			const double vel_abs = std::abs(velocity[row][col]);
			const double area_tube = std::pow(radius[row][col], 2) * declconst::PI;
			volume[row][col] = vel_abs * area_tube * time_step;
		}
	}
	
	return volume;
}

std::vector<std::vector<int>> func::Determine::gen_add_mnsc(const TMns& mnsc, const dst::Diamension& diamension)
{
	std::vector<std::vector<int>> add_mnsc = diamension.empty_table_templated<int>();
	
	for(int nrow = 0; nrow < diamension.node_rows(); ++ nrow)
	{
		for(int ncol = 0; ncol < diamension.node_cols(nrow); ++ ncol)
		{
			const std::vector<dst::Tube> cons
				= diamension.generate_tubes_connected_to_node(nrow, ncol);
			
			const int directions = cons.size();
			
			std::vector<bool> need_to_add(directions, false);
			
			bool blue_fluid_present = false;
			
			for(int direction = 0; direction < directions; ++ direction)
			{
				const dst::Tube& con = cons[direction];
				if(con.active)
				{
					const int end_type_fluid = mnsc[con.row][con.col].type_near_node(direction);
					if(end_type_fluid) // grey fluid
					{
						need_to_add[direction] = true;
					}
					else
					{
						blue_fluid_present = true;
					}
				}
			}
			
			if(blue_fluid_present == false)
			{
				continue;
			}
			
			for(int direction = 0; direction < directions; ++ direction)
			{
				const dst::Tube& con = cons[direction];
				if(con.active && need_to_add[direction])
				{
					add_mnsc[con.row][con.col] += cap_press_contb_from_direc(direction);
				}
			}
			
		}
	}
	
	return add_mnsc;
}

				
int func::Determine::cap_press_contb_from_direc(const int direction)
{
	if(direction > 1)
	{
		return -1;
	}
	
	return 1;
}
