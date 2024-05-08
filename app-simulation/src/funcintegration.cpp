#include "funcintegration.h"


/*
 * integrate returns a Tmns - new meniscus configuration
 *
 *
 */


/*
 * integrate(...)
 * {
 * 		calculate fluid_table
 * 		generate new men meniscus-configuration from fluid table by combining.
 * }
 */

func::Integration::IntegrationResult func::Integration::integrate(
	const Tdouble& radius,
	const TMns& mnsc,
	const Tdouble& velocity,
	const Tdouble& volume,
	const dst::Diamension& diamension,
	const double time_step
)
{
	auto fluid_table_result = calculate_fluid_table(
		radius,
		mnsc,
		velocity,
		volume,
		diamension,
		time_step
	);


	const auto new_meniscus_configuration = combine_fluid_additions(
		radius,
		mnsc,
		velocity,
		diamension,
		fluid_table_result.fluid_table
	);

	IntegrationResult result;
	result.new_mnsc = new_meniscus_configuration;
	result.fluid_injected = fluid_table_result.injection;
	result.fluid_expelled = fluid_table_result.expulsion;

	return result;
}

func::Integration::CalculateFluidTableResult func::Integration::calculate_fluid_table
(
	const Tdouble& radius,
	const TMns& mnsc,
	const Tdouble& velocity,
	const Tdouble& volume,
	const dst::Diamension& diamension,
	const double time_step
)
{
	TFluid fluid_addition_table = diamension.empty_table_templated<Fluid>();
	std::vector<double> injection_vec;
	std::vector<double> expulsion_vec;

	for(int row = 0; row < diamension.node_rows(); ++ row)
	{
		const int max_cols_in_this_row = diamension.node_cols(row);
		for(int col = 0; col < max_cols_in_this_row; ++ col)
		{
			Tank tank;
			std::vector<Tube_FromNode> tubes_from_node_vec;

			const std::vector<dst::Tube> tubes_connected_vec =
				diamension.generate_tubes_connected_to_node(row, col);

			const int connections_size = tubes_connected_vec.size();
			double vol_fluid_incoming = 0;
			double vol_fluid_outgoing = 0;
			for(int direction = 0; direction < connections_size; ++ direction)
			{
				const dst::Tube& connection = tubes_connected_vec[direction];
				if(connection.active)
				{
					const double rad = radius[connection.row][connection.col];
					const dst::Mns& mns = mnsc[connection.row][connection.col];
					const double vel = velocity[connection.row][connection.col];
					const double vol = volume[connection.row][connection.col];

					if(mns.is_the_flow_from_tube_into_node(direction, vel))
					{
						const std::vector<double> add_fluid =
							mns.vol_fluid_into_nodes(
								rad,
								direction,
								vel,
								time_step,
								declconst::TUBE_LENGTH
							);

						vol_fluid_incoming += vol;

						tank.add_fluid(add_fluid);
						continue;
					}

					Tube_FromNode tube
					{
						rad,
						connection.row,
						connection.col
					};

					tubes_from_node_vec.push_back(tube);
					vol_fluid_outgoing += vol;
				}
			}

			/*
			std::cout << std::endl << "--tubes and tank row=" << row
				<< ", col=" << col << std::endl;

			std::cout << "tank initial- blue="
				<< tank.show_blue() << " grey="	<< tank.show_grey()
				<< " yes-blue=" << tank.yes_blue() << " yes-grey"
				<< tank.yes_grey()<< std::endl;

			*/

			std::sort(tubes_from_node_vec.begin(),
				tubes_from_node_vec.end(),
				compare_where_wetting_fluid_go_first);

			/*
			for(const Tube_FromNode& tube: tubes_from_node_vec)
			{
				std::cout << "tube: r=" << tube.rad << " row=" << tube.row << " col=" << tube.col << std::endl;
			}
			*/

			if(diamension.is_this_an_open_node(row, col))
			{
				if(vol_fluid_outgoing > vol_fluid_incoming)
				{
					const double vol_fluid_to_be_injected_into_node =
						vol_fluid_outgoing - vol_fluid_incoming;

					injection_vec.push_back(vol_fluid_to_be_injected_into_node);
					expulsion_vec.push_back(0);
					const std::vector<double> add_fluid_vec
						{2 * vol_fluid_to_be_injected_into_node, 0};

					tank.add_fluid(add_fluid_vec);
				}
				else
				{
					const double vol_fluid_to_be_expelled_from_node =
						vol_fluid_incoming - vol_fluid_outgoing ;

					injection_vec.push_back(0);
					expulsion_vec.push_back(vol_fluid_to_be_expelled_from_node);
				}
			}

			for(const Tube_FromNode& tpshin: tubes_from_node_vec)
			{
				fluid_addition_table[tpshin.row][tpshin.col] = tank.pour_out_fluid(volume[tpshin.row][tpshin.col]);
			}

			/*
			std::cout << "tank final- blue="
				<< tank.show_blue() << " grey="	<< tank.show_grey()
				<< " yes-blue=" << tank.yes_blue() << " yes-grey"
				<< tank.yes_grey()<< std::endl;
			*/
		}
	}

	/*
	std::cout << std::endl << "Diagonistics of Addition table" << std::endl;
	for(int row = 0; row < diamension.rows; ++ row)
	{
		for(int col = 0; col < diamension.cols; ++ col)
		{
			const Fluid& fluid = fluid_addition_table[row][col];
			std::cout << "{" << fluid.blue << ", " << fluid.grey << "}" << '\t';
		}
		std::cout << std::endl;
	}
	*/
	CalculateFluidTableResult result;
	result.fluid_table = fluid_addition_table;
	result.injection = injection_vec;
	result.expulsion = expulsion_vec;

	return result;
}

