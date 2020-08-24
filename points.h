

#ifndef POINTS_H
#define POINTS_H

#include "curvepoint_fp.h"
#include "twistpoint_fp2.h"

extern curvepoint_fp_t curve_gen; // generator of E(\F_p)
extern twistpoint_fp2_t twist_gen; // generator of the subgroup of order n of E'(\F_{p^2})

void points_init(void);

void points_clear(void);

#endif // ifdef POINTS_H
