

#ifndef CURVEPOINT_FP_H
#define CURVEPOINT_FP_H

#include <gmp.h>
#include <stdio.h>

//#include "curve.h"
#include "fpe.h"

/// Structure describing a point on a BN-curve
typedef struct curvepoint_fp_struct curvepoint_fp_struct_t;
struct curvepoint_fp_struct
{	
	fpe_t m_x; // X-Coordinate (Jacobian Coordinate system)
	fpe_t m_y; // Y-Coordinate (Jacobian Coordinate system)
	fpe_t m_z; // Y-Coordinate (Jacobian Coordinate system)
	fpe_t m_t; // T = Z^2, only used during pairing computation, set to zero if not set
};

typedef curvepoint_fp_struct_t curvepoint_fp_t[1];

void curvepoint_fp_init(curvepoint_fp_t rop);

void curvepoint_fp_init_set_str(curvepoint_fp_t rop, const char* x,const char* y,const char* z);

void curvepoint_fp_init_set(curvepoint_fp_t rop, const curvepoint_fp_t op);

void curvepoint_fp_set_bytearray(curvepoint_fp_t rop, const unsigned char* x, const unsigned char* y);
	
// Generate a point on the curve
void curvepoint_fp_set_str(curvepoint_fp_t point, const char* x,const char* y,const char* z);

// Generate a curvepoint_fp_t by copying the coordinates from another curvepoint_fp
void curvepoint_fp_set(curvepoint_fp_t point, const curvepoint_fp_t arg);

void curvepoint_fp_mixadd(curvepoint_fp_t rop, const curvepoint_fp_t op1, const curvepoint_fp_t op2);

void curvepoint_fp_double(curvepoint_fp_t rop, const curvepoint_fp_t op);

void curvepoint_fp_mul(curvepoint_fp_t rop, const curvepoint_fp_t op, const mpz_t scalar);

// Compute the Inverse of a Point op, store result in rop:
void curvepoint_fp_neg(curvepoint_fp_t rop, const curvepoint_fp_t op);

// Transform to Affine Coordinates (z=1)
void curvepoint_fp_makeaffine(curvepoint_fp_t point);

// Print the (Jacobian) coordinates of a point
void curvepoint_fp_print(FILE *outfile, const curvepoint_fp_t point);

#endif // ifdef CURVEPOINT_FP_H
