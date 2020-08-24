#include <gmp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "fpe.h"
#include "fp.h"
#include "fp2e.h"
#include "fp2.h"
#include "fp6e.h"
#include "fp6.h"
#include "fp12e.h"
#include "fp12.h"

void fpe_print_hex(const fpe_t op)
{
    int i,j,k;
    fpe_t top;
    unsigned char rop[32];
    mp_limb_t dummy;
    size_t gmp_limb_bytes = GMP_LIMB_BITS/8;

    fpe_montgomery_retrans(top, op);
    for(i = 0; i < N_LIMBS; i++)
    {
        dummy = top->m_value[i];
        for(j = 0; j < gmp_limb_bytes; j++)
        {
            rop[gmp_limb_bytes * i + j] = (unsigned char) (dummy & 0xff);
            dummy >>= 8;
        }
    }

    k = gmp_limb_bytes * (i-1) + j;
    i = 0;
    while(k--)
    {
        printf("%02X", rop[k]);
        i++;
        if(i==4)
        {
            printf(" ");
            i = 0;
        }
    }
}

void fp2e_print_hex(const fp2e_t op)
{
    fpe_print_hex(op->m_a);
    printf(";\n");
    fpe_print_hex(op->m_b);
    printf(";\n");
}

void fp12e_print_hex(const fp12e_t op)
{
    fp2e_t a1, b1, c1, a0, b0, c0;

    fp2e_mul(a1, op->m_a->m_a, sm9_xi);
    fp2e_mul(b1, op->m_a->m_b, sm9_xi);
    fp2e_mul(c1, op->m_a->m_c, sm9_xi);
    fp2e_mul(a0, op->m_b->m_a, sm9_xi);
    fp2e_mul(b0, op->m_b->m_b, sm9_xi);
    fp2e_set(c0, op->m_b->m_c);

    fp2e_print_hex(c1);
    fp2e_print_hex(a0);
    fp2e_print_hex(b0);
    fp2e_print_hex(a1);
    fp2e_print_hex(b1);
    fp2e_print_hex(c0);
}


//+---------------------------------------------------------------------------
//|fp12e_t covert to bytearray
//+---------------------------------------------------------------------------
void fp12e_to_bytearray(unsigned char *rop, const fp12e_t op)
{
    fp2e_t a1, b1, c1, a0, b0, c0;

    fp2e_mul(a1, op->m_a->m_a, sm9_xi);
    fp2e_mul(b1, op->m_a->m_b, sm9_xi);
    fp2e_mul(c1, op->m_a->m_c, sm9_xi);
    fp2e_mul(a0, op->m_b->m_a, sm9_xi);
    fp2e_mul(b0, op->m_b->m_b, sm9_xi);
    fp2e_set(c0, op->m_b->m_c);

    fpe_to_bytearray(&rop[32*0], c1->m_a);
    fpe_to_bytearray(&rop[32*1], c1->m_b);
    fpe_to_bytearray(&rop[32*2], a0->m_a);
    fpe_to_bytearray(&rop[32*3], a0->m_b);
    fpe_to_bytearray(&rop[32*4], b0->m_a);
    fpe_to_bytearray(&rop[32*5], b0->m_b);
    fpe_to_bytearray(&rop[32*6], a1->m_a);
    fpe_to_bytearray(&rop[32*7], a1->m_b);
    fpe_to_bytearray(&rop[32*8], b1->m_a);
    fpe_to_bytearray(&rop[32*9], b1->m_b);
    fpe_to_bytearray(&rop[32*10], c0->m_a);
    fpe_to_bytearray(&rop[32*11], c0->m_b);
}

//+---------------------------------------------------------------------------
//|bytearray covert to fp12e_t
//+---------------------------------------------------------------------------
void bytearray_to_fp12e(const fp12e_t op, unsigned char *rop)
{
    fp2e_t a1, b1, c1, a0, b0, c0, xi_inv;

    fp2e_invert(xi_inv, sm9_xi);

    fpe_set_bytearray(c1->m_a, &rop[32*0], 32);
    fpe_set_bytearray(c1->m_b, &rop[32*1], 32);
    fpe_set_bytearray(a0->m_a, &rop[32*2], 32);
    fpe_set_bytearray(a0->m_b, &rop[32*3], 32);
    fpe_set_bytearray(b0->m_a, &rop[32*4], 32);
    fpe_set_bytearray(b0->m_b, &rop[32*5], 32);
    fpe_set_bytearray(a1->m_a, &rop[32*6], 32);
    fpe_set_bytearray(a1->m_b, &rop[32*7], 32);
    fpe_set_bytearray(b1->m_a, &rop[32*8], 32);
    fpe_set_bytearray(b1->m_b, &rop[32*9], 32);
    fpe_set_bytearray(c0->m_a, &rop[32*10], 32);
    fpe_set_bytearray(c0->m_b, &rop[32*11], 32);

    fp2e_mul(op->m_a->m_a, a1, xi_inv);
    fp2e_mul(op->m_a->m_b, b1, xi_inv);
    fp2e_mul(op->m_a->m_c, c1, xi_inv);
    fp2e_mul(op->m_b->m_a, a0, xi_inv);
    fp2e_mul(op->m_b->m_b, b0, xi_inv);
    fp2e_set(op->m_b->m_c, c0);

}

