

#ifndef TWISTPOINT_FP2_H
#define TWISTPOINT_FP2_H

#include "fp2e.h"

typedef struct twistpoint_fp2_struct twistpoint_fp2_struct_t;

struct twistpoint_fp2_struct
{	
	fp2e_t m_x; // X-Coordinate (Jacobian Coordinate system)
	fp2e_t m_y; // Y-Coordinate (Jacobian Coordinate system)
	fp2e_t m_z; // Z-Coordinate (Jacobian Coordinate system)
	fp2e_t m_t; // T = Z^2, only used during pairing computation, set to zero if not set
};

typedef twistpoint_fp2_struct_t twistpoint_fp2_t[1];

void twistpoint_fp2_set(twistpoint_fp2_t rop, const twistpoint_fp2_t op);

void twistpoint_fp2_set_fp2e(twistpoint_fp2_t rop, const fp2e_t x, const fp2e_t y, const fp2e_t z);

void twistpoint_fp2_affineset_fp2e(twistpoint_fp2_t rop, const fp2e_t x, const fp2e_t y);

void twistpoint_fp2_mixadd(twistpoint_fp2_t rop, const twistpoint_fp2_t op1, const twistpoint_fp2_t op2);

void twistpoint_fp2_double(twistpoint_fp2_t rop, const twistpoint_fp2_t op);

void twistpoint_fp2_mul(twistpoint_fp2_t rop, const twistpoint_fp2_t op, const mpz_t scalar);

void twistpoint_fp2_print(FILE *outfile, const twistpoint_fp2_t op);

// Transform to Affine Coordinates (z=1)
void twistpoint_fp2_makeaffine(twistpoint_fp2_t op);

void twistpoint_fp2_set_bytearray(twistpoint_fp2_t rop, unsigned char *ucXa, unsigned char *ucXb, unsigned char *ucYa, unsigned char *ucYb);


#endif // ifdef TWISTPOINT_FP2_H
