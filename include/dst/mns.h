#ifndef DEF_CMNS
#define DEF_CMNS

#include "decs/simconsts.h"
#include <list>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>

class mns
{
	static float _scontb_sig(bool condition);
	std::vector<float> gen_pos_long_after_dspl(float vel, float l) const;
	typedef std::list<std::pair<bool, float>> Ccmprt;
	Ccmprt gen_cmprt_existing(float vel, float l) const;
	static Ccmprt merge_existing_and_new_cmprts(Ccmprt& cmprt_existing, const Ccmprt& cmprt_new, float vel);
	struct CmnsAfterDspl;
	static std::vector<float> gen_pos_from_segmented(std::vector<float> pos_segmented);
	CmnsAfterDspl gen_pos_new_and_type(const Ccmprt& cmprt_new, const float threshold_fill) const;
	
public:
	int n; //number of meniscus present
	bool type; // 0 - corresponds to blue fluid - which is invading
	std::vector<float> pos; // positions of meniscus
	
	mns();
	mns(int n, bool type, float p1, float p2);
	
	float mu(const float mu1, const float mu2) const;
	float time(const float velocity, const float length, const float time_div) const;
	void update(const float vel, const float r, const std::vector<float>& add, const float threshold_fill);
	float scontb(int direction) const;
	bool is_flow_into_node(const int direction, const float velocity) const;
	bool type_fluid_into_node(int direction) const;
	float sum_type_first() const;
	std::vector<float> gen_pos_long() const;
};

std::ifstream& operator>> (std::ifstream& fin, mns& val);
std::ofstream& operator<< (std::ofstream& fout, const mns& val);
#endif
