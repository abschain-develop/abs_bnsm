

#include <stdio.h>
#include <assert.h>

#include "fp6.h"
#include "fp12e.h"
#include "fp12.h"

// Set fp12e_t rop to given value:
void fp12e_set(fp12e_t rop, const fp12e_t op)
{
	fp6e_set(rop->m_a, op->m_a);
	fp6e_set(rop->m_b, op->m_b);
}

// Initialize an fp12e, set to value given in two fp6es
void fp12e_set_fp6e(fp12e_t rop, const fp6e_t a, const fp6e_t b)
{
	fp6e_set(rop->m_a, a);
	fp6e_set(rop->m_b, b);
}

// Set rop to one:
void fp12e_setone(fp12e_t rop)
{
	fp6e_setzero(rop->m_a);
	fp6e_setone(rop->m_b);
}

// Set rop to zero:
void fp12e_setzero(fp12e_t rop)
{
	fp6e_setzero(rop->m_a);
	fp6e_setzero(rop->m_b);
}

// Add two fp12e, store result in rop:
void fp12e_add(fp12e_t rop, const fp12e_t op1, const fp12e_t op2)
{
	fp6e_add(rop->m_a, op1->m_a, op2->m_a);
	fp6e_add(rop->m_b, op1->m_b, op2->m_b);
}

// Subtract op2 from op1, store result in rop:
void fp12e_sub(fp12e_t rop, const fp12e_t op1, const fp12e_t op2)
{
	fp6e_sub(rop->m_a, op1->m_a, op2->m_a);
	fp6e_sub(rop->m_b, op1->m_b, op2->m_b);
}

// Multiply two fp12e, store result in rop:
void fp12e_mul(fp12e_t rop, const fp12e_t op1, const fp12e_t op2)
{

	fp6e_t tmp1, tmp2, tmp3; // Needed to store intermediary results

	fp6e_mul(tmp1, op1->m_a, op2->m_a);
	fp6e_mul(tmp3, op1->m_b, op2->m_b);

	fp6e_add(tmp2, op2->m_a, op2->m_b);
	fp6e_add(rop->m_a, op1->m_a, op1->m_b);
	fp6e_set(rop->m_b, tmp3);

	fp6e_mul(rop->m_a, rop->m_a, tmp2);
	fp6e_sub(rop->m_a, rop->m_a, tmp1);
	fp6e_sub(rop->m_a, rop->m_a, rop->m_b);
	fp6e_multau(tmp1, tmp1);
	//fp6e_mul(tmp1, tmp1, tau);
	fp6e_add(rop->m_b, rop->m_b, tmp1);

}

void fp12e_mul_fp6e(fp12e_t rop, const fp12e_t op1, const fp6e_t op2)
{
	fp6e_mul(rop->m_a, op1->m_a, op2);
	fp6e_mul(rop->m_b, op1->m_b, op2);
}

// Square an fp12e, store result in rop:
void fp12e_square(fp12e_t rop, const fp12e_t op)
{

	fp6e_t tmp1, tmp2, tmp3; // Needed to store intermediary results

	fp6e_mul(tmp1, op->m_a, op->m_b);

	fp6e_add(tmp2, op->m_a, op->m_b);
	fp6e_multau(tmp3, op->m_a);
	//fp6e_mul(tmp3, op->m_a, tau);
	fp6e_add(rop->m_b, tmp3, op->m_b);
	fp6e_mul(rop->m_b, rop->m_b, tmp2);

	fp6e_sub(rop->m_b, rop->m_b, tmp1);
	fp6e_multau(tmp2, tmp1);
	//fp6e_mul(tmp2, tmp1, tau);
	fp6e_sub(rop->m_b, rop->m_b, tmp2);

	fp6e_add(rop->m_a, tmp1, tmp1);

}

void fp12e_pow(fp12e_t rop, const fp12e_t op, const mpz_t exp)
{
	fp12e_t dummy;
	fp12e_set(dummy, op);
	fp12e_set(rop, op);
	int i;
	for(i = mpz_sizeinbase(exp, 2) - 1; i > 0; i--)
	{
		fp12e_square(rop, rop);
		if(mpz_tstbit(exp, i - 1)) 
			fp12e_mul(rop, rop, dummy);
	}
}


void fp12e_invert(fp12e_t rop, const fp12e_t op)
{

	fp6e_t tmp1, tmp2; // Needed to store intermediary results

	fp6e_square(tmp1, op->m_a);
	fp6e_square(tmp2, op->m_b);
	fp6e_multau(tmp1, tmp1);
	//fp6e_mul(tmp1, tmp1, tau);
	fp6e_sub(tmp1, tmp2, tmp1);
	fp6e_invert(tmp1, tmp1);
	fp12e_set(rop, op);
	fp6e_neg(rop->m_a, rop->m_a);
	fp12e_mul_fp6e(rop, rop, tmp1);

}

void fp12e_frobenius_p(fp12e_t rop, const fp12e_t op)
{
	fp6e_frobenius_p(rop->m_a, op->m_a);
	fp6e_frobenius_p(rop->m_b, op->m_b);
	fp6e_mul_fp2e(rop->m_a, rop->m_a, zpminus1);
}

void fp12e_frobenius_p2(fp12e_t rop, const fp12e_t op)
{
	fp6e_frobenius_p2(rop->m_a, op->m_a);
	fp6e_frobenius_p2(rop->m_b, op->m_b);
	fp6e_mul_fpe(rop->m_a, rop->m_a, zeta);
	fp6e_neg(rop->m_a, rop->m_a);
}

// Print the element to stdout:
void fp12e_print(FILE *outfile, const fp12e_t op)
{
	fp6e_print(outfile, op->m_a);
	fprintf(outfile, " * Z + ");
	fp6e_print(outfile, op->m_b);
}
