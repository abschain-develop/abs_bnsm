

#include <stdio.h>
#include <gmp.h>

#include "curve.h"
#include "fp2e.h"
#include "fp6.h"
#include "fp6e.h"
#include "fp12e.h"
#include "curvepoint_fp.h"
#include "twistpoint_fp2.h"
#include "init.h"
#include "points.h"
#include "final_expo.h"
#include "ate_optate.h"
#include "parameters.h"

static void linefunction_add_ate(
        fp12e_t rop1, 
        twistpoint_fp2_t rop2, 
        const twistpoint_fp2_t op1, 
        const twistpoint_fp2_t op2, 
        const curvepoint_fp_t op3,
        const fp2e_t r2 /* r2 = y^2, see "Faster Computation of Tate Pairings" */
        )
{
    fp2e_t tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9, tmp10; // Temporary variables needed for intermediary results
    fp6e_t tfp6e1, tfp6e2;


    fp2e_mul(tmp0, op2->m_x, op1->m_t); /* tmp0 = B = x2 * T1  = x2z1^2*/

    fp2e_add(tmp1, op2->m_y, op1->m_z);
    fp2e_square(tmp1, tmp1);
    fp2e_sub(tmp1, tmp1, r2);
    fp2e_sub(tmp1, tmp1, op1->m_t);
    fp2e_mul(tmp1, tmp1, op1->m_t); /* tmp1 = D = ((y2 + Z1)^2 - R2 - T1)T1  = 2y2z1^3 */

    fp2e_sub(tmp2, tmp0, op1->m_x); /* tmp2 = H = B - X1  = x2z1^2 - x1*/

    fp2e_square(tmp3, tmp2); /* tmp3 = I = H^2  = (x2z1^2 - x1)^2*/

    fp2e_double(tmp4, tmp3); 
    fp2e_double(tmp4, tmp4); /* tmp4 = E = 4I = 4(x2z1^2 - x1)^2*/

    fp2e_mul(tmp5, tmp2, tmp4); /* tmp5 = J = HE =  4(x2z1^2 - x1)(x2z1^2 - x1)^2*/

    fp2e_sub(tmp6, tmp1, op1->m_y); 
    fp2e_sub(tmp6, tmp6, op1->m_y); /* tmp6 = r = 2(D - 2Y1) = (2y2z1^3 - 2y1)*/
    
    fp2e_mul(tmp9, tmp6, op2->m_x); /* Needed later: tmp9 = x2(2y2z1^3 - 2y1)*/

    fp2e_mul(tmp7, op1->m_x, tmp4); /* tmp7 = V = X1*E = 4x1(x2z1^2 - x1)^2*/

    fp2e_square(rop2->m_x, tmp6);
    fp2e_sub(rop2->m_x, rop2->m_x, tmp5);
    fp2e_sub(rop2->m_x, rop2->m_x, tmp7);
    fp2e_sub(rop2->m_x, rop2->m_x, tmp7); /* X3 = r^2 - J - 2V = (2y2z1^3 - 2y1)^2 - 4(x2z1^2 - x1)(x2z1^2 - x1)^2 - 8x1(x2z1^2 - x1)^2*/

    fp2e_add(rop2->m_z, op1->m_z, tmp2);
    fp2e_square(rop2->m_z, rop2->m_z);
    fp2e_sub(rop2->m_z, rop2->m_z, op1->m_t);
    fp2e_sub(rop2->m_z, rop2->m_z, tmp3); /* Z3 = (z1 + H)^2 - T1 - I  = 2z1(x2z1^2 - x1) */
    
    fp2e_add(tmp10, op2->m_y, rop2->m_z); /* Needed later: tmp10 = y2 + z3*/

    fp2e_sub(tmp8, tmp7, rop2->m_x);
    fp2e_mul(tmp8, tmp8, tmp6);
    fp2e_mul(tmp0, op1->m_y, tmp5);
    fp2e_double(tmp0, tmp0);
    fp2e_sub(rop2->m_y, tmp8, tmp0); /* Y3 = r(V - X3) - 2Y1*J = (2y2z1^3 - 2y1)(4x1(x2z1^2 - x1)^2 - x3) - 8y1(x2z1^2 - x1)(x2z1^2 - x1)^2*/


    fp2e_square(rop2->m_t, rop2->m_z); /* T3 = Z3^2 */

    fp2e_square(tmp10, tmp10); /* tmp10 = (y2 + z3)^2 */
    fp2e_sub(tmp10, tmp10, r2);
    fp2e_sub(tmp10, tmp10, rop2->m_t); 
    fp2e_double(tmp9, tmp9);
    fp2e_sub(tmp9, tmp9, tmp10); /* tmp9 = 4x2(y2z1^3 - y1) - 2z3y2 */

    fp2e_mul_fpe(tmp10, rop2->m_z, op3->m_y); /* tmp10 = z3y_Q */
    fp2e_double(tmp10, tmp10);

    fp2e_neg(tmp6, tmp6);
    fp2e_mul_fpe(tmp1, tmp6, op3->m_x);
    fp2e_double(tmp1, tmp1);

    fp2e_setzero(tmp2);

    fp6e_set_fp2e(tfp6e1, tmp2, tmp9, tmp1);
    fp6e_set_fp2e(tfp6e2, tmp2, tmp2, tmp10);

    fp12e_set_fp6e(rop1, tfp6e1, tfp6e2);
}

