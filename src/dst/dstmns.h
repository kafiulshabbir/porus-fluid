#ifndef MNS_H
#define MNS_H

#include "decl/declconst.h"
#include <list>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>

namespace dst
{
	class Mns
	{
		static float true_is_minus_one(bool condition);
		
		struct PosNew_Type_Result
		{
			bool type;
			std::vector<float> v;
		};
		
		PosNew_Type_Result gen_pos_new_and_type(const Cmprt& cmprt_addition) const;
		std::vector<float> gen_pos_long_after_dspl(float vel, float l) const;
		
		typedef std::list<std::pair<bool, float>> Cmprt;
		Cmprt gen_cmprt_existing(float vel, float l) const;
		Cmprt dst::Mns::gen_cmprt_addition(const float vel, const float l1, const float l2);
		
		static Cmprt merge_existing_and_cmprt_addition(Cmprt& cmprt_existing, const Cmprt& cmprt_addition, float vel);
		static dst::Mns::Cmprt remove_dupl_cmprt(const dst::Mns::Cmprt& cmprt_merged)
		static std::vector<float> dst::Mns::cmprt_to_vector(const dst::Mns::Cmprt& cmprt);
		
		static std::vector<float> gen_pos_from_segmented(std::vector<float> pos_segmented);
		
		
	public:
		int n; //number of meniscus present
		bool type; // 0 - corresponds to blue fluid - which is invading
		std::vector<float> pos; // positions of meniscus
		
		Mns();
		Mns(int number_mns, bool type_fluid_start, float position_meniscus_1, float position_meniscus_1);
		
		int number_mns() const;
		bool type_fluid_start() const;
		std::vector<float> positions_of_mns() const;
		
		std::vector<float> gen_pos_long() const;
		float sign_of_capll_pressure(int direction) const;
		
		float mu(const float mu1, const float mu2) const;
		float time(const float velocity, const float length, const float time_div) const;
		
		bool is_the_flow_from_tube_into_node(const int direction, const float velocity) const;
		bool type_fluid_into_node(int direction) const;
		
		void update(const float vel, const float r, const std::vector<float>& add, bool tube_with_minimum_time);
		
		float sum_type_first() const;
		float printable() const;
	};
}

std::ifstream& operator>> (std::ifstream& fin, dst::Mns& val);
std::ofstream& operator<< (std::ofstream& fout, const dst::Mns& val);
std::istream& operator>> (std::istream& cin, dst::Mns& mns);
#endif
