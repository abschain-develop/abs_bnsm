
#include "curve.h"
#include "init.h"
#include "fp.h"
#include "fp2.h"
#include "fp6.h"
#include "fp12.h"
#include "points.h"

void init_globals()
{
	fp_init();
	fp2_init();
	fp6_init();
	fp12_init();
	curve_init();
	points_init();
}

void clear_globals()
{
	points_clear();
	curve_clear();
	fp12_clear();
	fp6_clear();
	fp2_clear();
	fp_clear();
}
