#include "func/funcintegration.h"

func::IntegrationResult func::Integration::integrate
(
	const Tfloat& radius,
	const TMns& mnsc,
	const Tfloat& velocity,
	const Tfloat& volume,
	const dst::Diamension& diamension,
	const int row_minimum_time,
	const int col_minimum_time
)
{
	const FluidAdditionResult fluid_addition_result =
		calculate_fluid_addition_result(radius,
		mnsc, velocity, volume, diamension);
		
	const TMns new_mnsc = combine_fluid_additions
	(
		radius,
		mnsc,
		velocity,
		diamension,
		fluid_addition_result.fluid_addition_table,
		row_minimum_time,
		col_minimum_time
	);
	
	func::IntegrationResult integration_result
	{
		new_mnsc,
		fluid_addition_result.fluid_displacements
	};
	
	return integration_result;
}

func::Integration::FluidAdditionResult func::Integration::calculate_fluid_addition_result
(
	const Tfloat& radius,
	const TMns& mnsc,
	const Tfloat& velocity,
	const Tfloat& volume,
	const dst::Diamension& diamension
)
{
	TFluidAdditions fluid_addition_table = diamension.empty_table_templated<FluidAdditions>();
	FluidDisplacements fluid_displacements;
	
	for(int row = 0; row < diamension.node_rows(); ++ row)
	{
		for(int col = 0; col < diamension.node_cols(row); ++ col)
		{
			FluidAdditions tank; //vol fluid from tube into node
			std::vector<Tube_FromNode> tubes_from_node_vec;
			
			const std::vector<dst::Tube> tubes_connected_vec =
				diamension.generate_tubes_connected_to_node(row, col);
				
			const int total_directions = tubes_connected_vec.size();
			for(int direction = 0; direction < total_directions; ++ direction)
			{
				const dst::Tube& connection = tubes_connected_vec[direction];
				if(connection.active)
				{
					const float rad = radius[connection.row][connection.col];
					const dst::Mns& mns = mnsc[connection.row][connection.col];
					const float vel = velocity[connection.row][connection.col];
					const float vol = volume[connection.row][connection.col];
					
					
					if(mns.is_the_flow_from_tube_into_node(direction, vel))
					{
						const int type_fluid = mns.type_fluid_into_node(direction);
						tank.fluid[type_fluid] += vol;
						continue;
					}

					Tube_FromNode tube
					{
						rad,
						connection.row,
						connection.col
					};
		
					tubes_from_node_vec.push_back(tube);
				}
			}
			
			//for(const auto& tpshin: tubes_from_node_vec)	std::cout << "tube_push_out before short: radius=" << tpshin.radius << ", r=" << tpshin.r << ", c=" << tpshin.c << std::endl;
			
			//std::cout << "second stage reached!" << std::endl;
			if(row == 0)
			{
				fluid_displacements.out.front() += tank.fluid.front();
				fluid_displacements.out.back() += tank.fluid.back();
				continue;
			}
			if(row == diamension.rows) // NOTE might remove else
			{ 
				for(const Tube_FromNode& tube: tubes_from_node_vec)
				{
					const float vol = volume[tube.row][tube.col];
					fluid_addition_table[tube.row][tube.col].fluid[0] = vol;
					fluid_displacements.in[0] += vol;
				}
				continue;
			}
			
			std::sort(tubes_from_node_vec.begin(),
				tubes_from_node_vec.end(),
				compare_where_wetting_fluid_go_first);
			
			for(const auto& tpshin: tubes_from_node_vec)
			{
				//std::cout << "tube_push_out after sort: radius=" << tpshin.radius << ", r=" << tpshin.r << ", c=" << tpshin.c << std::endl;
				
				fluid_addition_table[tpshin.row][tpshin.col] = add_fluid_from_tank(volume[tpshin.row][tpshin.col], tank);
			}
		}
	}
	
	FluidAdditionResult fluid_addition_result
	{
		fluid_addition_table,
		fluid_displacements
	};
	
	return fluid_addition_result;
}

TMns func::Integration::trimmer(TMns mnsc, const Tfloat& velocity)
{
	return mnsc;
}

TMns func::Integration::combine_fluid_additions
(
	const Tfloat& radius,
	TMns mnsc,
	const Tfloat& velocity,
	const dst::Diamension& diamension,
	const TFluidAdditions& fluid_addition_table,
	const int row_minimum_time,
	const int col_minimum_time
)
{
	for(int row = 0; row < diamension.rows; ++ row)
	{
		for(int col = 0; col < diamension.cols; ++ col)
		{
			const float vel = velocity[row][col];
			const float rad = radius[row][col];
			const std::vector<float>& add_fluid_from_tank_to_tube = fluid_addition_table[row][col].fluid;
			const bool tube_with_minimum_time =
				(row == row_minimum_time) &&
				(col == col_minimum_time);
			
			std::cout << std::endl << "(" << row << ", " << col << "): ";
			
			mnsc[row][col].update(vel, rad, add_fluid_from_tank_to_tube, tube_with_minimum_time);
		}
	}
	
	return mnsc;
}

bool func::Integration::compare_where_wetting_fluid_go_first(const Tube_FromNode& first, const Tube_FromNode& second)
{
	return first.rad < second.rad;
}

func::Integration::FluidAdditions func::Integration::add_fluid_from_tank(const float vol, func::Integration::FluidAdditions& tank)
{
	FluidAdditions fluid_addtion_to_tube;
	const float vol_first_fluid = std::min(tank.fluid.front(), vol);
	// min_of(fluid_1_left_in_tank, amount_to_be_actually_pushed_in);
	
	fluid_addtion_to_tube.fluid.front() = vol_first_fluid;
	fluid_addtion_to_tube.fluid.back() = vol - vol_first_fluid;
	
	tank.remove_fluid(fluid_addtion_to_tube);
	
	return fluid_addtion_to_tube;
}

func::Integration::FluidAdditions::FluidAdditions(): fluid(2) {}

func::FluidDisplacements::FluidDisplacements(): in(2), out(2) {}

void func::Integration::FluidAdditions::remove_fluid(const func::Integration::FluidAdditions& add_fluid_from_tank)
{
	for(size_t i = 0; i < fluid.size(); ++ i)
	{
		fluid[i] -= add_fluid_from_tank.fluid[i];
	}
}
