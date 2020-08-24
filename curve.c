
#include "curve.h"
#include "parameters.h"

mpz_t sm9_b; /* parameter b in the curve equation y^2 = x^3 + b */
mpz_t sm_n; /* order of the curve */
mpz_t sm9_x;
mpz_t sm9_trace; /* trace of Frobenius */
mpz_t chi; /* p^12 / n */

void curve_init(void)
{
	/* Curve parameters */
    mpz_init_set_str(sm9_x, BN_X, 10);
    mpz_init_set_str(sm_n, BN_N, 10);
	mpz_init_set_str(sm9_trace, BN_TRACE,10);
	mpz_init_set_str(chi, BN_CHI, 10); // (p^k - 1) / n
    mpz_init_set_str(sm9_b, BN_B, 10);
}

void curve_clear(void)
{
    mpz_clear(sm_n);
	mpz_clear(sm9_trace);
	mpz_clear(chi);
    mpz_clear(sm9_x);
    mpz_clear(sm9_b);
}
