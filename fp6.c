

#include "parameters.h"
#include "fp6.h"

fp2e_t sm9_xi; // constant coefficient in the irreducible polynomial Y^3 - xi, used to construct F_{p^6} as extension of F_{p^12}
fp2e_t ypminus1; // Y^{p-1} lies in F_{p^2}

fpe_t zeta; // Third root of unity in F_p fulfilling Z^{p^2} = -zeta * Z

void fp6_init()
{
    fp2e_set_str(sm9_xi, BN_XI);
	fp2e_set_str(ypminus1, BN_YPMINUS1);
	fpe_set_str(zeta, BN_ZETA);
}

void fp6_clear()
{
	;
}
