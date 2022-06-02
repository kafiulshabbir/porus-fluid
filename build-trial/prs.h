#ifndef DEF_PRS
#define DEF_PRS

#include "dec.h"


#include "dat_boundary.h"
#include "dat_radius.h"
#include "dat_fill.h"
#include "stc_node.h"

namespace prs
{
	//Generate a equation of size n * (n + 1)
	double sign_jump(double proportion, int direction);
	vector GaussElimination(matrix v);
	matrix GaussMatrixForPressure(const dat::cls_boundary&, const dat::cls_fill&, const dat::cls_radius&);
}

#endif