static void linefunction_double_ate(fp12e_t rop1, twistpoint_fp2_t rop2, const twistpoint_fp2_t op1, const curvepoint_fp_t op3)
{
    fp2e_t tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp7, dummy; // Temporary variables needed for intermediary results
    fp6e_t tfp6e1, tfp6e2;

    fp2e_square(tmp0, op1->m_x); /* tmp0 = A = X1^2 = x1^2 */
    fp2e_square(tmp1, op1->m_y); /* tmp1 = B = Y1^2 = y1^2 */

    fp2e_square(tmp2, tmp1); /* tmp2 = C = B^2 = y1^4 */

    fp2e_add(tmp3, op1->m_x, tmp1);
    fp2e_square(tmp3, tmp3);
    fp2e_sub(tmp3, tmp3, tmp0);
    fp2e_sub(tmp3, tmp3, tmp2);
    fp2e_double(tmp3, tmp3); /* tmp3 = D = 2(X1 + B)^2 - A - C) = 4x1y1^2 */

    fp2e_triple(tmp4, tmp0); /* tmp4 = E = 3A = 3x1^2 */
    
    fp2e_add(tmp7, tmp4, op1->m_x); /* Needed later */

    fp2e_square(tmp5, tmp4); /* tmp5 = G = E^2 = 9x1^4 */

    fp2e_sub(rop2->m_x, tmp5, tmp3);
    fp2e_sub(rop2->m_x, rop2->m_x, tmp3); /* X3 = G - 2D = 9x1^4 - 8x1y1^2 */

    fp2e_add(rop2->m_z, op1->m_y, op1->m_z);
    fp2e_square(rop2->m_z, rop2->m_z);
    fp2e_sub(rop2->m_z, rop2->m_z, tmp1);
    fp2e_sub(rop2->m_z, rop2->m_z, op1->m_t); /* Z3 = (Y1 + Z1)^2 - B - T1 = 2y1z1; */

    fp2e_sub(rop2->m_y, tmp3, rop2->m_x);
    fp2e_mul(rop2->m_y, rop2->m_y, tmp4); 
    fp2e_double(dummy, tmp2);
    fp2e_double(dummy, dummy);
    fp2e_double(dummy, dummy);
    fp2e_sub(rop2->m_y, rop2->m_y, dummy); /* Y3 = E(D - X3) - 8C = 3x1^2(4x1y1^2 - X3) - 8y1^4 */

    fp2e_mul(tmp3, tmp4, op1->m_t);
    fp2e_double(tmp3, tmp3);
    fp2e_neg(tmp3, tmp3);
    fp2e_mul_fpe(tmp3, tmp3, op3->m_x); /* tmp3 = -6x1^2z1^2 * x_Q */

    fp2e_square(tmp7, tmp7);
    fp2e_sub(tmp7, tmp7, tmp0);
    fp2e_sub(tmp7, tmp7, tmp5);
    fp2e_double(dummy, tmp1);
    fp2e_double(dummy, dummy);
    fp2e_sub(tmp7, tmp7, dummy); /* tmp7 = 6x1^3 - 4y1^2 */

    fp2e_mul(tmp0, rop2->m_z, op1->m_t);
    fp2e_double(tmp0, tmp0);
    fp2e_mul_fpe(tmp0, tmp0, op3->m_y);
    
    fp2e_square(rop2->m_t, rop2->m_z); 

    fp2e_setzero(tmp1);

    fp6e_set_fp2e(tfp6e1, tmp1, tmp7, tmp3);
    fp6e_set_fp2e(tfp6e2, tmp1, tmp1, tmp0);

    fp12e_set_fp6e(rop1, tfp6e1, tfp6e2);
}

