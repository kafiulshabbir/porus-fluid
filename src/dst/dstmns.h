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
		static float _scontb_sig(bool condition);
		std::vector<float> gen_pos_long_after_dspl(float vel, float l) const;
		typedef std::list<std::pair<bool, float>> Ccmprt;
		Ccmprt gen_cmprt_existing(float vel, float l) const;
		static Ccmprt merge_existing_and_new_cmprts(Ccmprt& cmprt_existing, const Ccmprt& cmprt_new, float vel);
		
		struct CmnsAfterDspl
		{
			bool type;
			std::vector<float> v;
		};
		
		static std::vector<float> gen_pos_from_segmented(std::vector<float> pos_segmented);
		CmnsAfterDspl gen_pos_new_and_type(const Ccmprt& cmprt_new) const;
		
	public:
		int n; //number of meniscus present
		bool type; // 0 - corresponds to blue fluid - which is invading
		std::vector<float> pos; // positions of meniscus
		
		Mns();
		Mns(int n, bool type, float p1, float p2);
		
		float mu(const float mu1, const float mu2) const;
		float time(const float velocity, const float length, const float time_div) const;
		void update(const float vel, const float r, const std::vector<float>& add);
		float scontb(int direction) const;
		bool is_flow_into_node(const int direction, const float velocity) const;
		bool type_fluid_into_node(int direction) const;
		float sum_type_first() const;
		std::vector<float> gen_pos_long() const;
		float printable() const;
	};
}

std::ifstream& operator>> (std::ifstream& fin, dst::Mns& val);
std::ofstream& operator<< (std::ofstream& fout, const dst::Mns& val);
std::istream& operator>> (std::istream& cin, dst::Mns& mns);
#endif
