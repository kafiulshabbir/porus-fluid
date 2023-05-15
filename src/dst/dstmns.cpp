#include "dst/dstmns.h"

dst::Mns::Mns(): n(0), type(1), pos(2) {} //by default everything is the defending fluid

dst::Mns::Mns(int n, bool type, float p1, float p2): n(n), type(type), pos{p1, p2} {}

// Data access
int dst::Mns::number_mns() const
{
	return n;
}

bool dst::Mns::type_fluid_start() const
{
	return type;
}

std::vector<float> dst::Mns::positions_of_mns() const
{
	return pos;
}

// Part-0 Universial Functions
std::vector<float> dst::Mns::gen_pos_long() const
{
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
 
	std::vector<float> v(n + 2);
	for(int i = 0; i < n; ++ i)
	{
		v[i + 1] = pos[i];
	}
	v.back() = 1;
	
	return v;
}



// Part-1 Linear Equations and pressure determination
float dst::Mns::sign_of_capll_pressure(int direction) const
{
	/*
	 * Direction	|	Type	|	Product
	 * 
	 * 	0 (+1)		|  false	|	+1
	 *  0 (+1)		|  true		|	-1
	 * 
	 * 	1 (+1)		|  false	|	+1
	 *  1 (+1)		|  true		|	-1
	 * 
	 * 	2 (-1)		|  false	|	-1
	 *  2 (-1)		|  true		|	+1
	 * 
	 * 	3 (-1)		|  false	|	-1
	 *  3 (-1)		|  true		|	+1
	 */
	 
	return true_is_minus_one(direction > 1) *
		true_is_minus_one(type) *
		(n % 2); // {even} = sign_of_capll_pressure = 0;
}

float dst::Mns::true_is_minus_one(bool condition)
{
	if(condition)
	{
		return -1;
	}
	
	return 1;
}
 
float dst::Mns::mu(const float mu1, const float mu2) const
{
	std::vector<float> mu_vec{mu1, mu2};
	const auto pos_long = gen_pos_long();
	
	float sum = 0;
	const int pos_long_size = pos_long.size();
	for(int i = 1; i < pos_long_size; ++ i)
	{
		const int which_fluid = (i - 1 + type) % mu_vec.size();
		const float normalized_length = pos_long[i] - pos_long[i - 1];
		sum += mu_vec[which_fluid] * normalized_length;
	}
	
	return sum;
}


 
/* FUNCTION DESCRIPTION - sign_of_capll_pressure
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



// Part-2 Minimum Time----------------------------------------------------------

float dst::Mns::time(const float velocity, const float length, const float time_div) const
{
	const float time_step_default = length / std::abs(velocity) / time_div;
	
	if(n == 0)
	{
		return time_step_default;
	}
	
	float displacement_active = pos.front();
	if(velocity >= 0)
	{
		displacement_active = 1.0 - pos[this->n - 1];
	}
	
	const float time_step = length * displacement_active / std::abs(velocity);
	return std::min(time_step, time_step_default);
}



// Part-3 Distribution --------------------------------------------------------

bool dst::Mns::is_the_flow_from_tube_into_node(const int direction,
	const float velocity) const
{
	/*
	 * direction	|	velocity	|	result = direction ^ velocity
	 * 
	 * 0 (1)		|	+ (1)		|	0
	 * 0 (1)		|	- (0)		|	1
	 * 
	 * 1 (1)		|	+ (1)		|	0
	 * 1 (1)		|	- (0)		|	1
	 * 
	 * 2 (0)		|	+ (1)		|	1
	 * 2 (0)		|	- (0)		|	0
	 * 
	 * 3 (0)		|	+ (1)		|	1
	 * 3 (0)		|	- (0)		|	0
	 */
	 
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
	 * direction	|	velocity	|	result = direction ^ velocity
	 * 
	 * 0 (1)		|	+ (1)		|	0
	 * 0 (1)		|	- (0)		|	1
	 * 
	 * 1 (1)		|	+ (1)		|	0
	 * 1 (1)		|	- (0)		|	1
	 * 
	 * 2 (0)		|	+ (1)		|	1
	 * 2 (0)		|	- (0)		|	0
	 * 
	 * 3 (0)		|	+ (1)		|	1
	 * 3 (0)		|	- (0)		|	0
	 */
	 
	return type ^ (n % 2); 
}



// Part-4 Update --------------------------------------------------------------
void dst::Mns::update(const float vel, const float rad,
	const std::vector<float>& add, bool tube_with_minimum_time)
{
	const float area = declconst::PI * std::pow(rad, 2);
	const float l1 = add.front() / area / declconst::TUBE_LENGTH;
	const float l2 = add.back() / area / declconst::TUBE_LENGTH;
	const float l = l1 + l2;
	
	Cmprt cmprt_existing = gen_cmprt_existing(vel, l, tube_with_minimum_time);
	Cmprt cmprt_additon = gen_cmprt_addition(vel, l1, l2);
	
	Cmprt cmprt_merged = merge_existing_and_cmprt_addition(cmprt_existing, cmprt_addition, vel);
	Cmprt cmprt_without_dupl = remove_dupl_cmprt(cmprt_merged);
	
	const bool type_begin_temp = cmprt_without_dupl.front().first;
	std::vector<float> pos_new_temp = cmprt_to_vector(cmprt);
	
	PosNew_Type_Result pos_new_and_type = gen_pos_new_and_type(type_begin_temp, pos_new_temp, vel);
	
	n = pos_new_and_type.v.size();
	type = pos_new_and_type.type;
	
	for(int i = 0; i < n; ++ i)
	{
		pos[i] = pos_new_and_type.v[i];
	}
}

