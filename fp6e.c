

#include <gmp.h>
#include <stdio.h>
#include <assert.h>

#include "fp6e.h"
#include "fp6.h"


// Set fp6e_t rop to given value:
void fp6e_set(fp6e_t rop, const fp6e_t op)
{
	fp2e_set(rop->m_a, op->m_a);
	fp2e_set(rop->m_b, op->m_b);
	fp2e_set(rop->m_c, op->m_c);
}

// Initialize an fp6e, set to value given in three fp2es
void fp6e_set_fp2e(fp6e_t rop, const fp2e_t a, const fp2e_t b, const fp2e_t c)
{
	fp2e_set(rop->m_a, a);
	fp2e_set(rop->m_b, b);
	fp2e_set(rop->m_c, c);
}

// Initialize an fp6e, set to value given in six strings
void fp6e_set_str(fp6e_t rop, const char *a1, const char *a0, const char *b1, const char *b0, const char *c1, const char *c0)
{
	fp2e_set_str(rop->m_a, a1, a0);
	fp2e_set_str(rop->m_b, b1, b0);
	fp2e_set_str(rop->m_c, c1, c0);
}

// Set rop to one:
void fp6e_setone(fp6e_t rop)
{
	fp2e_setzero(rop->m_a);
	fp2e_setzero(rop->m_b);
	fp2e_setone(rop->m_c);
}

// Set rop to zero:
void fp6e_setzero(fp6e_t rop)
{
	fp2e_setzero(rop->m_a);
	fp2e_setzero(rop->m_b);
	fp2e_setzero(rop->m_c);
}

// Add two fp6e, store result in rop:
void fp6e_add(fp6e_t rop, const fp6e_t op1, const fp6e_t op2)
{
	fp2e_add(rop->m_a, op1->m_a, op2->m_a);
	fp2e_add(rop->m_b, op1->m_b, op2->m_b);
	fp2e_add(rop->m_c, op1->m_c, op2->m_c);
}

// Subtract op2 from op1, store result in rop:
void fp6e_sub(fp6e_t rop, const fp6e_t op1, const fp6e_t op2)
{
	fp2e_sub(rop->m_a, op1->m_a, op2->m_a);
	fp2e_sub(rop->m_b, op1->m_b, op2->m_b);
	fp2e_sub(rop->m_c, op1->m_c, op2->m_c);
}

// Subtract op2 from op1, store result in rop:
void fp6e_neg(fp6e_t rop, const fp6e_t op)
{
	fp2e_neg(rop->m_a, op->m_a);
	fp2e_neg(rop->m_b, op->m_b);
	fp2e_neg(rop->m_c, op->m_c);
}

// Multiply two fp6e, store result in rop:
void fp6e_mul(fp6e_t rop, const fp6e_t op1, const fp6e_t op2)
{
	fp2e_t tmp1, tmp2, tmp3, tmp4, tmp5, tmp6; // Needed for intermediary values

	// See "Multiplication and Squaring in Pairing-Friendly Fields", section 4, Karatsuba method
	fp2e_mul(tmp3, op1->m_a, op2->m_a);
	fp2e_mul(tmp2, op1->m_b, op2->m_b);
	fp2e_mul(tmp1, op1->m_c, op2->m_c);

	fp2e_add(tmp4, op1->m_a, op1->m_b);
	fp2e_add(tmp5, op2->m_a, op2->m_b);
	fp2e_mul(tmp6, tmp4, tmp5); 
	fp2e_sub(tmp6, tmp6, tmp2);
	fp2e_sub(tmp6, tmp6, tmp3);
    //fp2e_mulxi(tmp6, tmp6);
    fp2e_mul(tmp6, tmp6, sm9_xi);
	fp2e_add(tmp6, tmp6, tmp1);

	fp2e_add(tmp4, op1->m_b, op1->m_c);
	fp2e_add(tmp5, op2->m_b, op2->m_c);
	fp2e_mul(rop->m_b, tmp4, tmp5);
	fp2e_sub(rop->m_b, rop->m_b, tmp1);
	fp2e_sub(rop->m_b, rop->m_b, tmp2);
    //fp2e_mulxi(tmp4, tmp3);
    fp2e_mul(tmp4, tmp3, sm9_xi);
	fp2e_add(rop->m_b, rop->m_b, tmp4);

	fp2e_add(tmp4, op1->m_a, op1->m_c);
	fp2e_add(tmp5, op2->m_a, op2->m_c);

	fp2e_set(rop->m_c, tmp6);

	fp2e_mul(rop->m_a, tmp4, tmp5);
	fp2e_sub(rop->m_a, rop->m_a, tmp1);
	fp2e_add(rop->m_a, rop->m_a, tmp2);
	fp2e_sub(rop->m_a, rop->m_a, tmp3);
}

