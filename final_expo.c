
#include "final_expo.h"
#include "fp.h"
#include "curve.h"

void final_expo(fp12e_t rop)
{
	// First part: (p^6 - 1)
	fp12e_t dummy1, dummy2, dummy3, fp, fp2, fp3;
	fp12e_set(dummy1, rop);
	
	// This is exactly the p^6-Frobenius action:
	fp6e_neg(rop->m_a, rop->m_a);
	
	fp12e_invert(dummy2, dummy1);
	fp12e_mul(rop, rop, dummy2);

	// Second part: (p^2 + 1)
	fp12e_set(dummy1, rop);
	fp12e_frobenius_p2(rop, rop);
	fp12e_mul(rop, rop, dummy1);

	// Third part: Hard part (see Implementing cryptographic pairings over BN curves)
	fp12e_invert(dummy1, rop); // dummy1 = f^{-1}
	
	mpz_t exp;
    mpz_init_set(exp, sm9_x);
	mpz_mul_ui(exp, exp, 6);
	mpz_add_ui(exp, exp, 5);

	fp12e_pow(dummy2, dummy1, exp); // dummy2 = f^{-(6x+5)}
	
	fp12e_frobenius_p(dummy3, dummy2);
	fp12e_mul(dummy3, dummy3, dummy2); // dummy3 = f^{-(6x+5)p}*f^{-(6x+5)}
	fp12e_frobenius_p(fp, rop);
	fp12e_frobenius_p2(fp2, rop);
	fp12e_frobenius_p(fp3, fp2);
	
	fp12e_square(dummy1, rop);
	fp12e_square(dummy1, dummy1);

	fp12e_mul(rop, rop, fp); // rop = f*f^p

	mpz_set_ui(exp, 9);
	fp12e_pow(rop, rop, exp);
	fp12e_mul(rop, rop, dummy1);
	fp12e_mul(rop, rop, dummy2);
	fp12e_mul(rop, rop, dummy3);
	fp12e_mul(rop, rop, fp3);

	fp12e_square(dummy1, fp);
	fp12e_mul(dummy1, dummy1, fp2);
	fp12e_mul(dummy1, dummy1, dummy3);

    mpz_mul(exp, sm9_x, sm9_x);
	mpz_mul_ui(exp, exp, 6);
	mpz_add_ui(exp, exp, 1);
	fp12e_pow(dummy1, dummy1, exp);
	fp12e_mul(rop, rop, dummy1);

	mpz_clear(exp);
}