TMns func::Integration::combine_fluid_additions
(
	const Tdouble& radius,
	TMns mnsc,
	const Tdouble& velocity,
	const dst::Diamension& diamension,
	const TFluid& fluid_addition_table
)
{
	for(int row = 0; row < diamension.rows; ++ row)
	{
		for(int col = 0; col < diamension.cols; ++ col)
		{
			const double vel = velocity[row][col];
			const double rad = radius[row][col];
			const std::vector<double> add_fluid_from_tank_to_tube
			{
				fluid_addition_table[row][col].blue,
				fluid_addition_table[row][col].grey
			};

			mnsc[row][col].update(vel, rad, add_fluid_from_tank_to_tube);
		}
	}

	return mnsc;
}

bool func::Integration::compare_where_wetting_fluid_go_first(const Tube_FromNode& first, const Tube_FromNode& second)
{
	return first.rad < second.rad;
}

void func::Integration::Tank::add_fluid(const std::vector<double>& add)
{
	if(add.front() > 0.0f)
	{
		fluid.blue += add.front();
		blue_present = true;
	}
	if(add.back() > 0.0f)
	{
		fluid.grey += add.back();
		grey_present = true;
	}
}

func::Integration::Fluid func::Integration::Tank::pour_out_fluid(const double vol)
{
	Fluid fluid_result;
	if((!blue_present) && (!grey_present))
	{
		return fluid_result;
	}

	if(!grey_present) //there is no grey
	{
		this->fluid.blue = std::max<double>(0, this->fluid.blue - vol);

		fluid_result.blue = vol;
		return fluid_result;
	}

	if(!blue_present) //there is no blue
	{
		this->fluid.grey = std::max<double>(0, this->fluid.grey - vol);
		fluid_result.grey = vol;
		return fluid_result;
	}

	if(vol < this->fluid.blue)
	{
		this->fluid.blue -= vol;
		fluid_result.blue = vol;
		return fluid_result;
	}

	fluid_result.blue = this->fluid.blue;
	this->fluid.blue = 0;

	fluid_result.grey = vol - fluid_result.blue;
	this->fluid.grey = std::max<double>(0, this->fluid.grey - fluid_result.grey);

	return fluid_result;
}

double func::Integration::Tank::show_blue() const
{
	return fluid.blue;
}

double func::Integration::Tank::show_grey() const
{
	return fluid.grey;
}

std::string func::Integration::Tank::yes_blue() const
{
	return (blue_present ? "yes" : "no");
}

std::string func::Integration::Tank::yes_grey() const
{
	return (grey_present ? "yes" : "no");
}
