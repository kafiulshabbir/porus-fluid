#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <utility>
#include <limits>
#include <list>

#include "drw.h"

// Type Definitions
typedef float real; //float can be changed to double or long double to increase the precision, float is used to make the program faster
class Cmns; //Class of Meniscus
typedef real Cradius; //Class of Radius
typedef std::vector<std::vector<real>> Treal; //Table of real numbers
typedef std::vector<std::vector<Cmns>> Tmns;
typedef std::vector<std::vector<Cradius>> Tradius;

//GENERAL CONSTANTS
const real PI = std::acos(-1);
const real HUGE = std::numeric_limits<real>::max();

//Physical Characterstics of the experiment
const real PRESSURE_BOTTOM = 100;
const real PRESSURE_TOP = 2;
const real SIGMA = 0; //7.56e-2; // FOR water at 20C in SI units, produces 75Pa pressure difference for tube of radius 1mm
const real TUBE_LENGTH = 1;//0.1;
const real MU1 = 1;//1e-3; // viscosity of the invading liquid: water
const real MU2 = 1;//1e-5; // viscosity of defending liquid: air

//Parameters of simulation
const real THRESHOLD_FILL = 0.001; //if any meniscus is smaller than this proportion, then it is destroyed
const real TIME_DIV = 10; // if the nearest meniscus by time is further, then L / TIME_DIV is prefered
const int IMAGE_SIZE = 1000;

//Input Output of File names
const std::string FILE_NAME_RADIUS = "radius.txt";
const std::string FILE_NAME_MNS = "fill.txt";
const std::string FOLDER_SAVE_PIC = "pic/";

class Cmns
{
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
	
	static real _scontb_sig(bool condition) //scontb's sign function
	{
		return condition ? -1 : 1;
	}
	
	/* generate positions long version
	 * 
	 * the generated std::vector<real> can be of 3 types:
	 * 1) [0, 1]
	 * 2) [0, pos1, 1]
	 * 3) [0, pos1, pos2, 1]
	 * 
	 * it is distinguished from the short version which is of the form
	 * 1) [], n = 0
	 * 2) [a], n = 1
	 * 3) [a, b], n = 2
	 */
	 
	std::vector<real> gen_pos_long() const
	{
		std::vector<real> v(n + 2);
		for(int i = 0; i < n; ++ i)
		{
			v[i + 1] = pos[i];
		}
		v.back() = 1;
		
		return v;
	}
		

	/*		vel	| [true]	| [false]	|
	 * drec		| above(<2)	| below(>=2)|
	 * [true]+1	| out-0		| in-1		|
	 * [flase]-1| in-1		| out-0		|
	 */
	 
	
	
	std::vector<real> gen_pos_long_after_dspl(real vel, real l) const
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
	
	//generate compartments of the configuration which exists
	typedef std::list<std::pair<bool, real>> Ccmprt;
	Ccmprt gen_cmprt_existing(real vel, real l) const
	{
		const auto pos_long_after_dspl = gen_pos_long_after_dspl(vel, l);
		Ccmprt cmprt_existing;
		for(int i = 1; i < pos_long_after_dspl.size(); ++ i)
		{
			cmprt_existing.push_back({(i + type - 1) % 2, pos_long_after_dspl[i] - pos_long_after_dspl[i - 1]});
		}
		
		return cmprt_existing;
	}
	
	static Ccmprt merge_existing_and_new_cmprts(Ccmprt& cmprt_existing, const Ccmprt& cmprt_new, real vel)
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
	
	struct CmnsAfterDspl
	{
		bool type;
		std::vector<real> v;
	};
	
	static std::vector<real> gen_pos_from_segmented(std::vector<real> pos_segmented)
	{
		pos_segmented.pop_back();
		for(int i = 1; i < pos_segmented.size(); ++ i)
		{
			pos_segmented[i] += pos_segmented[i - 1];
		}
		
		return pos_segmented;
	}
	
	CmnsAfterDspl gen_pos_new_and_type(const Ccmprt& cmprt_new, const real threshold_fill) const
	{
		std::vector<std::pair<int, real>> cmprt_new_temp_vector;
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
		std::vector<real> pos_segmented;
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
			const real l1 = 0;
			const real l2 = pos_new[0];
			const real l3 = pos_new[1];
			const real l4 = pos_new[2];
			
			const real d1 = l2 - l1;
			const real d2 = l4 - l3;
			const real d = d1 + d2;
			const real c1 = (l1 + l2) / 2;
			const real c2 = (l3 + l4) / 2;
			
			const real L1 = (c1 * d1 + c2 * d2) / d - d / 2;
			const real L2 = L1 + d;
			
			return {!type_begin, {L1, L2}};
		}
		if(pos_new.size() == 4)
		{
			const real l1 = pos_new[0];
			const real l2 = pos_new[1];
			const real l3 = pos_new[2];
			const real l4 = pos_new[3];
			
			const real d1 = l2 - l1;
			const real d2 = l4 - l3;
			const real d = d1 + d2;
			const real c1 = (l1 + l2) / 2;
			const real c2 = (l3 + l4) / 2;
			
			const real L1 = (c1 * d1 + c2 * d2) / d - d / 2;
			const real L2 = L1 + d;
			
			return {type_begin, {L1, L2}};
		}
		else
		{
			std::cout << "ER3-oversized decompartalization" << std::endl;
		}
	}
	
