#include "dst/dst::Mns.h"

/* FUNCTION DESCRIPTION - scontb
 * 
 * when we write the equation, about the volume
 * q = dV/dt = PI / 8 / MU * r ^ 4 / L * [(P[i] - P[j]) + 2 * SIGMA / r)
 * If there is high pressure and mescible fluid in the [i],
 * a positive sign before 2 * SIGMA / r means 
 * the pressure difference is made higher by the interface meniscus
 * 1) blue: 0
 * 2) grey: 0
 * 3) blue|grey: +1
 * 4) grey|blue: -1
 * 5) blue|grey|blue: 0
 * 6) grey|blue|grey: 0
 * 
 * sign going down means direction is 2 or 3,
 * for which type 0 means the meniscus is oriented away from [i],
 * giving a negative contribution of cappilary pressure
 */
float dst::Mns::_scontb_sig(bool condition) //scontb's sign function
{
	return condition ? -1 : 1;
}

/* generate positions long version
 * 
 * the generated std::vector<float> can be of 3 types:
 * 1) [0, 1]
 * 2) [0, pos1, 1]
 * 3) [0, pos1, pos2, 1]
 * 
 * it is distinguished from the short version which is of the form
 * 1) [], n = 0
 * 2) [a], n = 1
 * 3) [a, b], n = 2
 */
std::vector<float> dst::Mns::gen_pos_long_after_dspl(float vel, float l) const
{
	auto pos_long_after_dslp = gen_pos_long();
	if(vel < 0)
	{
		pos_long_after_dslp.front() = l;
	}
	else
	{
		pos_long_after_dslp.back() -= l;
	}
	
	return pos_long_after_dslp;
}


dst::Mns::Ccmprt dst::Mns::gen_cmprt_existing(float vel, float l) const
{
	const auto pos_long_after_dspl = gen_pos_long_after_dspl(vel, l);
	dst::Mns::Ccmprt cmprt_existing;
	for(int i = 1; i < pos_long_after_dspl.size(); ++ i)
	{
		cmprt_existing.push_back({(i + type - 1) % 2, pos_long_after_dspl[i] - pos_long_after_dspl[i - 1]});
	}
	
	return cmprt_existing;
}

dst::Mns::Ccmprt dst::Mns::merge_existing_and_new_cmprts(dst::Mns::Ccmprt& cmprt_existing, const dst::Mns::Ccmprt& cmprt_new, float vel)
{
	if(vel > 0)
	{
		cmprt_existing.insert(cmprt_existing.begin(), cmprt_new.crbegin(), cmprt_new.crend());
	}
	else
	{
		cmprt_existing.insert(cmprt_existing.end(), cmprt_new.begin(), cmprt_new.end());
	}
	return cmprt_existing;
}

struct dst::Mns::dst::Mns::CmnsAfterDspl
{
	bool type;
	std::vector<float> v;
};

std::vector<float> dst::Mns::gen_pos_from_segmented(std::vector<float> pos_segmented)
{
	pos_segmented.pop_back();
	for(int i = 1; i < pos_segmented.size(); ++ i)
	{
		pos_segmented[i] += pos_segmented[i - 1];
	}
	
	return pos_segmented;
}

dst::Mns::CmnsAfterDspl dst::Mns::gen_pos_new_and_type(const dst::Mns::Ccmprt& cmprt_new, const float threshold_fill) const
{
	std::vector<std::pair<int, float>> cmprt_new_temp_vector;
	for(const auto& x: cmprt_new) // Step-1 Filter out anything smaller than threshold_fill
	{
		if(x.second >= threshold_fill)
		{
			cmprt_new_temp_vector.push_back({x.first, x.second});
		}
	}
	
	for(int i = 1; i < cmprt_new_temp_vector.size(); ++ i) // Step-2 Merge any two compartments of the smae fluid type
	{
		if(cmprt_new_temp_vector[i - 1].first == cmprt_new_temp_vector[i].first)
		{
			cmprt_new_temp_vector[i - 1].first = -1;
			cmprt_new_temp_vector[i].second += cmprt_new_temp_vector[i - 1].second;
		}
	}
	
	int type_begin_temp = -1;
	std::vector<float> pos_segmented;
	for(const auto& x: cmprt_new_temp_vector)
	{
		if(x.first != -1)
		{
			if(type_begin_temp == -1)
			{
				type_begin_temp = x.first;
			}
			pos_segmented.push_back(x.second);
		}
	}
	
	const bool type_begin = type_begin_temp;
	const auto pos_new = gen_pos_from_segmented(pos_segmented);
	
	if(pos_new.size() < 3) // Depending on the number of meniscus, recombinate or preenet as it is
	{
		return {type_begin, pos_new};
	}
	if(pos_new.size() == 3)
	{
		const float l1 = 0;
		const float l2 = pos_new[0];
		const float l3 = pos_new[1];
		const float l4 = pos_new[2];
		
		const float d1 = l2 - l1;
		const float d2 = l4 - l3;
		const float d = d1 + d2;
		const float c1 = (l1 + l2) / 2;
		const float c2 = (l3 + l4) / 2;
		
		const float L1 = (c1 * d1 + c2 * d2) / d - d / 2;
		const float L2 = L1 + d;
		
		return {!type_begin, {L1, L2}};
	}
	if(pos_new.size() == 4)
	{
		const float l1 = pos_new[0];
		const float l2 = pos_new[1];
		const float l3 = pos_new[2];
		const float l4 = pos_new[3];
		
		const float d1 = l2 - l1;
		const float d2 = l4 - l3;
		const float d = d1 + d2;
		const float c1 = (l1 + l2) / 2;
		const float c2 = (l3 + l4) / 2;
		
		const float L1 = (c1 * d1 + c2 * d2) / d - d / 2;
		const float L2 = L1 + d;
		
		return {type_begin, {L1, L2}};
	}
	
	std::cout << "ER3-oversized decompartalization" << std::endl;
	return {type_begin, {-1, -1}};	
}