// Square an fp6e, store result in rop:
void fp6e_square(fp6e_t rop, const fp6e_t op)
{
	fp6e_mul(rop, op, op);
}
// Multiply with tau:
void fp6e_multau(fp6e_t rop, const fp6e_t op)
{
    fp2e_t tmp1;
    fp2e_set(tmp1, op->m_b);
    fp2e_set(rop->m_b, op->m_c);
    //fp2e_mulxi(rop->m_c, op->m_a);
    fp2e_mul(rop->m_c, op->m_a, sm9_xi);
    fp2e_set(rop->m_a, tmp1);
}

void fp6e_mul_fpe(fp6e_t rop, const fp6e_t op1, const fpe_t op2)
{
	fp2e_mul_fpe(rop->m_a, op1->m_a, op2);
	fp2e_mul_fpe(rop->m_b, op1->m_b, op2);
	fp2e_mul_fpe(rop->m_c, op1->m_c, op2);
}

void fp6e_mul_fp2e(fp6e_t rop, const fp6e_t op1, const fp2e_t op2)
{
	fp2e_mul(rop->m_a, op1->m_a, op2);
	fp2e_mul(rop->m_b, op1->m_b, op2);
	fp2e_mul(rop->m_c, op1->m_c, op2);
}

void fp6e_invert(fp6e_t rop, const fp6e_t op)
{
	fp2e_t tmp1, tmp2, tmp3, tmp4, tmp5;  // Needed to store intermediary results

	// See "Implementing cryptographic pairings"
	fp2e_square(tmp1, op->m_c);
	fp2e_mul(tmp5, op->m_a, op->m_b);
    //fp2e_mulxi(tmp5, tmp5);
    fp2e_mul(tmp5, tmp5, sm9_xi);
	fp2e_sub(tmp1, tmp1, tmp5); // A
	
	fp2e_square(tmp2, op->m_a);
    //fp2e_mulxi(tmp2, tmp2);
    fp2e_mul(tmp2, tmp2, sm9_xi);
    fp2e_mul(tmp5, op->m_b, op->m_c);
	fp2e_sub(tmp2, tmp2, tmp5); // B

	fp2e_square(tmp3, op->m_b);
	fp2e_mul(tmp5, op->m_a, op->m_c);
	fp2e_sub(tmp3, tmp3, tmp5); // C

	fp2e_mul(tmp4, tmp3, op->m_b);
    //fp2e_mulxi(tmp4, tmp4);
    fp2e_mul(tmp4, tmp4, sm9_xi);
    fp2e_mul(tmp5, tmp1, op->m_c);
	fp2e_add(tmp4, tmp4, tmp5);
	fp2e_mul(tmp5, tmp2, op->m_a);
    //fp2e_mulxi(tmp5, tmp5);
    fp2e_mul(tmp5, tmp5, sm9_xi);
    fp2e_add(tmp4, tmp4, tmp5); // F
	
	fp2e_invert(tmp4, tmp4);

	fp2e_mul(rop->m_a, tmp3, tmp4);
	fp2e_mul(rop->m_b, tmp2, tmp4);
	fp2e_mul(rop->m_c, tmp1, tmp4);
}

void fp6e_frobenius_p(fp6e_t rop, const fp6e_t op)
{
	fp2e_t tmp; // Needed to store intermediary results

	fp6e_set(rop, op);
	fpe_neg((rop->m_a)->m_a, (rop->m_a)->m_a);
	fpe_neg((rop->m_b)->m_a, (rop->m_b)->m_a);
	fpe_neg((rop->m_c)->m_a, (rop->m_c)->m_a);

	fp2e_mul(rop->m_b, rop->m_b, ypminus1);
	fp2e_square(tmp, ypminus1);
	fp2e_mul(rop->m_a, rop->m_a, tmp);
}

void fp6e_frobenius_p2(fp6e_t rop, const fp6e_t op)
{
	fpe_t tmp; // Needed for intermediary results

	fpe_square(tmp, zeta);
	fp2e_set(rop->m_c, op->m_c);
	fp2e_mul_fpe(rop->m_b, op->m_b, tmp);
	fp2e_mul_fpe(rop->m_a, op->m_a, zeta);
}

// Print the fp6e:
void fp6e_print(FILE * outfile, const fp6e_t op)
{
	fprintf(outfile, "[");
	fp2e_print(outfile, op->m_a);
	fprintf(outfile, " * Y^2 + ");
	fp2e_print(outfile, op->m_b);
	fprintf(outfile, " * Y + ");
	fp2e_print(outfile, op->m_c);
	fprintf(outfile, "]");
}
