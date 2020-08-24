

#include "fp2e.h"
#include "fp6e.h"
#include "fp12.h"
#include "parameters.h"

fp6e_t tau; // F_{p^{12}} is constructed as F_{p^6}[Z]/(Z^2 - tau) F_{p^6}[Z]
fp2e_t zpminus1; // Z^{p-1}, lies in F_{p^2}
fp2e_t zpminus1inv; // Z^{p-1}, lies in F_{p^2}

void fp12_init()
{
	fp6e_set_str(tau, BN_TAU);
	fp2e_set_str(zpminus1, BN_ZPMINUS1);
	fp2e_set_str(zpminus1inv, BN_ZPMINUS1INV);
}

void fp12_clear()
{
	;
}