std::vector<float> dst::Mns::cmprt_to_vector(const dst::Mns::Cmprt& cmprt)
{
	std::vector<float> v;
	float sum = 0;
	for(const std::pair<bool, float>& section: cmprt)
	{
		sum += section.second;
		v.push_back(sum);
	}
	
	v.pop_back();
	return v;
}

Cmprt dst::Mns::gen_cmprt_addition(const float vel, const float l1, const float l2)
{
	Cmprt cmprt_addition;
	
	if(l1 > 0)
	{
		cmprt_addition.push_back{0, l1};
	}
	if(l2 > 0)
	{
		cmprt_addition.push_back{1, l2};
	}
	
	if(vel > 0)
	{
		std::reverse(cmprt_addition.begin(), cmprt_addition.end());
	}
	
	return cmprt_addition;
}
	
	
dst::Mns::Cmprt dst::Mns::gen_cmprt_existing(float vel,
	float l, bool tube_with_minimum_time) const
{
	//typedef std::list<std::pair<bool, float>> Cmprt;
	std::vector<float> pos_long_after_dspl = gen_pos_long_after_dspl(vel, l);
	
	Cmprt cmprt_existing;
	
	const int pos_long_after_dspl_size = pos_long_after_dspl.size();
	
	for(int i = 1; i < pos_long_after_dspl_size; ++ i)
	{
		const int fluid_type = (i + type - 1) % 2;
		const float length_fluid = pos_long_after_dspl[i] -
			pos_long_after_dspl[i - 1];
			
		std::pair<bool, float> cmprt_section
		{
			fluid_type,
			length_fluid
		};
		
		cmprt_existing.push_back(cmprt_section);
	}
	
	if(tube_with_minimum_time)
	{
		if(vel > 0)
		{
			cmprt_existing.pop_back();
		}
		else
		{
			cmprt_existing.pop_front();
		}
	}
	
	return cmprt_existing;
}


std::vector<float> dst::Mns::gen_pos_long_after_dspl(float vel, float l) const
{
	std::vector<float> pos_long_after_dspl = gen_pos_long();
	
	if(vel < 0)
	{
		pos_long_after_dspl.front() = l;
	}
	else
	{
		pos_long_after_dspl.back() -= l;
	}
	
	return pos_long_after_dspl;
}

dst::Mns::Cmprt dst::Mns::merge_existing_and_cmprt_addition(dst::Mns::Cmprt& cmprt_existing, dst::Mns::Cmprt& cmprt_addition, float vel)
{
	if(vel > 0)
	{
		cmprt_addition.insert(cmprt_addition.end(), cmprt_existing.begin(), cmprt_existing.end());
		return cmprt_addition
	}

	cmprt_existing.insert(cmprt_existing.end(), cmprt_addition.begin(), cmprt_addition.end());
	return cmprt_existing;
}

dst::Mns::Cmprt dst::Mns::remove_dupl_cmprt(const dst::Mns::Cmprt& cmprt_merged)
{
	Cmprt cmprt_without_dupl;

	for(const std::pair<bool, float>& section: cmprt_merged)
	{
		if(cmprt_without_dupl.empty())
		{
			cmprt_without_dupl.push_back(section);
			continue;
		}
		
		if(cmprt_without_dupl.back().first == section.first)
		{
			cmprt_without_dupl.back().second += section.second;
			continue;
		}
		
		cmprt_without_dupl.push_back(section);
	}
	
	return cmprt_without_dupl;
}


dst::Mns::PosNew_Type_Result dst::Mns::gen_pos_new_and_type(bool type_begin, const vector<float> pos_new, const float vel) const
{
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
	
	return {type_begin, pos_new};
}


std::vector<float> dst::Mns::gen_pos_from_segmented(std::vector<float> pos_segmented)
{
	pos_segmented.pop_back();
	
	const int pos_segmented_size = pos_segmented.size();
	for(int i = 1; i < pos_segmented_size; ++ i)
	{
		pos_segmented[i] += pos_segmented[i - 1];
	}
	
	return pos_segmented;
}




// Part-5 Measurement ---------------------------------------------------------
float dst::Mns::sum_type_first() const
{
	const auto pos_long = gen_pos_long();
	float sum = 0;
	
	const int pos_long_size = pos_long.size();
	for(int i = 1 + type; i < pos_long_size; i += 2)
	{
		sum += pos_long[i] - pos_long[i - 1];
	}
	
	return sum;
}



// Part-6 Printing and Reading mnsc overloads ---------------------------------

float dst::Mns::printable() const
{
	const auto amount_type_first = sum_type_first();
	const auto amount_type_second = 1.0f - amount_type_first;
	if(amount_type_first > amount_type_second)
	{
		return amount_type_first * 100;
	}
	return -amount_type_second * 100;
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

std::istream& operator>> (std::istream& cin, dst::Mns& mns)
{
	std::cout << " Mns, (int n, bool type, float pos1, float pos2): ";
	
	dst::Mns ipt;
	cin >> ipt.n >> ipt.type >> ipt.pos.front() >> ipt.pos.back();
	mns = ipt;
	
	return cin;
}
