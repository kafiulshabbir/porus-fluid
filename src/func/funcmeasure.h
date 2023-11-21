#ifndef FUNCMEASURE_H
#define FUNCMEASURE_H

#include "decl/decltypedef.h"
#include "dst/dstdiamension.h"

namespace func
{	
	class Measure
	{
		static bool is_inside(
			const int val_min,
			const int val,
			const int val_max
		);
		
	public:
	
		class FluidPpr
		{
			const double ctime;
			double xvol_inner;
			double xvol_outer;
			double xvol_blue_inner;
			double xvol_blue_outer;
			
			double xcap_pressure_inner;
			double xcap_pressure_outer;
			double xarea_inner;
			double xarea_outer;
			
		public:
			
			FluidPpr(const double time = -1);
			
			void add_blue(const double rad, const double blue_ppr, bool is_inner);
			void add_capillary_pressure_data(const double sigma, const double rad, bool is_inner);
			
			double time() const;
			double vol_tube_outer() const;
			double vol_tube_inner() const;
			double vol_tube_total() const;
			
			double vol_blue_inner() const;
			double vol_blue_outer() const;
			double vol_blue_total() const;
			
			double vol_grey_inner() const;
			double vol_grey_outer() const;
			double vol_grey_total() const;
			
			double ppr_blue_inner() const;
			double ppr_blue_outer() const;
			double ppr_blue_total() const;
			
			double ppr_grey_inner() const;
			double ppr_grey_outer() const;
			double ppr_grey_total() const;
			
			double average_capillary_pressure_inside() const;
			double average_capillary_pressure_outside() const;
			
			static std::vector<std::string> header();
			std::vector<double> val_vec() const;
		};
		
		static FluidPpr fluid_ppr(
			const Tdouble& radius,
			const TMns& mnsc,
			const double time,
			const int row_b,
			const int col_b,
			const int row_e,
			const int col_e,
			const dst::Diamension& dm
		);
			
		static double measure_wetting_fluid_proportion(const Tdouble& radius, const TMns& mnsc);
	};
}

#endif