public:
	int n; //number of meniscus present
	bool type; // 0 - corresponds to blue fluid - which is invading
	std::vector<real> pos; // positions of meniscus
	
	Cmns(): n(0), type(1), pos(2) {} //by default everything is the defending fluid
	Cmns(int n, bool type, real p1, real p2): n(n), type(type), pos{p1, p2} {}
	
	real mu(const real mu1, const real mu2) const
	{
		std::vector<real> muv{mu1, mu2};
		const auto pos_long = gen_pos_long();
		
		real sum = 0;
		for(int i = 1; i < pos_long.size(); ++ i)
		{
			sum += muv[(i - 1 + type) % muv.size()] * (pos_long[i] - pos_long[i - 1]);
		}
		
		return sum;
	}
	
	real time(const real velocity, const real length, const real time_div)
	{
		if(n == 0)
		{
			return length / velocity / time_div;
		}
		
		real dspl = (velocity >= 0 ? (1 - pos[n - 1]): pos.front());
		dspl = std::min(1.0f / time_div, dspl);
		return length * dspl / velocity;
	}
	
	void update(const real vel, const real r, const std::vector<real>& add, const real threshold_fill)
	{
		const real area = PI * std::pow(r, 2);
		const real l1 = add.front() / area;
		const real l2 = add.back() / area;
		const real l = l1 + l2;
		
		auto cmprt_existing = gen_cmprt_existing(vel, l);
		Ccmprt cmprt_new{{0, l1}, {1, l2}};
		auto cmprt = merge_existing_and_new_cmprts(cmprt_existing, cmprt_new, vel);
		auto pos_new_and_type = gen_pos_new_and_type(cmprt, threshold_fill);
		n = pos_new_and_type.v.size();
		type = pos_new_and_type.type;
		pos = pos_new_and_type.v;
		pos.resize(2);
	}
	
	real scontb(int direction) const
	{
		return _scontb_sig(direction > 1) * _scontb_sig(type) * (n % 2);
	}
	
	bool is_flow_into_node(const int direction, const real velocity) const
	{
		return (direction < 2) ^ (velocity >= 0);
	}
	
	bool type_fluid_into_node(int direction) const
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
	
	
};

std::ifstream& operator>> (std::ifstream& fin, Cmns& val)
{
	fin >> val.n >> val.type >> val.pos.front() >> val.pos.back();
	return fin;
}

std::ofstream& operator<< (std::ofstream& fout, const Cmns& val)
{
	fout << '\n' << val.n << ' ' << val.type << ' ' << val.pos.front() << ' ' << val.pos.back();
	return fout;
}

struct Coordinate
{
	real x;
	real y;
};

template <class T>
class FTable
{
public:
	int nrows;
	int ncols;
	std::vector<std::vector<T>> v;
	
	FTable() = default;
	FTable(int nrows, int ncols, const T& val = T()): nrows(nrows), ncols(ncols), v(nrows, std::vector<T>(ncols, val)) {}
	
	bool read(const std::string& file_name)
	{
		std::ifstream fin(file_name);
		if(!(fin >> nrows >> ncols))
		{
			std::cout << "-ER2-" << file_name << " is corrupted!" << '\n';
			return false;
		}
		
		std::vector<T> w;
		T val;
		
		while(fin >> val)
		{
			w.push_back(val);
		}
		
		if(nrows * ncols != w.size())
		{
			std::cout << "-ER2-" << file_name << " has incorrect diamensions." << '\n';
			return false;
		}
		
		v.resize(nrows, std::vector<T>(ncols));
		for(int i = 0; i < w.size(); ++ i)
		{
			v[i / ncols][i % ncols] = w[i];
		}
		
		return true;
	}
	
	Coordinate _coordinate (int row, int col) const
	{
		return {0.5f + col, -0.5f + nrows - row};
	}
	
	void write(const std::string& file_name) const 
	{
		std::ofstream fout(file_name);
		fout << nrows << ' ' << ncols << "\n\n";
		for(const auto& row: v)
		{
			for(const auto& val: row)
			{
				fout << val << ' ';
			}
			
			fout << '\n';
		}
	}
	