dst::Mns::dst::Mns(): n(0), type(1), pos(2) {} //by default everything is the defending fluid
dst::Mns::dst::Mns(int n, bool type, float p1, float p2): n(n), type(type), pos{p1, p2} {}

float dst::Mns::mu(const float mu1, const float mu2) const
{
	std::vector<float> muv{mu1, mu2};
	const auto pos_long = gen_pos_long();
	
	float sum = 0;
	for(int i = 1; i < pos_long.size(); ++ i)
	{
		sum += muv[(i - 1 + type) % muv.size()] * (pos_long[i] - pos_long[i - 1]);
	}
	
	return sum;
}

float dst::Mns::time(const float velocity, const float length, const float time_div) const
{
	if(n == 0)
	{
		return length / velocity / time_div;
	}
	
	float dspl = (velocity >= 0 ? (1 - pos[n - 1]): pos.front());
	dspl = std::min(1.0f / time_div, dspl);
	return length * dspl / velocity;
}

void dst::Mns::update(const float vel, const float r, const std::vector<float>& add, const float threshold_fill)
{
	const float area = simconsts::PI * std::pow(r, 2);
	const float l1 = add.front() / area;
	const float l2 = add.back() / area;
	const float l = l1 + l2;
	
	auto cmprt_existing = gen_cmprt_existing(vel, l);
	dst::Mns::Ccmprt cmprt_new{{0, l1}, {1, l2}};
	auto cmprt = merge_existing_and_new_cmprts(cmprt_existing, cmprt_new, vel);
	auto pos_new_and_type = gen_pos_new_and_type(cmprt, threshold_fill);
	n = pos_new_and_type.v.size();
	type = pos_new_and_type.type;
	pos = pos_new_and_type.v;
	pos.resize(2);
}

float dst::Mns::scontb(int direction) const
{
	return _scontb_sig(direction > 1) * _scontb_sig(type) * (n % 2);
}

/*		vel	| [true]	| [false]	|
 * drec		| above(<2)	| below(>=2)|
 * [true]+1	| out-0		| in-1		|
 * [flase]-1| in-1		| out-0		|
 */
 
bool dst::Mns::is_flow_into_node(const int direction, const float velocity) const
{
	return (direction < 2) ^ (velocity >= 0);
}

bool dst::Mns::type_fluid_into_node(int direction) const
{
	if(direction < 2) // if fluid is coming from the above
	{
		return type; // whatever is at the lowest part is what gets into the node
	}
	/*
	 * What is on the top part?
	 * 
	 * n	| type=0 | type=1 |
	 * 0	| 0		 | 1	  |
	 * 1	| 1		 | 0	  |
	 * 2	| 0		 | 1	  |
	 */
	 
	return type ^ (n % 2); 
}

float dst::Mns::sum_type_first() const
{
	const auto pos_long = gen_pos_long();
	float sum = 0;
	for(int i = 1 + type; i < pos_long.size(); i += 2)
	{
		sum += pos_long[i] - pos_long[i - 1];
	}
	
	return sum;
}

std::vector<float> dst::Mns::gen_pos_long() const
{
	std::vector<float> v(n + 2);
	for(int i = 0; i < n; ++ i)
	{
		v[i + 1] = pos[i];
	}
	v.back() = 1;
	
	return v;
}


std::ifstream& operator>> (std::ifstream& fin, dst::Mns& val)
{
	fin >> val.n >> val.type >> val.pos.front() >> val.pos.back();
	return fin;
}

std::ofstream& operator<< (std::ofstream& fout, const dst::Mns& val)
{
	fout << '\n' << val.n << ' ' << val.type << ' ' << val.pos.front() << ' ' << val.pos.back();
	return fout;
}












