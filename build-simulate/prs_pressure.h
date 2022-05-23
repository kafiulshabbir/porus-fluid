#ifndef DEF_PRS_PRESSURE
#define DEF_PRS_PRESSURE

#include "dec.h"


#include "dat_boundary.h"
#include "dat_radius.h"
#include "dat_fill.h"
#include "stc_node.h"

namespace prs
{
	//Generate a equation of size n * (n + 1)
	
	std::vector<double> GaussElimination(matrix v);
	matrix CalculatePressureDistribution(const dat::cls_boundary&, const dat::cls_fill&, const dat::cls_radius&);
}

#endif