	bool between(real x, real a, real b) const
	{
		return x >= a && x <= b;
	}
	
	bool inside(real x1, real x2, real y1, real y2, const Coordinate& coordinate) const
	{
		return between(coordinate.x, x1, x2) && between(coordinate.y, y1, y2);
	}
	
	void update(real x1, real x2, real y1, real y2, const T& val)
	{
		real xmin = std::min(x1, x2);
		real xmax = std::max(x1, x2);
		
		real ymin = std::min(y1, y2);
		real ymax = std::max(y1, y2);
		for(int i = 0; i < nrows; ++ i)
		{
			for(int j = 0; j < ncols; ++ j)
			{
				if(inside(xmin, xmax, ymin, ymax, _coordinate(i, j)))
				{
					v[i][j] = val;
				}
			}
		}
	}
	
	void print() const
	{
		for(const auto& row: v)
		{
			for(const auto& val: row)
			{
				std::cout << val << ' ';
			}
			std::cout << '\n';
		}
	}
};

typedef FTable<Cradius> FTradius;
typedef FTable<Cmns> FTmns;

bool FCheckValidity()
{
	FTradius radius;
	FTmns mns;
	
	bool validity = true;
	if(radius.read(FILE_NAME_RADIUS))
	{
		std::cout << "-FDK-" << FILE_NAME_RADIUS << " is valid" << '\n';
	}
	else
	{
		validity = false;
	}
	
	if(mns.read(FILE_NAME_MNS))
	{
		std::cout << "-FDK-" << FILE_NAME_MNS << " is valid, " << '\n';
	}
	else
	{
		validity = false;
	}
	
	if(validity)
	{
		if((radius.nrows == mns.nrows) && (radius.ncols == mns.ncols))
		{
			std::cout << "-FDK-" << "diamensions of " << FILE_NAME_RADIUS << " and " << FILE_NAME_MNS << " match" << '\n';
		}
		else
		{
			std::cout << "-ERR-" << "diamensions of " << FILE_NAME_RADIUS << " and " << FILE_NAME_MNS << " do not match!" << '\n';
			validity = false;
		}
	}
	
	return validity;
}

void FPrintValidityStatus()
{
	if(FCheckValidity())
	{
		std::cout << "-FDK-" << FILE_NAME_RADIUS << ", " << FILE_NAME_MNS << " is okay" << '\n';
	}
	else
	{
		std::cout << "-ERR-" << std::string(30, '!') << '\n';
	}
}		
	
Treal FReadFileRadius()
{
	FTradius radius;
	radius.read(FILE_NAME_RADIUS);
	return radius.v;
}

Tmns FReadFileFill()
{
	FTmns mns;
	mns.read(FILE_NAME_MNS);
	return mns.v;
}



void FPlot(Tmns mns, Tradius radius, real clock, int count)
{
	std::reverse(mns.begin(), mns.end());
	std::reverse(radius.begin(), radius.end());
	
	real max_radius = -1;
	real min_radius = 1e12;
	
	for(const auto& x: radius)
	{
		for(auto y: x)
		{
			max_radius = std::max(max_radius, y);
			min_radius = std::min(min_radius, y);
		}
	}

	const int image_size = IMAGE_SIZE;
	const int length = mns.front().size();
	const int height = mns.size();
	
	const int effective_length = image_size / (std::max(length, height) + 2);
	
	drw::bmp a(image_size, image_size);
	
	const int start_y = effective_length;
	const int start_x = effective_length;
	const real max_thick = effective_length / 3.0;
	const real min_thick = effective_length / 6.0;
	
	
	int y = start_y;
	for(int row = 0; row < mns.size(); ++ row)
	{
		const auto& w = mns[row];
		int x = start_x + effective_length * (row % 2);
		for(int col = 0; col < w.size(); ++ col)
		{
			int sign = (1 - 2 * (row % 2)) * (1 - 2 * (col % 2));
			const real r = radius[row][col];
			real thick = min_thick;
			if(max_radius != min_radius)
			{
				thick  += (r - min_radius) * (max_thick - min_thick) / (max_radius - min_radius);
			}
			a.drawVector(x, y, effective_length, thick, sign, w[col].n, w[col].pos, w[col].type);
			x += 2 * effective_length * (sign > 0);
		}
		
		y += effective_length;
	}
	
	a.save(FOLDER_SAVE_PIC + "pic-" + std::to_string(count) + "_t-" + std::to_string(clock) + ".bmp");
}

int main()
{
	FPrintValidityStatus();
	const auto radius = FReadFileRadius();
	auto mns = FReadFileFill();
	
	FPlot(mns, radius, 1.34, 1000);
	return 0;
}
