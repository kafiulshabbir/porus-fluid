#include "funcmeasure.h"

double func::Measure::measure_wetting_fluid_proportion(const Tdouble& radius, const TMns& mnsc)
{
	const int n = radius.size();
	const int m = radius.front().size();
	double total = 0;
	double type_first = 0;
	for(int i = 0; i < n; ++ i)
	{
		for(int j = 0; j < m; ++ j)
		{
			const double rsq = std::pow(radius[i][j], 2);
			type_first += mnsc[i][j].sum_type_first() * rsq;
			total += rsq;
		}
	}

	return type_first / total;
}

func::Measure::FluidPpr func::Measure::fluid_ppr(
	const Tdouble& radius,
	const TMns& mnsc,
	const double time,
	const int row_b,
	const int col_b,
	const int row_e,
	const int col_e,
	const dst::Diamension& dm
)
{
	FluidPpr fluidp(time);

	const int rmin = std::min(row_b, row_e);
	const int rmax = std::max(row_b, row_e);
	const int cmin = std::min(col_b, col_e);
	const int cmax = std::max(col_b, col_e);

	for(int row = 0; row < dm.rows; ++ row)
	{
		for(int col = 0; col < dm.cols; ++ col)
		{
			const double rad = radius[row][col];
			const double blue_ppr = mnsc[row][col].sum_type_first();

			const bool inside
				= is_inside(rmin, row, rmax)
				&& is_inside(cmin, col, cmax);

			fluidp.add_blue(rad, blue_ppr, inside);

			if(mnsc[row][col].n % 2 == 0)
			{
				continue;
			}

			fluidp.add_capillary_pressure_data(declconst::SIGMA, rad, inside);
		}
	}

	return fluidp;
}

func::Measure::FluidPpr::FluidPpr(const double time):
	ctime(time),
	xvol_inner(0),
	xvol_outer(0),
	xvol_blue_inner(0),
	xvol_blue_outer(0),
	xcap_pressure_inner(0),
	xcap_pressure_outer(0),
	xarea_inner(0),
	xarea_outer(0)
{}


void func::Measure::FluidPpr::add_blue(const double rad, const double ppr_blue, bool is_inner)
{
	const double area = std::pow(rad, 2.0);
	const double vol = area * 1.0;

	if(is_inner)
	{
		xvol_inner += vol;
		xvol_blue_inner += vol * ppr_blue;
	}
	else
	{
		xvol_outer += vol;
		xvol_blue_outer += vol * ppr_blue;
	}
}

void func::Measure::FluidPpr::add_capillary_pressure_data(const double sigma, const double rad, bool is_inner)
{
	const double area = std::pow(rad, 2.0);
	const double capillary_pressure_local = 2.0 * sigma * rad;
	if(is_inner)
	{
		xcap_pressure_inner += capillary_pressure_local;
		xarea_inner += area;
	}
	else
	{
		xcap_pressure_outer += capillary_pressure_local;
		xarea_outer += area;
	}
}



double func::Measure::FluidPpr::time() const
{
	return ctime;
}

double func::Measure::FluidPpr::vol_tube_outer() const
{
	return xvol_outer;
}

double func::Measure::FluidPpr::vol_tube_inner() const
{
	return xvol_inner;
}

double func::Measure::FluidPpr::vol_tube_total() const
{
	return vol_tube_inner() + vol_tube_outer();
}

double func::Measure::FluidPpr::vol_blue_inner() const
{
	return xvol_blue_inner;
}

double func::Measure::FluidPpr::vol_blue_outer() const
{
	return xvol_blue_outer;
}

double func::Measure::FluidPpr::vol_blue_total() const
{
	return vol_blue_inner() + vol_blue_outer();
}

double func::Measure::FluidPpr::vol_grey_inner() const
{
	return vol_tube_inner() - vol_blue_inner();
}

double func::Measure::FluidPpr::vol_grey_outer() const
{
	return vol_tube_outer() - vol_blue_outer();
}

double func::Measure::FluidPpr::vol_grey_total() const
{
	return vol_grey_inner() + vol_grey_outer();
}

double func::Measure::FluidPpr::ppr_blue_inner() const
{
	return vol_blue_inner() / vol_tube_inner();
}

double func::Measure::FluidPpr::ppr_blue_outer() const
{
	return vol_blue_outer() / vol_tube_outer();
}

double func::Measure::FluidPpr::ppr_blue_total() const
{
	return vol_blue_total() / vol_tube_total();
}

double func::Measure::FluidPpr::ppr_grey_inner() const
{
	return vol_grey_inner() / vol_tube_inner();
}

double func::Measure::FluidPpr::ppr_grey_outer() const
{
	return vol_grey_outer() / vol_tube_outer();
}

double func::Measure::FluidPpr::ppr_grey_total() const
{
	return vol_grey_total() / vol_tube_total();
}

double func::Measure::FluidPpr::average_capillary_pressure_inside() const
{
	return xcap_pressure_inner / xarea_inner;
}

double func::Measure::FluidPpr::average_capillary_pressure_outside() const
{
	return xcap_pressure_outer / xarea_outer;
}

bool func::Measure::is_inside(
	const int val_min,
	const int val,
	const int val_max
)
{
	return (val >= val_min) && (val <= val_max);
}

std::vector<std::string> func::Measure::FluidPpr::header()
{
	return std::vector<std::string>
	{
		"time",

		"vol_tube_outer",
		"vol_tube_inner",
		"vol_tube_total",

		"vol_blue_inner",
		"vol_blue_outer",
		"vol_blue_total",

		"vol_grey_inner",
		"vol_grey_outer",
		"vol_grey_total",

		"ppr_blue_inner",
		"ppr_blue_outer",
		"ppr_blue_total",

		"ppr_grey_inner",
		"ppr_grey_outer",
		"ppr_grey_total",

		"average_capillary_pressure_inside",
		"average_capillary_pressure_outside"
	};
}
std::vector<double> func::Measure::FluidPpr::val_vec() const
{
	return std::vector<double>
	{
		time(),
		vol_tube_outer(),
		vol_tube_inner(),
		vol_tube_total(),

		vol_blue_inner(),
		vol_blue_outer(),
		vol_blue_total(),

		vol_grey_inner(),
		vol_grey_outer(),
		vol_grey_total(),

		ppr_blue_inner(),
		ppr_blue_outer(),
		ppr_blue_total(),

		ppr_grey_inner(),
		ppr_grey_outer(),
		ppr_grey_total(),
		average_capillary_pressure_inside(),
		average_capillary_pressure_outside()
	};
}
