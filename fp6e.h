

#ifndef FP6E_H
#define FP6E_H

#include <gmp.h>
#include "fp2e.h"

// Elements from F_{p^6}= F_{p^2}[Y] / (Y^3 - xi)F_{p^2}[Y] are represented as aY^2 + bY + c 
typedef struct fp6e_struct fp6e_struct_t;

struct fp6e_struct
{
	fp2e_t m_a;
	fp2e_t m_b;
	fp2e_t m_c;
};

typedef fp6e_struct_t fp6e_t[1];

// Set fp6e_t rop to given value:
void fp6e_set(fp6e_t rop, const fp6e_t op);

// Initialize an fp6e, set to value given in three fp2es
void fp6e_set_fp2e(fp6e_t rop, const fp2e_t a, const fp2e_t b, const fp2e_t c);

// Initialize an fp6e, set to value given in six strings
void fp6e_set_str(fp6e_t rop, const char *a1, const char *a0, const char *b1, const char *b0, const char *c1, const char *c0);

// Set rop to one:
void fp6e_setone(fp6e_t rop);

// Set rop to zero:
void fp6e_setzero(fp6e_t rop);

// Add two fp6e, store result in rop:
void fp6e_add(fp6e_t rop, const fp6e_t op1, const fp6e_t op2);

// Subtract op2 from op1, store result in rop:
void fp6e_sub(fp6e_t rop, const fp6e_t op1, const fp6e_t op2);

// Negate an fp6e
void fp6e_neg(fp6e_t rop, const fp6e_t op);

// Multiply two fp6e, store result in rop:
void fp6e_mul(fp6e_t rop, const fp6e_t op1, const fp6e_t op2);

// Square an fp6e, store result in rop:
void fp6e_square(fp6e_t rop, const fp6e_t op);

// Multiply with tau:
void fp6e_multau(fp6e_t rop, const fp6e_t op);

void fp6e_mul_fpe(fp6e_t rop, const fp6e_t op1, const fpe_t op2);

void fp6e_mul_fp2e(fp6e_t rop, const fp6e_t op1, const fp2e_t op2);

void fp6e_invert(fp6e_t rop, const fp6e_t op);

void fp6e_frobenius_p(fp6e_t rop, const fp6e_t op);

void fp6e_frobenius_p2(fp6e_t rop, const fp6e_t op);

// Print the element to stdout:
void fp6e_print(FILE *outfile, const fp6e_t op);

#endif // ifndef FP6E_H
