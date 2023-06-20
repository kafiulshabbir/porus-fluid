#include "fileio/fileioread.h"
#include "cmdio/cmdioprint.h"
#include "func/funcdetermine.h"

int main()
{
	fileio::Data data = fileio::Read::loop_until_proper_files();
	
	const std::vector<std::vector<int>> add_mnsc
		= func::Determine::gen_add_mnsc(data.mnsc, data.diamension);
		
	cmdio::Print::table(add_mnsc);
	
	return 0;
}

/*

const double time_step = 1;
const double length_tube = 1;
const double radius = 1;


const std::vector<dst::Mns> mns_vec
	{
		dst::Mns(0, 0, -1, -1), //blue
		dst::Mns(0, 1, -1, -1), //grey
		dst::Mns(1, 0, 0.75, -1), //[0.75blue, 0.25grey]
		dst::Mns(2, 0, 0.75, 0.85) //[0.75blue, 0.10grey, 0.15blue]
	};
	
	const std::vector<int> direc_vec{0, 1, 2, 3};
	
	
	const std::vector<double> velocity{0.1, 0.2, 0.3, 0.4, 0.5, 0.8};
	
	for(const dst::Mns& mns: mns_vec)
	{
		std::cout << std::endl << std::string(50, '-') << std::endl;
		for(const double vel: velocity)
		{
			for(const int direc: direc_vec)
			{
				process(mns, vel, direc);
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}
	}
	

void process(const dst::Mns& mns, const double vel, const int direc)
{
	std::cout << mns;
	std::cout << "vel=" << vel << ", direc=" << direc << std::endl;
	
	const std::vector<double> vol = mns.vol_fluid_into_nodes(
		radius,
		direc,
		vel,
		time_step,
		length_tube
	);
	
	std::cout << "blue=" << vol.front() / declconst::PI << ", grey=" << vol.back() / declconst::PI;
}

Tdouble v
{
	{2, -1, -1, 10}, //s12 = 10
	{-1, 2, -1, -110}, //s13 = -100
	{-1, -1, 2, 100}
};

Tdouble v
{
	{1,1,2,1,1,0,1,8},
	{2,1,2,3,1,3,0,21},
	{1,-1,-1,2,4,-5,6,12},
	{1,0,2,2,0,1,3,7},
	{-1,-2,0,4,3,2,4,23},
	{0,6,5,4,3,2,1,35},
	{4,5,10,16,12,3,15,106}
};


math::Linear::gauss_elimination(v);


// Tests for diamension

dst::Diamension d1(2, 4);
dst::Diamension d2(6, 4);

std::cout << "total-nodes=" << d1.total_nodes() << std::endl;
std::cout << "rows=" << d1.node_rows() << std::endl;
std::cout << "cols0=" << d1.node_cols(0) << std::endl;
std::cout << "cols1=" << d1.node_cols(1) << std::endl;

std::cout << "total-nodes=" << d2.total_nodes() << std::endl;
std::cout << "rows=" << d2.node_rows() << std::endl;
std::cout << "cols0=" << d2.node_cols(0) << std::endl;
std::cout << "cols1=" << d2.node_cols(1) << std::endl;

for(int i = 0; i < d1.node_rows(); ++ i)
{
	const int j_max = d1.node_cols(i);
	for(int j = 0; j < j_max; ++ j)
	{
		std::cout << "(" << i << ", " << j << ") " <<
			d1.linear_node_from_coordinate(i, j) <<
			":" << std::endl;
		
		const std::vector<dst::Tube> connecv =
			d1.generate_tubes_connected_to_node(i, j);
			
		for(const dst::Tube& tube: connecv)
		{
			std::cout << tube << std::endl;
		}
		
		std::cout << std::endl;
	}
}

for(int i = 0; i < 2; ++ i)
{
	for(int j = 0; j < 4; ++ j)
	{
		const std::pair<int, int> p = 
			d1.linear_node_at_ends_of_tube(i, j);
		
		std::cout << "(" << i << ", " << j << "):"
			<< p.first << ", " << p.second << std::endl;
			
	}
}
*/
