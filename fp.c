

#include "fp.h"
#include "parameters.h"

mpz_t sm9_p;
mp_limb_t p_inv; // -p^{-1} mod 2^{GMP_LIMB_BITS} used in Montgomery reduction

void fp_init()
{
    mpz_init_set_str(sm9_p, BN_P, 10);

	// Set the value m', used in Montgomery reduction, see HAC, alg. 14.32
#if(GMP_LIMB_BITS == 32)
	p_inv = BN_PINV32;
#elif(GMP_LIMB_BITS == 64)
	p_inv = BN_PINV64;
#else
#error "Only 32 and 64 Bit architectures are supported"
#endif
}

void fp_clear()
{
    mpz_clear(sm9_p);
}
