

#include "points.h"
#include "fp2e.h"
#include "curvepoint_fp.h"
#include "twistpoint_fp2.h"
#include "parameters.h"

curvepoint_fp_t curve_gen; // generator of E(\F_p)
twistpoint_fp2_t twist_gen; // generator of the subgroup of order n of E'(\F_{p^2})

void points_init()
{
	fp2e_t twistgen_x;
	fp2e_t twistgen_y;
	fp2e_set_str(twistgen_x, BN_TWISTGEN_X);
	fp2e_set_str(twistgen_y, BN_TWISTGEN_Y);
	curvepoint_fp_set_str(curve_gen, BN_CURVEGEN);
	twistpoint_fp2_affineset_fp2e(twist_gen, twistgen_x, twistgen_y);
}

void points_clear()
{
	;
}
