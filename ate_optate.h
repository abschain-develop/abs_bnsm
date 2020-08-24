
#ifndef ATE_H
#define ATE_H

#include "curvepoint_fp.h"
#include "twistpoint_fp2.h"
#include "fp12e.h"

void ate(fp12e_t rop, const twistpoint_fp2_t op1, const curvepoint_fp_t op2);

void optate(fp12e_t rop, const twistpoint_fp2_t op1, const curvepoint_fp_t op2);

#endif
