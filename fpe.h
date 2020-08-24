

#ifndef FPE_H
#define FPE_H

#include <gmp.h>
#include <stdio.h>

#include "fp.h"
#include "parameters.h"

#ifndef M_LN2
#define M_LOG10E 0.43429448190325182765
#define M_LN2 0.69314718055994530942
#endif

typedef struct fpe_struct fpe_struct_t;

struct fpe_struct
{
	mp_limb_t m_value[N_LIMBS];
};

typedef fpe_struct_t fpe_t[1];

// Set fpe_t rop to given value:
void fpe_set(fpe_t rop, const fpe_t op);

// Set fpe_t rop to given value:
void fpe_set_ui(fpe_t rop, const unsigned int op);

// Set fpe_t rop to value given in bytearray -- inverse function to fpe_to_bytearray
void fpe_set_bytearray(fpe_t rop, const unsigned char *op, size_t oplen);

// Set fpe_t rop to given value given as (ASCII) string
void fpe_set_str(fpe_t rop, const char* op);

// Set rop to one
void fpe_setone(fpe_t rop);

// Set rop to zero
void fpe_setzero(fpe_t rop);

// Return 1 if op is zero, 0 otherwise
int fpe_iszero(const fpe_t op);

// Compute the negative of an fpe
void fpe_neg(fpe_t rop, const fpe_t op);

// Double an fpe:
void fpe_double(fpe_t rop, const fpe_t op);

// Halve an fpe:
void fpe_halve(fpe_t rop, const fpe_t op);

// Triple an fpe:
void fpe_triple(fpe_t rop, const fpe_t op);

// Add two fpe, store result in rop:
void fpe_add(fpe_t rop, const fpe_t op1, const fpe_t op2);

// Subtract op2 from op1, store result in rop:
void fpe_sub(fpe_t rop, const fpe_t op1, const fpe_t op2);

// Multiply two fpe, store result in rop:
void fpe_mul(fpe_t rop, const fpe_t op1, const fpe_t op2);

// Square an fpe, store result in rop:
// void fpe_square(fpe_t rop, const fpe_t op);
#define fpe_square(rop, op) fpe_mul(rop, op, op)

// Scalar multiple of an fpe, store result in rop:
void fpe_mul_mpz(fpe_t rop, const fpe_t op1, const mpz_t op2);

// Scalar multiple of an fpe, store result in rop:
void fpe_mul_ui(fpe_t rop, const fpe_t op1, const unsigned int op2);

// Compute inverse of an fpe, store result in rop:
void fpe_invert(fpe_t rop, const fpe_t op1);

// Print the element to stdout:
void fpe_print(FILE * outfile, const fpe_t op);

// Convert fpe into a bytearray, the destination must have space for NLIMBS mpz_limb_t
void fpe_to_bytearray(unsigned char * rop, const fpe_t op);

void fpe_montgomery_retrans(fpe_t rop, const fpe_t op);

#endif // define FPE_H
