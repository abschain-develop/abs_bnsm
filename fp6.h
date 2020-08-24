

#ifndef FP6_H
#define FP6_H

#include "fp2e.h"

extern fp2e_t sm9_xi; // constant coefficient in the irreducible polynomial Y^3 - xi, used to construct F_{p^6} as extension of F_{p^12}
extern fp2e_t ypminus1; // Y^{p-1} lies in F_{p^2}

extern fpe_t zeta; // Third root of unity in F_p fulfilling Z^{p^2} = -zeta * Z

void fp6_init(void);
void fp6_clear(void);

#endif // ifdef FP6_H
