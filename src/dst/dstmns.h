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
		typedef std::list<std::pair<bool, float>> Cmprt;
		struct PosNew_Type_Result
		{
			bool type;
			std::vector<float> v;
		};
		
		static float true_is_minus_one(bool condition);
		
		PosNew_Type_Result centre_of_mass_recombination(const bool begin_type,
			const std::vector<float>& pos_new) const;
		std::vector<float> gen_pos_long_after_dspl(float vel, float l) const;
		
		Cmprt gen_cmprt_existing(const float vel,
			const float l, const bool tube_with_minimum_time) const;
		Cmprt gen_cmprt_addition(const float vel,
			const float l1, const float l2);
		
		static Cmprt merge_existing_and_cmprt_addition(Cmprt& cmprt_existing,
			Cmprt& cmprt_addition, const float vel);
		static Cmprt remove_dupl_cmprt(const Cmprt& cmprt_merged);
		
		static std::vector<float> cmprt_to_vector(const Cmprt& cmprt);
		
		
		
	public:
		struct TimeType
		{
			float time;
			bool does_mns_reach_node;
		};
		
		int n; //number of meniscus present
		bool type; // 0 - corresponds to blue fluid - which is invading
		std::vector<float> pos; // positions of meniscus
		
		Mns();
		Mns(const int number_mns, const bool type_fluid_start,
			const float position_mns_1, const float position_mns_2);
		
		int number_mns() const;
		bool type_fluid_start() const;
		std::vector<float> positions_of_mns() const;
		
		std::vector<float> gen_pos_long() const;
		float sign_of_capll_pressure(int direction) const;
		
		float mu(const float mu1, const float mu2) const;
		TimeType time(const float velocity, const float length, const float time_div) const;
		
		bool is_the_flow_from_tube_into_node(const int direction, const float velocity) const;
		bool type_fluid_into_node(int direction) const;
		
		void update(const float vel, const float r,
			const std::vector<float>& add,
			const bool tube_with_minimum_time);
		
		float sum_type_first() const;
		float printable() const;
		
		Mns inverse() const;
	};
}

std::ifstream& operator>> (std::ifstream& fin, dst::Mns& val);
std::ofstream& operator<< (std::ofstream& fout, const dst::Mns& val);
std::istream& operator>> (std::istream& cin, dst::Mns& mns);
#endif
