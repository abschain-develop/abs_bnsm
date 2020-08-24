//+++++++++++++++++++++++++++++++++++++++++++++++++
#include "sm9geny.h"

//+-----------------------------------------------
//| function
//+-----------------------------------------------

int sqrt_y(mpz_t y, mpz_t in)
{
    int retflag = 0;
    mpz_t u, z, d, g;
    mpz_init(u);
    mpz_init(z);
    mpz_init(d);
    mpz_init(g);
    mpz_set(g, in);
    mpz_sub_ui(u, sm9_p, 5);
    mpz_div_ui(u, u, 8);
    mpz_mul_ui(d, u, 2);
    mpz_add_ui(d, d, 1);
    mpz_powm(z, g, d, sm9_p);
    if(!mpz_cmp_ui(z, 1))
    {
        mpz_add_ui(d, u, 1);
        mpz_powm(y, g, d, sm9_p);
    }
    else
    {
        mpz_sub(z, sm9_p, z);
        if(!mpz_cmp_ui(z, 1))
        {
            mpz_mul_ui(z, g, 4);
            mpz_powm(z, z, u, sm9_p);
            mpz_mul_ui(y, g, 2);
            mpz_mul(y, y, z);
            mpz_mod(y, y, sm9_p);
            mpz_sub(y, sm9_p, y);
        }
        else
        {
            retflag = -1;
        }
    }
    mpz_clear(u);
    mpz_clear(z);
    mpz_clear(d);
    mpz_clear(g);
    return retflag;
}

void gen_y(uchar8_t *Ya, uchar8_t *Yb, uchar8_t *din)
{
    int sq_vaild = 0;
    size_t len;
    mpz_t tw_x0;
    mpz_t tw_x1;
    mpz_t tmp_x0, tmp_x1, tmp_x2, tmp_x3, tmp_x4;

    mpz_init(tw_x0);
    mpz_init(tw_x1);
    mpz_init(tmp_x0);
    mpz_init(tmp_x1);
    mpz_init(tmp_x2);
    mpz_init(tmp_x3);
    mpz_init(tmp_x4);

    mpz_import(tw_x1, 32, 1, 1, 0, 0, &din[1]);
    mpz_import(tw_x0, 32, 1, 1, 0, 0, &din[33]);

    mpz_pow_ui(tmp_x0, tw_x0, 3);
    mpz_pow_ui(tmp_x1, tw_x1, 2);
    mpz_mul(tmp_x1, tmp_x1, tw_x0);
    mpz_mul_ui(tmp_x1, tmp_x1, 6);
    mpz_sub(tmp_x0, tmp_x0, tmp_x1);
    mpz_mod(tmp_x0, tmp_x0, sm9_p);

    mpz_pow_ui(tmp_x3, tw_x1, 3);
    mpz_mul_ui(tmp_x3, tmp_x3, 2);
    mpz_pow_ui(tmp_x2, tw_x0, 2);
    mpz_mul(tmp_x2, tmp_x2, tw_x1);
    mpz_mul_ui(tmp_x2, tmp_x2, 3);
    mpz_sub(tmp_x2, tmp_x2, tmp_x3);
    mpz_add_ui(tmp_x2, tmp_x2, 5);
    mpz_mod(tmp_x2, tmp_x2, sm9_p);

    //+---------------------------------------------
    //|fp2, sqrt
    mpz_mul(tmp_x1, tmp_x0, tmp_x0);//a^2
    mpz_mul(tmp_x3, tmp_x2, tmp_x2);//b^2
    mpz_mul_ui(tmp_x3, tmp_x3, 2);//2*b^2
    mpz_add(tmp_x3, tmp_x1, tmp_x3);//a^2 + 2*b^2
    sqrt_y(tmp_x4, tmp_x3);
    mpz_set(tmp_x3, tmp_x4);
    len = 2;
    do{
        mpz_add(tmp_x3, tmp_x3, tmp_x0);
        mpz_set_ui(tmp_x1, 2);
        mpz_invert(tmp_x1, tmp_x1, sm9_p);
        mpz_mul(tmp_x3, tmp_x3, tmp_x1);
        sq_vaild = sqrt_y(tmp_x3, tmp_x3);
        len++;
        if(sq_vaild)
            mpz_sub(tmp_x3, sm9_p, tmp_x4);
    }while(sq_vaild && len);

    mpz_mul_ui(tmp_x1, tmp_x3, 2);
    mpz_invert(tmp_x1, tmp_x1, sm9_p);
    mpz_mul(tmp_x1, tmp_x1, tmp_x2);
    mpz_mod(tmp_x1, tmp_x1, sm9_p);

    if(din[0]==02)
    {
        if(mpz_tstbit(tmp_x1, 0))
        {
            mpz_sub(tmp_x1, sm9_p, tmp_x1);
            mpz_sub(tmp_x3, sm9_p, tmp_x3);
        }
    }

    if(din[0]==03)
    {
        if(!mpz_tstbit(tmp_x1, 0))
        {
            mpz_sub(tmp_x1, sm9_p, tmp_x1);
            mpz_sub(tmp_x3, sm9_p, tmp_x3);
        }
    }

    mpz_export(Ya, &len, 1, 1, 0, 0, tmp_x1);
    memmove(&Ya[32-len], &Ya[0], len);
    memset(&Ya[0], 0, 32-len);

    mpz_export(Yb, &len, 1, 1, 0, 0, tmp_x3);
    memmove(&Yb[32-len], &Yb[0], len);
    memset(&Yb[0], 0, 32-len);

    mpz_clear(tmp_x0);
    mpz_clear(tmp_x1);
    mpz_clear(tmp_x2);
    mpz_clear(tmp_x3);
    mpz_clear(tmp_x4);
    mpz_clear(tw_x0);
    mpz_clear(tw_x1);
}


//+=================================================================
//|G2 ASN.1
//+=================================================================













