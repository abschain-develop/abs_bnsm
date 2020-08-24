

#include <stdio.h>
#include <gmp.h>
#include <assert.h>

#include "fp2.h"
#include "fp2e.h"

// Set fp2e_t rop to given value:
void fp2e_set(fp2e_t rop, const fp2e_t op)
{
	fpe_set(rop->m_a, op->m_a);
	fpe_set(rop->m_b, op->m_b);
}

// Set fp2e_t rop to given value:
void fp2e_set_fpe(fp2e_t rop, const fpe_t op)
{
	fpe_set_ui(rop->m_a, 0);
	fpe_set(rop->m_b, op);
}

// Set rop to one
void fp2e_setone(fp2e_t rop)
{
	fpe_setzero(rop->m_a);
	fpe_setone(rop->m_b);
}

// Set rop to zero
void fp2e_setzero(fp2e_t rop)
{
	fpe_setzero(rop->m_a);
	fpe_setzero(rop->m_b);;
}

// Set an fp2e_t to value given in two strings
void fp2e_set_str(fp2e_t rop, const char* a_str, const char* b_str)
{
	fpe_set_str(rop->m_a, a_str);
	fpe_set_str(rop->m_b, b_str);
}

// Double an fp2e:
void fp2e_double(fp2e_t rop, const fp2e_t op)
{
	fpe_double(rop->m_a, op->m_a);
	fpe_double(rop->m_b, op->m_b);
}

// Triple an fp2e:
void fp2e_triple(fp2e_t rop, const fp2e_t op)
{
	fpe_triple(rop->m_a, op->m_a);
	fpe_triple(rop->m_b, op->m_b);
}

// Add two fp2e, store result in rop:
void fp2e_add(fp2e_t rop, const fp2e_t op1, const fp2e_t op2)
{
	fpe_add(rop->m_a, op1->m_a, op2->m_a);
	fpe_add(rop->m_b, op1->m_b, op2->m_b);
}

// Subtract op2 from op1, store result in rop:
void fp2e_sub(fp2e_t rop, const fp2e_t op1, const fp2e_t op2)
{
	fpe_sub(rop->m_a, op1->m_a, op2->m_a);
	fpe_sub(rop->m_b, op1->m_b, op2->m_b);
}

// Negate op
void fp2e_neg(fp2e_t rop, const fp2e_t op)
{
	fpe_neg(rop->m_a, op->m_a);
	fpe_neg(rop->m_b, op->m_b);
}


// Multiply two fp2e, store result in rop:
void fp2e_mul(fp2e_t rop, const fp2e_t op1, const fp2e_t op2)
{

	fpe_t tmp1, tmp2, tmp3; // Needed for intermediary results

	if((fpe_iszero(op1->m_a) && fpe_iszero(op1->m_b)) || (fpe_iszero(op2->m_a) && fpe_iszero(op2->m_b)))
		fp2e_setzero(rop);
	else
	{
		fpe_mul(tmp1, op1->m_a, op2->m_a);
		fpe_mul(tmp2, op1->m_b, op2->m_b);
		fpe_add(tmp3, op2->m_a, op2->m_b);
		fpe_add(rop->m_a, op1->m_a, op1->m_b);
		fpe_set(rop->m_b, tmp2);
		fpe_mul(rop->m_a, rop->m_a, tmp3);
		fpe_sub(rop->m_a, rop->m_a, tmp1);
		fpe_sub(rop->m_a, rop->m_a, rop->m_b);
#if(ALPHA == 2)
		fpe_double(tmp1, tmp1);
		fpe_add(rop->m_b, rop->m_b, tmp1);
#elif(ALPHA == -2)
		fpe_double(tmp1, tmp1);
		fpe_sub(rop->m_b, rop->m_b, tmp1);
#elif(ALPHA == -1)
		fpe_sub(rop->m_b, rop->m_b, tmp1);
#else
#error "ALPHA must be -1, 2 or -2"
#endif
	}

}

