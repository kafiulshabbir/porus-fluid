#include "dst/dstmns.h"

dst::Mns::Mns(): n(0), type(1), pos(2) {} //by default everything is the defending fluid

dst::Mns::Mns(const int n, const bool type,
	const double p1,	const double p2):
	n(n), type(type), pos{p1, p2} {}


// Part-0 Universial Functions
std::vector<double> dst::Mns::gen_pos_long() const
{
	/* generate positions long version
	 * 
	 * the generated std::vector<double> can be of 3 types:
	 * 1) [0, 1]
	 * 2) [0, pos1, 1]
	 * 3) [0, pos1, pos2, 1]
	 * 
	 * it is distinguished from the short version which is of the form
	 * 1) [], n = 0
	 * 2) [a], n = 1
	 * 3) [a, b], n = 2
	 */
 
	std::vector<double> v(n + 2);
	for(int i = 0; i < n; ++ i)
	{
		v[i + 1] = pos[i];
	}
	v.back() = 1;
	
	return v;
}


std::vector<double> dst::Mns::gen_pos_long_rev() const
{
	std::vector<double> pos = gen_pos_long();
	for(double& x: pos)
	{
		x = (1.0f - x);
	}
	
	std::reverse(pos.begin(), pos.end());
	
	return pos;
}


bool dst::Mns::type_rev() const
{
	const int t = this->type;
	return (this->n + t) % 2;
}

// Part-1 Linear Equations and pressure determination
double dst::Mns::sign_of_capll_pressure(int direction) const
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

double dst::Mns::true_is_minus_one(bool condition)
{
	if(condition)
	{
		return -1;
	}
	
	return 1;
}
 
