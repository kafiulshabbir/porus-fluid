#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <utility>
#include <limits>

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
const real THRESHOLD_FILL = 0.01; //if any meniscus is smaller than this proportion, then it is destroyed
const real TIME_DIV = 10; // if the nearest meniscus by time is further, then L / TIME_DIV is prefered
const int IMAGE_SIZE = 1000;

//Input Output of File names
const std::string FILE_NAME_RADIUS = "radius.txt";
const std::string FILE_NAME_FILL = "fill.txt";
const std::string FOLDER_SAVE_PIC = "pic/";

class Cmns
{
	int n; //number of meniscus present
	bool type; // 0 - corresponds to blue fluid - which is invading
	std::vector<real> pos; // positions of meniscus
	
	/*
	 * FUNCTION DESCRIPTION - scontb
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
	
	real scontb(int direction) const
	{
		return _scontb_sig(direction > 1) * _scontb_sig(type) * (n % 2);
	}
	
	/* 
	 * generate positions long version
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
		
	real mu(const real mu1, const real mu2) const
	{
		std::vector<real> muv{mu1, mu2};
		const auto pos_long = gen_pos_long();
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
		dspl = std::min(1.0 / time_div, dspl);
		return length * dspl / velocity;
	}
	
	bool fluid_is_out(int direction, real velocity) const
	{
		return (direction < 2) ^ (velocity >= 0);
	}
	
	bool type_fluid_in(int direction) const
	{
		if(direction < 2)
		{
			return type;
		}
		
		return type ^ (n % 2);
	}
	
	std::vector<real> _pos_sequence_disp(real vel, real l) const
	{
		real begin = 0;
		real end = 1;
		if(vel > 0)
		{
			begin = l;
		}
		else
		{
			end -= l;
		}
		
		std::vector<real> v(n + 2);
		v.front() = begin;
		for(int i = 0; i < n; ++ i)
		{
			v[i + 1] = pos[i];
		}
		v.back() = end;
		
		return v;
	}
	
	std::list<std::pair<bool, real>> _gen_vol_comp_old_config(real vel, real l) const
	{
		const auto pos_seq_disp = _pos_sequence_disp(vel, l);
		std::list<std::pair<bool, real>> lst;
		for(int i = 1; i < pos_seq_disp.size(); ++ i)
		{
			lst.push_back({(i + type + 1) % 2, pos_seq_disp[i] - pos_seq_disp[i - 1]});
		}
		
		return lst;
	}
	
	static std::list<std::pair<bool, real>> _merge(std::list<std::pair<bool, real>>& old_config, const std::list<std::pair<bool, real>>& new_addents, real vel)
	{
		if(vel > 0)
		{
			old_config.insert(old_config.begin(), new_addents.crbegin(), new_addents.crend());
		}
		else
		{
			old_config.insert(old_config.end(), new_addents.begin(), new_addents.end());
		}
		return old_config;
	}
	struct _ProcessNewComp
	{
		bool type;
		std::vector<real> v;
	};
	
	_ProcessNewComp _decomp(const std::vector<std::pair<bool, real>>& v)
	{
		int n = v.size() - 1;
		real sum = 0;
		std::vector<real> w;
		for(int i = 0; i < n; ++ i)
		{
			sum += v[i].second;
			w.push_back(sum);
		}
		
		return {v.front().first, w};
	}
	
	_ProcessNewComp _process_new_comp(const std::list<std::pair<bool, real>>& lst)
	{
		std::vector<std::pair<int, real>> v;
		for(const auto& x: lst)
		{
			if(x.second >= 0.001)
			{
				v.push_back({x.first, x.second});
			}
		}
		
		for(int i = 1; i < v.size(); ++ i)
		{
			if(v[i - 1].first == v[i].first)
			{
				v[i - 1].first = -1;
				v[i].second += v[i - 1].second;
			}
		}
		
		std::vector<std::pair<bool, real>> w;
		for(const auto& x: v)
		{
			if(x.first != -1)
			{
				w.push_back({x.first, x.second});
			}
		}
		
		if(w.size() < 4)
		{
			return _decomp(w);
		}
		
		if(w.size() == 4)
		{
			auto x = _decomp(w);
			real l1 = 0;
			real l2 = x.v[0];
			real l3 = x.v[1];
			real l4 = x.v[2];
			
			real d1 = l2 - l1;
			real d2 = l4 - l3;
			real d = d1 + d2;
			real c1 = (l1 + l2) / 2;
			real c2 = (l3 + l4) / 2;
			
			real L1 = (c1 * d1 + c2 * d2) / d - d / 2;
			real L2 = L1 + d;
			
			return {!x.type, {L1, L2}};
		}
		if(w.size() == 5)
		{
			auto x = _decomp(w);
			real l1 = x.v[0];
			real l2 = x.v[1];
			real l3 = x.v[2];
			real l4 = x.v[3];
			
			real d1 = l2 - l1;
			real d2 = l4 - l3;
			real d = d1 + d2;
			real c1 = (l1 + l2) / 2;
			real c2 = (l3 + l4) / 2;
			
			real L1 = (c1 * d1 + c2 * d2) / d - d / 2;
			real L2 = L1 + d;
			
			return {x.type, {L1, L2}};
		}
		else
		{
			std::cout << "ER3-oversized decompartalization" << std::endl;
		}
	}
	
	
	void update(real vel, real r, const std::vector<real>& add)
	{
		const real area = PI * std::pow(r, 2);
		const real l1 = add.front() / area;
		const real l2 = add.back() / area;
		const real l = l1 + l2;
		
		auto old_config = _gen_vol_comp_old_config(vel, l);
		auto new_comp = _merge(old_config, {{0, l1}, {1, l2}}, vel);
		auto comp = _process_new_comp(new_comp);
		n = comp.v.size();
		type = comp.type;
		pos = comp.v;
		pos.resize(2);
	}
	
public:
	Cmns(): n(0), type(1), pos(2) {} //by default everything is the defending fluid
	Cmns(int n, bool type, real p1, real p2): n(n), type(type), pos{p1, p2} {}
};

std::ifstream& operator>> (std::ifstream& fin, FillProperty& val)
{
	fin >> val.n >> val.type >> val.pos.front() >> val.pos.back();
	return fin;
}

std::ofstream& operator<< (std::ofstream& fout, const FillProperty& val)
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
struct Tube
{
	int nrows;
	int ncols;
	std::vector<std::vector<T>> v;
	
	Tube() = default;
	Tube(int nrows, int ncols, const T& val = T()): nrows(nrows), ncols(ncols), v(nrows, std::vector<T>(ncols, val)) {}
	
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

typedef Tube<real> Radius;
typedef Tube<FillProperty> Fill;

bool FCheckValidity()
{
	Radius radius;
	Fill fill;
	
	bool validity = true;
	if(radius.read(FILE_NAME_RADIUS))
	{
		std::cout << "-FDK-" << FILE_NAME_RADIUS << " is valid" << '\n';
	}
	else
	{
		validity = false;
	}
	
	if(fill.read(FILE_NAME_FILL))
	{
		std::cout << "-FDK-" << FILE_NAME_FILL << " is valid, " << '\n';
	}
	else
	{
		validity = false;
	}
	
	if(validity)
	{
		if((radius.nrows == fill.nrows) && (radius.ncols == fill.ncols))
		{
			std::cout << "-FDK-" << "diamensions of " << FILE_NAME_RADIUS << " and " << FILE_NAME_FILL << " match" << '\n';
		}
		else
		{
			std::cout << "-ERR-" << "diamensions of " << FILE_NAME_RADIUS << " and " << FILE_NAME_FILL << " do not match!" << '\n';
			validity = false;
		}
	}
	
	return validity;
}

void FPrintValidityStatus()
{
	if(FCheckValidity())
	{
		std::cout << "-FDK-" << FILE_NAME_RADIUS << ", " << FILE_NAME_FILL << " is okay" << '\n';
	}
	else
	{
		std::cout << "-ERR-" << std::string(30, '!') << '\n';
	}
}	
			real sum = 0;
			for(int i = 1; i < locationsv.size(); ++ i)
			{
				sum += muv[(i - 1 + type) % 2] * (locationsv[i] - locationsv[i - 1]);
			}
			
			return sum;
		}
		
		real time(real velocity, real length)
		{
			if(n == 0)
			{
				return length / velocity / 10;
			}
			
			real dspl = (velocity >= 0 ? (1 - pos[n - 1]): pos.front());
			dspl = std::min(0.1f, dspl);
			return length * dspl / velocity;
		}
		
		bool fluid_is_out(int direction, real velocity) const
		{
			return (direction < 2) ^ (velocity >= 0);
		}
		
		bool type_fluid_in(int direction) const
		{
			if(direction < 2)
			{
				return type;
			}
			
			return type ^ (n % 2);
		}
		
		std::vector<real> _pos_sequence_disp(real vel, real l) const
		{
			real begin = 0;
			real end = 1;
			if(vel > 0)
			{
				begin = l;
			}
			else
			{
				end -= l;
			}
			
			std::vector<real> v(n + 2);
			v.front() = begin;
			for(int i = 0; i < n; ++ i)
			{
				v[i + 1] = pos[i];
			}
			v.back() = end;
			
			return v;
		}
		
		std::list<std::pair<bool, real>> _gen_vol_comp_old_config(real vel, real l) const
		{
			const auto pos_seq_disp = _pos_sequence_disp(vel, l);
			std::list<std::pair<bool, real>> lst;
			for(int i = 1; i < pos_seq_disp.size(); ++ i)
			{
				lst.push_back({(i + type + 1) % 2, pos_seq_disp[i] - pos_seq_disp[i - 1]});
			}
			
			return lst;
		}
		
		static std::list<std::pair<bool, real>> _merge(std::list<std::pair<bool, real>>& old_config, const std::list<std::pair<bool, real>>& new_addents, real vel)
		{
			if(vel > 0)
			{
				old_config.insert(old_config.begin(), new_addents.crbegin(), new_addents.crend());
			}
			else
			{
				old_config.insert(old_config.end(), new_addents.begin(), new_addents.end());
			}
			return old_config;
		}
		struct _ProcessNewComp
		{
			bool type;
			std::vector<real> v;
		};
		
		_ProcessNewComp _decomp(const std::vector<std::pair<bool, real>>& v)
		{
			int n = v.size() - 1;
			real sum = 0;
			std::vector<real> w;
			for(int i = 0; i < n; ++ i)
			{
				sum += v[i].second;
				w.push_back(sum);
			}
			
			return {v.front().first, w};
		}
		
		_ProcessNewComp _process_new_comp(const std::list<std::pair<bool, real>>& lst)
		{
			std::vector<std::pair<int, real>> v;
			for(const auto& x: lst)
			{
				if(x.second >= 0.001)
				{
					v.push_back({x.first, x.second});
				}
			}
			
			for(int i = 1; i < v.size(); ++ i)
			{
				if(v[i - 1].first == v[i].first)
				{
					v[i - 1].first = -1;
					v[i].second += v[i - 1].second;
				}
			}
			
			std::vector<std::pair<bool, real>> w;
			for(const auto& x: v)
			{
				if(x.first != -1)
				{
					w.push_back({x.first, x.second});
				}
			}
			
			if(w.size() < 4)
			{
				return _decomp(w);
			}
			
			if(w.size() == 4)
			{
				auto x = _decomp(w);
				real l1 = 0;
				real l2 = x.v[0];
				real l3 = x.v[1];
				real l4 = x.v[2];
				
				real d1 = l2 - l1;
				real d2 = l4 - l3;
				real d = d1 + d2;
				real c1 = (l1 + l2) / 2;
				real c2 = (l3 + l4) / 2;
				
				real L1 = (c1 * d1 + c2 * d2) / d - d / 2;
				real L2 = L1 + d;
				
				return {!x.type, {L1, L2}};
			}
			if(w.size() == 5)
			{
				auto x = _decomp(w);
				real l1 = x.v[0];
				real l2 = x.v[1];
				real l3 = x.v[2];
				real l4 = x.v[3];
				
				real d1 = l2 - l1;
				real d2 = l4 - l3;
				real d = d1 + d2;
				real c1 = (l1 + l2) / 2;
				real c2 = (l3 + l4) / 2;
				
				real L1 = (c1 * d1 + c2 * d2) / d - d / 2;
				real L2 = L1 + d;
				
				return {x.type, {L1, L2}};
			}
			else
			{
				std::cout << "ER3-oversized decompartalization" << std::endl;
			}
		}
		
		
		void update(real vel, real r, const std::vector<real>& add)
		{
			const real area = PI * std::pow(r, 2);
			const real l1 = add.front() / area;
			const real l2 = add.back() / area;
			const real l = l1 + l2;
			
			auto old_config = _gen_vol_comp_old_config(vel, l);
			auto new_comp = _merge(old_config, {{0, l1}, {1, l2}}, vel);
			auto comp = _process_new_comp(new_comp);
			n = comp.v.size();
			type = comp.type;
			pos = comp.v;
			pos.resize(2);
		}
	};

	std::ifstream& operator>> (std::ifstream& fin, FillProperty& val)
	{
		fin >> val.n >> val.type >> val.pos.front() >> val.pos.back();
		return fin;
	}

	std::ofstream& operator<< (std::ofstream& fout, const FillProperty& val)
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
	struct Tube
	{
		int nrows;
		int ncols;
		std::vector<std::vector<T>> v;
		
		Tube() = default;
		Tube(int nrows, int ncols, const T& val = T()): nrows(nrows), ncols(ncols), v(nrows, std::vector<T>(ncols, val)) {}
		
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

	typedef Tube<real> Radius;
	typedef Tube<FillProperty> Fill;
	
	bool FCheckValidity()
	{
		Radius radius;
		Fill fill;
		
		bool validity = true;
		if(radius.read(FILE_NAME_RADIUS))
		{
			std::cout << "-FDK-" << FILE_NAME_RADIUS << " is valid" << '\n';
		}
		else
		{
			validity = false;
		}
		
		if(fill.read(FILE_NAME_FILL))
		{
			std::cout << "-FDK-" << FILE_NAME_FILL << " is valid, " << '\n';
		}
		else
		{
			validity = false;
		}
		
		if(validity)
		{
			if((radius.nrows == fill.nrows) && (radius.ncols == fill.ncols))
			{
				std::cout << "-FDK-" << "diamensions of " << FILE_NAME_RADIUS << " and " << FILE_NAME_FILL << " match" << '\n';
			}
			else
			{
				std::cout << "-ERR-" << "diamensions of " << FILE_NAME_RADIUS << " and " << FILE_NAME_FILL << " do not match!" << '\n';
				validity = false;
			}
		}
		
		return validity;
	}

	void FPrintValidityStatus()
	{
		if(FCheckValidity())
		{
			std::cout << "-FDK-" << FILE_NAME_RADIUS << ", " << FILE_NAME_FILL << " is okay" << '\n';
		}
		else
		{
			std::cout << "-ERR-" << std::string(30, '!') << '\n';
		}
	}
	
	
matrix FReadFileRadius()
{
	dst::Radius radius;
	radius.read(dst::FILE_NAME_RADIUS);
	return radius.v;
}

Fill FReadFileFill()
{
	dst::Fill fill;
	fill.read(dst::FILE_NAME_FILL);
	return fill.v;
}

int FLinearLocNode(int i, int j, int m)
{
	return (i * (m + 1) + (i % 2)) / 2 + j;
}

struct Connections
{
	bool a = true;
	int r;
	int c;
	int p;	
};

std::vector<Connections> FGenConnectionsEqu(int r, int c, int n, int m)
{
	const auto p = FLinearLocNode(r, c, m);
	std::vector<Connections> v
	{
		{true, r - 1, 2 * c - 1 + r % 2, p - m / 2 - 1},
		{true, r - 1, 2 * c - 0 + r % 2, p - m / 2 - 0},
		{true, r - 0, 2 * c - 1 + r % 2, p + m / 2 + 1},
		{true, r - 0, 2 * c - 0 + r % 2, p + m / 2 + 0}
	};
	
	if(r % 2)
	{
		return v;
	}
	
	if(r == 0)
	{
		v[0].a = false;
		v[1].a = false;
	}
	if(c == 0)
	{
		v[0].a = false;
		v[3].a = false;
	}
	if(c + 1 == m)
	{
		v[1].a = false;
		v[2].a = false;
	}
	if(r + 1 == n)
	{
		v[2].a = false;
		v[3].a = false;
	}
	
	return v;
}

std::vector<real> FGaussElimination(matrix M)
{
	const int n = M.front().size() - 1;
	for(int i = 0; i < n; ++ i)
	{
		real divider = M[i][i];
		for(int j = 0; j <= n; ++ j)
		{
			M[i][j] /= divider;
		}
		
		for(int j = 0; j < n; ++ j)
		{
			if(i == j)
			{
				continue;
			}
			
			real coeff = M[j][i];
			
			for(int k = 0; k <= n; ++ k)
			{
				M[j][k] -= M[i][k] * coeff;
			}
		}
	}
	std::vector<real> v;

	for(auto& row: M)
	{
		v.push_back(row.back());
	}
	
	return v;
}

int FTotalNodes(int n, int m)
{
	return ((n + 1) * (m + 1) + 1) / 2;
}

matrix FGenEquForGauss(const matrix& radius, const Fill& fill)
{
	const int n = radius.size();
	const int m = radius.front().size();
	const int total_nodes = FTotalNodes(n, m);
	matrix equation(total_nodes, std::vector<real>(total_nodes + 1));
	
	for(int i = 0; i <= n; ++ i)
	{
		int mt = m / 2 - (i % 2);
		for(int j = 0; j <= mt; ++ j)
		{
			const int l = FLinearLocNode(i, j, m);
			auto& e = equation[l];
			if(i == 0)
			{
				e[l] = 1;
				e.back() = PRESSURE_TOP;
				continue;
			}
			if(i == n)
			{
				e[l] = 1;
				e.back() = PRESSURE_BOTTOM;
				continue;
			}
			
			//derection: 0-topleft, 1-topright, 2-bottomright, 3-bottomleft
			const auto connections = FGenConnectionsEqu(i, j, n, m);
			
			for(int i = 0; i < connections.size(); ++ i)
			{
				const auto& c = connections[i];
				if(c.a)
				{
					const auto& r = radius[c.r][c.c];
					const auto& f = fill[c.r][c.c];
					const auto& s = f.scontb(i);
					
					const real K = std::pow(r, 3) / f.mu(MU1, MU2);
					e[l] += r * K;
					e[c.p] -= r * K;
					e.back() -= SIGMA * 2 * s * K;
				}
			}
		}
	}
	
	return equation;
}
	
std::vector<real> FCalcPressure(const matrix& radius, const Fill& fill)
{
	return FGaussElimination(FGenEquForGauss(radius, fill));
}

std::pair<int, int> FConnectionEnds(int r, int c, int m)
{
	return {FLinearLocNode(r, c/2 + 1 - (r % 2), m), FLinearLocNode(r + 1, c/2 + (r % 2), m)};
}

matrix FCalcVelocity(std::vector<real>& pressure, const matrix& radius, const Fill& fill)
{
	const int n = radius.size();
	const int m = radius.front().size();
	auto velocity = radius;
	for(int i = 0; i < n; ++ i)
	{
		for(int j = 0; j < m; ++ j)
		{
			const auto locs = FConnectionEnds(i, j, m);
			const auto delp = pressure[locs.second] - pressure[locs.first];
			const auto& r = radius[i][j];
			const auto& mu = fill[i][j].mu(MU1, MU2);
			const auto& s = fill[i][j].scontb(0);
			velocity[i][j] = r / 8 / mu / TUBE_LENGTH * (delp * r + s * 2 * SIGMA);
		}
	}
	
	return velocity;
}
				
matrix FCalcVolume(matrix velocity, const matrix& radius, const real time_step)
{
	for(int i = 0; i < velocity.size(); ++ i)
	{
		auto& v = velocity[i];
		for(int j = 0; j < v.size(); ++ j)
		{
			v[j] = std::abs(v[j] * std::pow(radius[i][j], 2) * PI * time_step);
		}
	}
	
	return velocity;
}

real FDetermineTimeStep(const Fill& fill, const matrix& velocity)
{
	real min_time = HUGE;
	for(int i = 0; i < fill.size(); ++ i)
	{
		for(int j = 0; j < fill[i].size(); ++ j)
		{
			const real temp_time = fill[i][j].time(velocity[i][j], TUBE_LENGTH);
			min_time = std::min(temp_time, min_time);
		}
	}
	
	return min_time;
}

struct IntegrationResult
{
	Fill fill;
	real fluid1in;
	real fluid1out;
	real fluid2in;
	real fluid2out;
};

int FCountConnections(const std::vector<Connections>& connections)
{
	int count = 0;
	
	for(const auto& connection: connections)
	{
		count += connection.a;
	}
	
	return count;
}

struct TubeWhereFlowOut
{
	real radius;
	int r;
	int c;
};

bool Fcomparison_outflow(const TubeWhereFlowOut& first, const TubeWhereFlowOut& second)
{
	return first.radius < second.radius;
}

std::vector<real> FAmountVolumeToBePushedIn(real volume, std::vector<real>& tank)
{
	auto v = tank;
	v.front() = std::min(tank.front(), volume);
	v.back() = volume - v.front();
	
	for(int i = 0; i < tank.size(); ++ i)
	{
		tank[i] -= v[i];
	}
	
	return v;
}
	

Fill FCombineFillAndAdditions(Fill fill, const matrix& velocity, const matrix& radius, const std::vector<std::vector<std::vector<real>>>& additions)
{
	for(int i = 0; i < fill.size(); ++ i)
	{
		auto& f = fill[i];
		for(int j = 0; j < f.size(); ++ j)
		{
			f[j].update(velocity[i][j], radius[i][j], additions[i][j]);
		}
	}
	
	return fill;
}

IntegrationResult FPerformIntegration(const Fill& fill, const matrix& volume, const matrix& velocity, const matrix& radius, const real time_step)
{
	const int n = volume.size();
	const int m = volume.front().size();
	
	real fluid1in = 0;
	real fluid1out = 0;
	real fluid2in = 0;
	real fluid2out = 0;
	
	std::vector<std::vector<std::vector<real>>> additions(n, std::vector<std::vector<real>>(m));
	
	for(int i = 0; i <= n; ++ i)
	{
		int mt = m / 2 - i % 2;
		for(int j = 0; j < mt; ++ j)
		{
			const auto connections = FGenConnectionsEqu(i, j, n, m);

			std::vector<real> vol_in(2);
			std::vector<TubeWhereFlowOut> tubes_flow_out;
			for(int direction = 0; direction < connections.size(); ++ direction)
			{
				const auto& c = connections[direction];
				if(c.a)
				{
					const auto& f = fill[c.r][c.c];
					const auto& vel = velocity[c.r][c.c];
					const auto& vol = volume[c.r][c.c];
					const auto& r = radius[c.r][c.c];
					if(f.fluid_is_out(direction, vel))
					{
						tubes_flow_out.push_back({r, c.r, c.c});
					}
					else
					{
						vol_in[f.type_fluid_in(direction)] += vol;
					}
				}
			}
			
			if(FCountConnections(connections) == 1)
			{
				fluid1out += vol_in.front();
				fluid2out += vol_in.back();
				
				if(tubes_flow_out.size())
				{
					vol_in.front() = volume[tubes_flow_out.front().r][tubes_flow_out.front().c];
				}
			}
			
			std::sort(tubes_flow_out.begin(), tubes_flow_out.end(), *Fcomparison_outflow);
			for(const auto& tpshin: tubes_flow_out)
			{
				additions[tpshin.r][tpshin.c] = FAmountVolumeToBePushedIn(volume[tpshin.r][tpshin.c], vol_in);
			}
		}
	}
	
	return FCombineFillAndAdditions(fill, velocity, additions, time_step);
}

void FPlotFill(Fill fill, matrix radius, real clock, int count)
{
	std::reverse(fill.begin(), fill.end());
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
	const int length = fill.front().size();
	const int height = fill.size();
	
	const int effective_length = image_size / (std::max(length, height) + 2);
	
	drw::bmp a(image_size, image_size);
	
	const int start_y = effective_length;
	const int start_x = effective_length;
	const real max_thick = length_vector / 5.0;
	const real min_thick = length_vector / 10.0;
	
	
	int y = start_y;
	for(int row = 0; row < fill.size(); ++ row)
	{
		const auto& w = fill[row];
		int x = start_x + effective_length * (row % 2);
		for(int col = 0; col < w.size(); ++ col)
		{
			int sign = (1 - 2 * (row % 2)) * (1 - 2 * (col % 2));
			const real r = radius[row][col];
			real thick = min_thick;
			if(max_thick != min_thick)
			{
				thick  += (r - min_radius) * (max_thick - min_thick) / (max_radius - min_radius);
			}
			a.drawVector(x, y, effective_length, thick, sign, w[col].n, w[col].pos);
			x += 2 * effective_length * (sign > 0);
		}
		
		y += effective_length;
	}
	
	a.save(FOLDER_SAVE_PIC + "pic-" + std::to_string(count) + "_t-" + std::to_string(time_ran) + ".bmp");
}

void FSimulate(const matrix& radius, Fill& fill)
{
	real clock = 0;
	int count = 10000;
	//while(FLowWettingFluid(fill, C_MAX_WETTING_PROPORTION))
	while(true)
	{
		FPlotFill(fill, clock, count++);
		const auto pressure = FCalcPressure(radius, fill);
		const auto velocity = FCalcVelocity(pressure, radius, fill);
		
		const auto time_step = FDetermineTimeStep(fill, velocity);
		const auto volume = FCalcVolume(velocity, radius, time_step);
		
		fill = FPerformIntegration(fill, volume, velocity, radius, time_step);
		clock += time_step;
	}
}

int main()
{
	dst::FPrintValidityStatus();
	const auto radius = FReadFileRadius();
	auto fill = FReadFileFill();
	
	FSimulate(radius, fill);
	return 0;
}