// Square an fp2e, store result in rop:
void fp2e_square(fp2e_t rop, const fp2e_t op)
{

	fpe_t tmp1, tmp2, tmp3; // Needed for intermediary results

	fpe_mul(tmp1, op->m_a, op->m_b);

	fpe_add(tmp2, op->m_a, op->m_b);
#if(ALPHA == 2)
	fpe_double(tmp3, op->m_a);
	fpe_add(rop->m_b, op->m_b, tmp3);
#elif(ALPHA == -2)
	fpe_double(tmp3, op->m_a);
	fpe_sub(rop->m_b, op->m_b, tmp3);
#elif(ALPHA == -1)
	fpe_sub(rop->m_b, op->m_b, op->m_a);
#else
#error "ALPHA must be -1, 2 or -2"
#endif
	fpe_mul(rop->m_b, rop->m_b, tmp2);

	fpe_sub(rop->m_b, rop->m_b, tmp1);
#if(ALPHA == 2)
	fpe_double(tmp2, tmp1);
	fpe_sub(rop->m_b, rop->m_b, tmp2);
#elif(ALPHA == -2)
	fpe_double(tmp2, tmp1);
	fpe_add(rop->m_b, rop->m_b, tmp2);
#elif(ALPHA == -1)
	fpe_add(rop->m_b, rop->m_b, tmp1);
#else
#error "ALPHA must be -1, 2 or -2"
#endif

	fpe_double(rop->m_a, tmp1);

}

// Multiply by xi which is used to construct F_p^6
//void fp2e_mulxi(fp2e_t rop, const fp2e_t op)
//{
//    //TODO Check for XI and ALPHA
//    fpe_t tmp1,tmp2;
   
//    fpe_add(tmp1, op->m_a, op->m_a);
//    fpe_set(tmp2, op->m_b);
//    fpe_neg(rop->m_b, tmp1); //-2a
//    fpe_set(rop->m_a, tmp2);  //b
//}

// Multiply an fpe by xi which is used to construct F_p^6
//void fp2e_mulxi_fpe(fp2e_t rop, const fpe_t op)
//{
//    //TODO Check for XI
//    fpe_set(rop->m_a, op);
//}

// Scalar multiple of an fp2e, store result in rop:
void fp2e_mul_fpe(fp2e_t rop, const fp2e_t op1, const fpe_t op2)
{
	fpe_mul(rop->m_a, op1->m_a, op2);
	fpe_mul(rop->m_b, op1->m_b, op2);
}

// Inverse multiple of an fp2e, store result in rop:
void fp2e_invert(fp2e_t rop, const fp2e_t op)
{

	fpe_t tmp1, tmp2;  // Needed for intermediary results

	fpe_mul(tmp1, op->m_a, op->m_a);
	fpe_mul(tmp2, op->m_b, op->m_b);
#if(ALPHA == 2)
	fpe_double(tmp1, tmp1);
	fpe_sub(tmp2, tmp2, tmp1);
#elif(ALPHA == -2)
	fpe_double(tmp1, tmp1);
	fpe_add(tmp2, tmp2, tmp1);
#elif(ALPHA == -1)
	fpe_add(tmp2, tmp2, tmp1);
#else
#error "ALPHA must be -1, 2 or -2"
#endif
	fpe_invert(tmp2, tmp2);
	fpe_mul(rop->m_b, op->m_b, tmp2);
	fpe_neg(tmp2, tmp2);
	fpe_mul(rop->m_a, op->m_a, tmp2);

}

// Print the fp2e:
void fp2e_print(FILE *outfile, const fp2e_t op)
{
	fprintf(outfile, "(");
	fpe_print(outfile, op->m_a);
	fprintf(outfile, " * X + ");
	fpe_print(outfile, op->m_b);
	fprintf(outfile, ")");
}