double dst::Mns::mu(const double mu1, const double mu2) const
{
	std::vector<double> mu_vec{mu1, mu2};
	const auto pos_long = gen_pos_long();
	
	double sum = 0;
	const int pos_long_size = pos_long.size();
	for(int i = 1; i < pos_long_size; ++ i)
	{
		const int which_fluid = (i - 1 + type) % mu_vec.size();
		const double normalized_length = pos_long[i] - pos_long[i - 1];
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


// Part-3 Distribution --------------------------------------------------------

bool dst::Mns::is_the_flow_from_tube_into_node(const int direction,
	const double velocity) const
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

int dst::Mns::type_from_vel(const double vel) const
{
	if(vel < 0)
	{
		return type_rev();
	}
	
	return type;
}


int dst::Mns::type_from_direction(const int direction) const
{
	if(direction > 1)
	{
		return type_rev();
		
	}
	return type;	
}

std::vector<double> dst::Mns::pos_from_direction(const int direction) const
{
	if(direction > 1)
	{
		return gen_pos_long_rev();
		
	}
	
	return gen_pos_long();
}

std::vector<double> dst::Mns::pos_from_vel(const double vel) const
{
	if(vel < 0)
	{
		return gen_pos_long_rev();
		
	}
	
	return gen_pos_long();
}

std::vector<double> dst::Mns::pos_fluid_into_nodes(
	const int direction, const double pos_length) const
{
	const std::vector<double> pos = pos_from_direction(direction);
	const int type = type_from_direction(direction);
	
	std::vector<double> fluid(2);
	
	for(int i = 0; pos[i] < pos_length; ++ i)
	{
		const int current_type = (type + i) % 2;
		const double right_end = std::min(pos_length, pos[i + 1]);
		const double current_length = right_end - pos[i];
		fluid[current_type] += current_length;
	}
	
	return fluid;
}

std::vector<double> dst::Mns::vol_fluid_into_nodes(
	const double radius,
	const int direction,
	const double velocity,
	const double time_step,
	const double length_tube
	) const
{
	const double length_pos = std::abs(velocity) * time_step / length_tube;
	
	std::vector<double> pos = pos_fluid_into_nodes(
		direction, length_pos);
	
	const double area = declconst::PI * std::pow(radius, 2);
	for(double& x: pos)
	{
		x *= length_tube * area;
	}
	
	return pos;
}

	
	

// Part-4 Update --------------------------------------------------------------
void dst::Mns::update(
	const double vel,
	const double rad,
	const std::vector<double>& add
)
{
	if(vel == 0.0)
	{
		return;
	}
	
	const double area = declconst::PI * std::pow(rad, 2);
	const double l1 = add.front() / area / declconst::TUBE_LENGTH;
	const double l2 = add.back() / area / declconst::TUBE_LENGTH;
	const double l = l1 + l2;
	
	Cmprt cmprt_existing =
		gen_cmprt_existing(vel, l);
	Cmprt cmprt_addition = gen_cmprt_addition(l1, l2);
	
	Cmprt cmprt_merged =
		merge_existing_and_cmprt_addition(cmprt_existing, cmprt_addition, vel);
	Cmprt cmprt_without_dupl = remove_dupl_cmprt(cmprt_merged);
	
	bool type_begin_temp = cmprt_without_dupl.front().first;
	std::vector<double> pos_new_temp = cmprt_to_vector(cmprt_without_dupl);

	PosNew_Type_Result pos_new_and_type =
		centre_of_mass_recombination(type_begin_temp, pos_new_temp);
	
	n = pos_new_and_type.v.size();
	type = pos_new_and_type.type;
	
	for(int i = 0; i < n; ++ i)
	{
		pos[i] = pos_new_and_type.v[i];
	}
}

std::vector<double> dst::Mns::cmprt_to_vector(const dst::Mns::Cmprt& cmprt)
{
	std::vector<double> v;
	double sum = 0;
	for(const std::pair<bool, double>& section: cmprt)
	{
		sum += section.second;
		v.push_back(sum);
	}
	
	v.pop_back();
	return v;
}

dst::Mns::Cmprt dst::Mns::gen_cmprt_addition(
	const double l1,
	const double l2
) const
{
	Cmprt cmprt_addition;
	
	if(l1 > declconst::TRIMMER_PRECISION)
	{
		cmprt_addition.push_back({0, l1});
	}
	if(l2 > declconst::TRIMMER_PRECISION)
	{
		cmprt_addition.push_back({1, l2});
	}
	
	return cmprt_addition;
}
	
	
dst::Mns::Cmprt dst::Mns::gen_cmprt_existing(
	double vel,
	double dspl
) const
{
	const std::vector<double> pos_long = pos_from_vel(vel);
	const int type_begin = type_from_vel(vel);
	
	Cmprt compartments;
	
	const double l = 1.0 - dspl;
	for(int i = 0; pos_long[i] < l; ++ i)
	{
		const double min = std::min(l, pos_long[i + 1]);
		const double len_phase = min - pos_long[i];
		
		if(len_phase < declconst::TRIMMER_PRECISION)
		{
			continue;
		}
		
		const int type_temp = (type_begin + i) % 2;
		
		std::pair<bool, double> compartment
		{
			type_temp,
			len_phase
		};
		
		compartments.push_back(compartment);
	}
	
	return compartments;
}

dst::Mns::Cmprt dst::Mns::merge_existing_and_cmprt_addition
(
	dst::Mns::Cmprt& cmprt_existing,
	dst::Mns::Cmprt& cmprt_addition,
	double vel
)
{
	cmprt_addition.insert(cmprt_addition.end(), cmprt_existing.begin(), cmprt_existing.end());
	
	if(vel < 0)
	{
		std::reverse(cmprt_addition.begin(), cmprt_addition.end());
	}
	
	return cmprt_addition;
}

dst::Mns::Cmprt dst::Mns::remove_dupl_cmprt(const dst::Mns::Cmprt& cmprt_merged)
{
	Cmprt cmprt_without_dupl;

	for(const std::pair<bool, double>& section: cmprt_merged)
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


dst::Mns::PosNew_Type_Result dst::Mns::centre_of_mass_recombination
(
	const bool type_begin,
	const std::vector<double>& pos_new
) const
{
	if(pos_new.size() < 3)
	{
		return {type_begin, pos_new};
	}
	
	bool new_type_begin = type_begin;
	double l1 = -1;
	double l2 = -1;
	double l3 = -1;
	double l4 = -1;
	
	// NEEED CLARIFICATION SOON 15.05.2023 TRY BOTH
	if(pos_new.size() == 3)
	{
		if(type_begin)
		{
			l1 = pos_new[0];
			l2 = pos_new[1];
			l3 = pos_new[2];
			l4 = 1;	
		}
		else
		{
			l1 = 0;
			l2 = pos_new[0];
			l3 = pos_new[1];
			l4 = pos_new[2];
			new_type_begin = !type_begin;
		}
	}
	if(pos_new.size() == 4)
	{
		l1 = pos_new[0];
		l2 = pos_new[1];
		l3 = pos_new[2];
		l4 = pos_new[3];
	}
	
	return {new_type_begin, centre_of_mass_equation(l1, l2, l3, l4)};
}

std::vector<double> dst::Mns::centre_of_mass_equation
(
	const double l1,
	const double l2,
	const double l3,
	const double l4
)
{
	const double d1 = l2 - l1;
	const double d2 = l4 - l3;
	const double d = d1 + d2;
	const double c1 = (l1 + l2) / 2;
	const double c2 = (l3 + l4) / 2;
	
	const double L = (c1 * d1 + c2 * d2) / d;
	const double L1 = L - d / 2;
	const double L2 = L1 + d;
	
	return std::vector<double> {L1, L2};
}

// Part-5 Measurement ---------------------------------------------------------
double dst::Mns::sum_type_first() const
{
	const auto pos_long = gen_pos_long();
	double sum = 0;
	
	const int pos_long_size = pos_long.size();
	for(int i = 1 + type; i < pos_long_size; i += 2)
	{
		sum += pos_long[i] - pos_long[i - 1];
	}
	
	return sum;
}



// Part-6 Printing and Reading mnsc overloads ---------------------------------

double dst::Mns::printable() const
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
	std::cout << " Mns, (int n, bool type, double pos1, double pos2): ";
	
	dst::Mns ipt;
	cin >> ipt.n >> ipt.type >> ipt.pos.front() >> ipt.pos.back();
	mns = ipt;
	
	return cin;
}

std::ostream& operator<< (std::ostream& cout, const dst::Mns& mns)
{
	std::cout << "[n=" << mns.n << ", t=" << mns.type << ", pos=" << mns.pos.front() << ", " << mns.pos.back() << "]";
	return cout;
}
