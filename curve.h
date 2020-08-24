
#ifndef CURVE_H
#define CURVE_H

#include <gmp.h>

extern mpz_t sm9_b; /* parameter b in the curve equation y^2 = x^3 + b */
extern mpz_t sm_n; /* order of the curve */
extern mpz_t sm9_x;
extern mpz_t sm9_trace; /* trace of Frobenius */
extern mpz_t chi; /* p^12 / n */


/* Initialize Curve paramesters */
void curve_init(void);

/* Free memory used by curve parameters */
void curve_clear(void);

#endif /* ifdef CURVE_H */