void optate(fp12e_t rop, const twistpoint_fp2_t op1, const curvepoint_fp_t op2)
{
    // op1 and op2 are assumed to be in affine coordinates!
    fpe_t zeta2;
    fp2e_t z2p, z3p;
    fpe_set_str(zeta2, BN_ZETA2);
    fp2e_set_str(z2p, BN_Z2P);
    fp2e_set_str(z3p, BN_Z3P);
    twistpoint_fp2_t q1, q2, q3;
    fp12e_setone(rop);


    fp12e_t dummy;
    fp2e_t tfp2e1, tfp2e2;

    mpz_t _6uplus2;
    mpz_init_set(_6uplus2, sm9_x);
    mpz_mul_ui(_6uplus2, _6uplus2, 6);
    mpz_add_ui(_6uplus2, _6uplus2, 2);

    twistpoint_fp2_t r, t;
    twistpoint_fp2_set(r, op1);
    fp2e_setone(r->m_t); /* As r has to be in affine coordinates this is ok */
    fp2e_setone(t->m_t); /* As t has to be in affine coordinates this is ok */

    fp2e_t r2;
    fp2e_square(r2, op1->m_y);

    unsigned long int i;


    for(i = mpz_sizeinbase(_6uplus2, 2) - 1; i > 0; i--)
    {
        linefunction_double_ate(dummy, r, r, op2);
        fp12e_square(rop, rop);
        fp12e_mul(rop, rop, dummy);

        if (mpz_tstbit(_6uplus2, i - 1)) 
        {
            linefunction_add_ate(dummy, r, r, op1, op2, r2);
            fp12e_mul(rop, rop, dummy);
        }
    }
    // Needed because linfunction_add needs op2 in affine coordinates
    twistpoint_fp2_makeaffine(r);
    fp2e_setone(r->m_t); /* As r is in affine coordinates this is ok */

    fp2e_mul_fpe(tfp2e1, op1->m_x, zeta2);
    twistpoint_fp2_affineset_fp2e(q2, tfp2e1, op1->m_y); 

    fp2e_set(tfp2e1, op1->m_x);
    fpe_neg(tfp2e1->m_a, tfp2e1->m_a);
    fp2e_mul(tfp2e1, tfp2e1, z2p);
    fp2e_set(tfp2e2, op1->m_y);
    fpe_neg(tfp2e2->m_a, tfp2e2->m_a);
    fp2e_mul(tfp2e2, tfp2e2, z3p);
    twistpoint_fp2_affineset_fp2e(q1, tfp2e1, tfp2e2);

    fp2e_mul_fpe(tfp2e1, tfp2e1, zeta2);
    fp2e_neg(tfp2e2, tfp2e2);
    twistpoint_fp2_affineset_fp2e(q3, tfp2e1, tfp2e2);

    fp2e_setone(q3->m_t);
    fp2e_square(r2, q2->m_y);
    linefunction_add_ate(dummy, t, q3, q2, op2, r2);
    fp12e_mul(rop, rop, dummy);
    fp2e_square(r2, q1->m_y);
    linefunction_add_ate(dummy, t, t, q1, op2, r2);
    fp12e_mul(rop, rop, dummy);
    fp2e_square(r2, r->m_y);
    linefunction_add_ate(dummy, t, t, r, op2, r2);
    fp12e_mul(rop, rop, dummy);

    final_expo(rop);
}

void ate(fp12e_t rop, const twistpoint_fp2_t op1, const curvepoint_fp_t op2)
{
    fp12e_setone(rop);

    fp12e_t dummy;

    mpz_t tminus1;
    mpz_init_set(tminus1, sm9_trace);
    mpz_sub_ui(tminus1, tminus1, 1);

    twistpoint_fp2_t r;
    twistpoint_fp2_set(r, op1);
    fp2e_setone(r->m_t); /* As r has to be in affine coordinates this is ok */
    
    fp2e_t r2;
    fp2e_square(r2, op1->m_y);

    unsigned long int i;

    for(i = mpz_sizeinbase(tminus1, 2) - 1; i > 0; i--)
    {
        linefunction_double_ate(dummy, r, r, op2);
        fp12e_square(rop, rop);
        fp12e_mul(rop, rop, dummy);

        if (mpz_tstbit(tminus1, i - 1)) 
        {
            linefunction_add_ate(dummy, r, r, op1, op2, r2);
            fp12e_mul(rop, rop, dummy);
        }
    }

    final_expo(rop);

}
