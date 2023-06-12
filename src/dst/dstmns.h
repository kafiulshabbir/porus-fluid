#ifndef MNS_H
#define MNS_H

#include "decl/declconst.h"
#include <list>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>

namespace dst
{
	class Mns
	{
		typedef std::list<std::pair<bool, double>> Cmprt;
		struct PosNew_Type_Result
		{
			bool type;
			std::vector<double> v;
		};
		
		std::vector<double> gen_pos_long_rev() const;
		bool type_rev() const;
		
		static double true_is_minus_one(bool condition);
		
		PosNew_Type_Result centre_of_mass_recombination(const bool begin_type,
			const std::vector<double>& pos_new) const;
			
		static std::vector<double> centre_of_mass_equation(const double l1,
			const double l2, const double l3, const double l4);
		
		Cmprt gen_cmprt_existing(
			const double vel, const double l
		) const;
		
		Cmprt gen_cmprt_addition(
			const double l1,
			const double l2
		) const;
		
		static Cmprt merge_existing_and_cmprt_addition(
			Cmprt& cmprt_existing,
			Cmprt& cmprt_addition,
			const double vel
		);
		static Cmprt remove_dupl_cmprt(const Cmprt& cmprt_merged);
		
		static std::vector<double> cmprt_to_vector(const Cmprt& cmprt);
		
		int type_from_direction(const int direction) const;
		std::vector<double> pos_from_direction(const int direction) const;
		std::vector<double> pos_fluid_into_nodes(
			const int direction, const double pos_length) const;
		
		std::vector<double> pos_from_vel(const double vel) const;
		int type_from_vel(const double vel) const;
		
	public:
		int n; //number of meniscus present
		bool type; // 0 - corresponds to blue fluid - which is invading
		std::vector<double> pos; // positions of meniscus
		
		Mns();
		Mns(const int number_mns, const bool type_fluid_start,
			const double position_mns_1, const double position_mns_2);

		std::vector<double> gen_pos_long() const;
		
		double sign_of_capll_pressure(int direction) const;
		
		double mu(const double mu1, const double mu2) const;
		
		bool is_the_flow_from_tube_into_node(const int direction, const double velocity) const;
		std::vector<double> vol_fluid_into_nodes(
			const double radius,
			const int direction,
			const double velocity,
			const double time_step,
			const double length_tube
			) const;
			
		
		void update(const double vel, const double r,
			const std::vector<double>& add);
		
		double sum_type_first() const;
		double printable() const;

	};
}

std::ifstream& operator>> (std::ifstream& fin, dst::Mns& val);
std::ofstream& operator<< (std::ofstream& fout, const dst::Mns& val);
std::istream& operator>> (std::istream& cin, dst::Mns& mns);
std::ostream& operator<< (std::ostream& cout, const dst::Mns& mns);
#endif
