

#ifndef FP2E_H
#define FP2E_H

#include <stdio.h>
#include <gmp.h>

#include "fpe.h"

// Elements from F_{p^2}= F_p[X] / (x^2 - alpha)F_p[X] are represented as aX + b
typedef struct fp2e_struct fp2e_struct_t;

struct fp2e_struct
{
	fpe_t m_a;
	fpe_t m_b;
};

typedef fp2e_struct_t fp2e_t[1];

// Set fp2e_t rop to given value:
void fp2e_set(fp2e_t rop, const fp2e_t op);

// Set fp2e_t rop to given value contained in the subfield F_p:
void fp2e_set_fpe(fp2e_t rop, const fpe_t op);

// Set rop to one
void fp2e_setone(fp2e_t rop);

// Set rop to zero
void fp2e_setzero(fp2e_t rop);

// Set an fp2e_t to value given in two strings
void fp2e_set_str(fp2e_t rop, const char* a_str, const char* b_str);

// Double an fp2e:
void fp2e_double(fp2e_t rop, const fp2e_t op);

// Triple an fp2e:
void fp2e_triple(fp2e_t rop, const fp2e_t op);

// Add two fp2e, store result in rop:
void fp2e_add(fp2e_t rop, const fp2e_t op1, const fp2e_t op2);

// Subtract op2 from op1, store result in rop:
void fp2e_sub(fp2e_t rop, const fp2e_t op1, const fp2e_t op2);

void fp2e_neg(fp2e_t rop, const fp2e_t op);

// Multiply two fp2e, store result in rop:
void fp2e_mul(fp2e_t rop, const fp2e_t op1, const fp2e_t op2);

// Multiply two fp2e, store result in rop, don't reduce the coefficients:
void fp2e_mul_nored(fp2e_t rop, const fp2e_t op1, const fp2e_t op2);

// Square anf fp2e, store result in rop:
void fp2e_square(fp2e_t rop, const fp2e_t op);

// Multiply by xi which is used to construct F_p^6
void fp2e_mulxi(fp2e_t rop, const fp2e_t op);

// Multiply an fpe by xi which is used to construct F_p^6
//void fp2e_mulxi_fpe(fp2e_t rop, const fpe_t op);

// Multiple of an fp2e, store result in rop:
void fp2e_mul_fpe(fp2e_t rop, const fp2e_t op1, const fpe_t op2);

// Inverse multiple of an fp2e, store result in rop:
void fp2e_invert(fp2e_t rop, const fp2e_t op1);

// Print the element to stdout:
void fp2e_print(FILE * outfile, const fp2e_t op);

#endif // ifndef FP2E_H
