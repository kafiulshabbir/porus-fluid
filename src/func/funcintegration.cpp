#include "func/funcintegration.h"

func::IntegrationResult func::Integration::integrate
(
	const Tfloat& radius,
	const TMns& mnsc,
	const Tfloat& velocity,
	const Tfloat& volume,
	const dst::Diamension& diamension
)
{
	
	const FluidAdditionResult fluid_addition_result = calculate_fluid_addition(diamension);
	TMns new_mnsc = combine_additions(radius, mnsc, velocity, fluid_addition_result.fluid_addition_to_tube_table);
	
	func::IntegrationResult integration_result
	{
		new_mnsc,
		fluid_addition_result.fluid_displacements
	};
	
	return integration_result;
}

FluidAdditionResult func::Integration::calculate_fluid_addition(const dst::Diamension& diamension)
{
	TFluidAdditionToTube add_fluid_table(diamension.rows, std::vector<FluidAdditionToTube>(diamension.cols));
	FluidDisplacements fluid_displacements;
	
	for(int row = 0; row <= diamension.rows; ++ row)
	{
		const int nodes_in_this_row = diamension.number_nodes_in_this_row(row);
		for(int col = 0; col <= nodes_in_this_row; ++ col)
		{
			FluidAdditionToTube vol_from_tube_into_node;
			std::vector<func::TubeFlowIntoFromNode> tubes_flow_into_from_node;
			
			const std::vector<dst::Tube> connections_vec =
				diamension.generate_tubes_connected_to_node(row, col);
				
			for(int direction = 0; direction < connections_vec.size(); ++ direction)
			{
				const dst::Tube& connection = connections_vec[direction];
				if(connection.active)
				{
					const float rad = radius[connection.row][connection.col];
					const dst::Mns& mns = mnsc[connection.row][connection.col];
					const float vel = velocity[connection.row][connection.col];
					const float vol = volume[connection.row][connection.col];
					
					
					if(mns.is_flow_into_node(direction, vel))
					{
						const int type_fluid = mns.type_fluid_into_node(direction);
						vol_from_tube_into_node.fluid[type_fluid] += vol;
						continue;
					}

					TubeFlowIntoFromNode tube
					{
						rad,
						connection.row,
						connection.col
					};
		
					tubes_flow_into_from_node.push_back(tube);
				}
			}
			
			//for(const auto& tpshin: tubes_flow_into_from_node)	std::cout << "tube_push_out before short: radius=" << tpshin.radius << ", r=" << tpshin.r << ", c=" << tpshin.c << std::endl;
			
			//std::cout << "second stage reached!" << std::endl;
			if(row == 0)
			{
				fluid_displacements.out.front() += vol_in.front();
				fluid_displacements.out.back() += vol_in.back();
				continue;
			}
			if(row == n) // NOTE might remove else
			{ 
				for(const func::TubeFlowIntoFromNode& tube: tubes_flow_into_from_node)
				{
					const float vol = volume[tube.row][tube.col];
					add_fluid_table[tube.row][tube.col].fluid[0] = vol;
					fluid_displacements.in[0] = vol;
				}
				continue;
			}
			
			std::sort(tubes_flow_into_from_node.begin(), tubes_flow_into_from_node.end(), func::Integration::comparison_outflow);
			for(const auto& tpshin: tubes_flow_into_from_node)
			{
				//std::cout << "tube_push_out after sort: radius=" << tpshin.radius << ", r=" << tpshin.r << ", c=" << tpshin.c << std::endl;
				
				additions[tpshin.row][tpshin.col] = add_fluid(volume[tpshin.row][tpshin.col], vol_from_tube_into_node);
			}
		}
	}
	
	FluidAdditionResult fluid_addition_result
	{
		add_fluid_table,
		fluid_displacements
	};
	
	return fluid_addition_result;
}

TMns func::Global::trimmer(TMns mnsc, const Tfloat& velocity)
{
	return mnsc;
}

TMns func::Global::combine_additions(TMns mnsc, const Tfloat& velocity, const Tfloat& radius, const std::vector<std::vector<std::vector<float>>>& additions)
{
	for(int i = 0; i < mnsc.size(); ++ i)
	{
		auto& f = mnsc[i];
		for(int j = 0; j < f.size(); ++ j)
		{
			f[j].update(velocity[i][j], radius[i][j], additions[i][j]);
		}
	}
	
	return mnsc;
}

bool func::Integration::comparison_outflow(const func::TubeFlowIntoFromNode& first, const func::TubeFlowIntoFromNode& second)
{
	return first.rad < second.rad;
}

std::vector<float> func::Integration::add_fluid(const float vol, FluidAdditionToTube& tank)
{
	FluidAdditionToTube fluid_addtion_to_tube;
	const float vol_first_fluid = std::min(tank.fluid.front(), vol)
	// min_of(fluid_1_left_in_tank, amount_to_be_actually_pushed_in);
	
	fluid_addtion_to_tube.fluid.front() = vol_first_fluid;
	fluid_addtion_to_tube.fluid.back() = vol - vol_first_fluid;
	
	tank.remove_fluid(fluid_addtion_to_tube);
	
	return fluid_addtion_to_tube;
}

func::FluidAdditionToTube::FluidAdditionToTube(): fluid(2) {}
func::FluidDisplacements::FluidDisplacements(): in(2), out(2) {}

void func::FluidAdditionToTube::remove_fluid(const FluidAdditionToTube& add_fluid)
{
	for(size_t i = 0; i < this->fluid.size(); ++ i)
	{
		this->fluid[i] -= add_fluid[i];
	}
}
