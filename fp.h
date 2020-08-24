

#ifndef FP_H
#define FP_H

#include <gmp.h>

#include "fpe.h"



extern mpz_t sm9_p;
extern mp_limb_t p_inv; // -p^{-1} mod 2^{GMP_LIMB_BITS} used in Montgomery reduction

void fp_init(void);

void fp_clear(void);

#endif // ifdef FP_H
