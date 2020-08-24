

#ifndef FP12E_H
#define FP12E_H

#include <gmp.h>
#include "fp6e.h"

// Elements from F_{p^{12}}= F_{p^6}[Z] / (Z^2 - tau)F_{p^6}[Z] are represented as aZ + b
typedef struct fp12e_struct fp12e_struct_t;

struct fp12e_struct
{
	fp6e_t m_a;
	fp6e_t m_b;
};

typedef fp12e_struct_t fp12e_t[1];

// Set fp12e_t rop to given value:
void fp12e_set(fp12e_t rop, const fp12e_t op);

// Initialize an fp12e, set to value given in two fp6es
void fp12e_set_fp6e(fp12e_t rop, const fp6e_t a, const fp6e_t b);

// Set rop to one:
void fp12e_setone(fp12e_t rop);

// Set rop to zero:
void fp12e_setzero(fp12e_t rop);

// Add two fp12e, store result in rop:
void fp12e_add(fp12e_t rop, const fp12e_t op1, const fp12e_t op2);

// Subtract op2 from op1, store result in rop:
void fp12e_sub(fp12e_t rop, const fp12e_t op1, const fp12e_t op2);

// Multiply two fp12e, store result in rop:
void fp12e_mul(fp12e_t rop, const fp12e_t op1, const fp12e_t op2);

void fp12e_mul_fp6e(fp12e_t rop, const fp12e_t op1, const fp6e_t op2);

// Square an fp12e, store result in rop:
void fp12e_square(fp12e_t rop, const fp12e_t op);

void fp12e_pow(fp12e_t rop, const fp12e_t op, const mpz_t exp);

void fp12e_invert(fp12e_t rop, const fp12e_t op);

void fp12e_frobenius_p(fp12e_t rop, const fp12e_t op);

void fp12e_frobenius_p2(fp12e_t rop, const fp12e_t op);

// Scalar multiple of an fp12e, store result in rop:
void fp12e_mul_mpz(fp12e_t rop, const fp12e_t op1, const mpz_t op2);

// Print the element to stdout:
void fp12e_print(FILE *outfile, const fp12e_t op);

#endif // ifndef FP12E_H
