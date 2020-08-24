//+++++++++++++++++++++++++++++++++++++++++++++++++
//| Copyright @ HZFLK
//| Author : Xiedongfu
//| Email : xiedongfu@hzflk.com
//| Date : 2017.3.29
//+++++++++++++++++++++++++++++++++++++++++++++++++
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "init.h"
#include "curvepoint_fp.h"
#include "twistpoint_fp2.h"
#include "points.h"
#include "fp12e.h"
#include "randompoints.h"
#include "ate_optate.h"
#include "fp6.h"
#include "print_hex.h"
#include "grade.h"
#include "curve.h"
#include "fpe.h"


//+-----------------------------------------------
//| init_grade: initialling system parameters
//| clear_grade: free ram by initialling
//+-----------------------------------------------

void init_grade(void)
{
    init_globals();//initialling pairing parameters

}

void clear_grade(void)
{
    clear_globals();//initialling pairing parameters

}

void fpe_test(void)
{
    int i;
    mpz_t kt, kt_i;
    twistpoint_fp2_t pubT, pubT_mi, pubT_i;
    fpe_t sec, sec1, sec2, sec3;
    fp12e_t pubEt, pubEtm, pubEtm1, pubEtm2, pubEtm3;
    mpz_t mSigma1, mSigma2, mSigma3;
    unsigned long ulUserGrade = 1;

    mpz_init(kt_i);
    mpz_init(kt);
    get_random_g(kt);

    mpz_invert(kt_i, kt, sm_n);
    twistpoint_fp2_mul(pubT_mi, twist_gen, kt_i);
    twistpoint_fp2_makeaffine(pubT_mi);

    curvepoint_fp_mul(sec, curve_gen, kt);
    curvepoint_fp_makeaffine(sec);

    optate(pubEtm, pubT_mi, sec);//E = e(P2, P1)
    fp12e_print_hex(pubEtm);
    printf("\n");

    optate(pubEt, twist_gen, curve_gen);//E = e(P2, P1)
    fp12e_print_hex(pubEt);
    printf("\n");

    mpz_init(mSigma3);
    mpz_init(mSigma1);
    get_random_g(mSigma1);

    mpz_init(mSigma2);
    mpz_sub(mSigma2, kt, mSigma1);

    mpz_set(mSigma3, kt);

    printf("kt=\n");
    mpz_out_str(stdout, 16, kt);
    printf("\n");
    printf("mSigma1=\n");
    mpz_out_str(stdout, 16, mSigma1);
    printf("\n");
    printf("mSigma2=\n");
    mpz_out_str(stdout, 16, mSigma2);
    printf("\n");

    printf("mSigma2 mod=\n");
    mpz_mod(mSigma2, mSigma2, sm_n);//mpz_mod important
    mpz_out_str(stdout, 16, mSigma2);
    printf("\n\n");

    //mpz_set_si(kt, 9);
    mpz_invert(kt_i, kt, sm_n);
    twistpoint_fp2_mul(pubT_mi, twist_gen, kt_i);
    twistpoint_fp2_makeaffine(pubT_mi);

    //mpz_set_si(mSigma1, 3);
    curvepoint_fp_mul(sec1, curve_gen, mSigma1);
    curvepoint_fp_makeaffine(sec1);
    optate(pubEtm1, pubT_mi, sec1);
    printf("mSigma1 ate=\n");
    fp12e_print_hex(pubEtm1);
    printf("\n");

    //mpz_set_si(mSigma2, 6);
    curvepoint_fp_mul(sec2, curve_gen, mSigma2);
    curvepoint_fp_makeaffine(sec2);
    optate(pubEtm2, pubT_mi, sec2);
    printf("mSigma2 ate=\n");
    fp12e_print_hex(pubEtm2);
    printf("\n");


    fp12e_mul(pubEtm, pubEtm1, pubEtm2);
    fp12e_print_hex(pubEtm);
    printf("\n");

    //mpz_set_si(mSigma3, 9);
    optate(pubEt, pubT_mi, curve_gen);
    fp12e_pow(pubEtm2, pubEt, mSigma3);
    fp12e_print_hex(pubEtm2);
    printf("\n");

    mpz_clear(kt_i);
    mpz_clear(kt);
    mpz_clear(mSigma1);
    mpz_clear(mSigma2);

}

//+-----------------------------------------------
//| master key: private key & public key
//+-----------------------------------------------
void mst_key_grade(unsigned char *ucMstPubKey, unsigned char *ucMstPvaKey)
{
    int i, j;
    unsigned char ucEquFlag = 0;
    mpz_t kt;
    mpz_t kd[GRADE_KEY_NUM];
    size_t len;

    fp12e_t pubEt;
    twistpoint_fp2_t pvaD;
    curvepoint_fp_t pubT;

    //+--------------------------------------------
    //|share key, private
    mpz_init(kt);
    get_random_g(kt);

    mpz_export(&ucMstPvaKey[0], &len, 1, 1, 0, 0, kt);  //share key
    memmove(&ucMstPvaKey[32-len], &ucMstPvaKey[0], len);
    memset(&ucMstPvaKey[0], 0, 32-len);

    //+--------------------------------------------
    //|share key, public
    optate(pubEt, twist_gen, curve_gen);//E = e(P2, P1)
    fp12e_pow(pubEt, pubEt, kt);//Et
    fp12e_to_bytearray(ucMstPubKey, pubEt);

#ifdef GRADE_TEST
    printf("kt = \n");
    mpz_out_str(stdout, 16, kt);
    printf("\n");
    printf("Et = \n");
    fp12e_print_hex(pubEt);
    printf("\n");
    optate(pubEt, twist_gen, curve_gen);//E = e(P2, P1)
    printf("E = \n");
    fp12e_print_hex(pubEt);
    printf("\n");
#endif

    do{
        for(i=0; i<GRADE_KEY_NUM; i++)
        {
            //+--------------------------------------------
            //|part keys, private
            mpz_init(kd[i]);
            get_random_g(kd[i]);
            twistpoint_fp2_mul(pvaD, twist_gen, kd[i]);//D = [kd]P2
            twistpoint_fp2_makeaffine(pvaD);

            fpe_to_bytearray(&ucMstPvaKey[32*(1+i*4+0)], pvaD->m_x->m_a);
            fpe_to_bytearray(&ucMstPvaKey[32*(1+i*4+1)], pvaD->m_x->m_b);
            fpe_to_bytearray(&ucMstPvaKey[32*(1+i*4+2)], pvaD->m_y->m_a);
            fpe_to_bytearray(&ucMstPvaKey[32*(1+i*4+3)], pvaD->m_y->m_b);

            //+--------------------------------------------
            //|part keys, public
            mpz_invert(kt, kd[i], sm_n);//invert kd
            curvepoint_fp_mul(pubT, curve_gen, kt);
            curvepoint_fp_makeaffine(pubT);

            fpe_to_bytearray(&ucMstPubKey[32*(12+i*2+0)], pubT->m_x);
            fpe_to_bytearray(&ucMstPubKey[32*(12+i*2+1)], pubT->m_y);

            #ifdef GRADE_TEST
                    printf("kd = \n");
                    mpz_out_str(stdout, 16, kd[i]);
                    printf("\n");
                    optate(pubEt, pvaD, pubT);//E = e(P2, P1)
                    printf("E%d = \n",i);
                    fp12e_print_hex(pubEt);
                    printf("\n");
            #endif
        }

        for(i=0; i<GRADE_KEY_NUM-1; i++)
        {
            for(j=i+1; j<GRADE_KEY_NUM; j++)
            {
                if(kd[i]==kd[j])
                {
                    ucEquFlag = 1;
                    break;
                }
            }
        }
    }while(ucEquFlag);

    //+-------------------------------------------------
    //|free ram
    mpz_clear(kt);
    for(i=0; i<GRADE_KEY_NUM; i++)
    {
        mpz_clear(kd[i]);
    }

}


//+-----------------------------------------------
//| user key: private key
//+-----------------------------------------------
int user_key_grade(unsigned char *ucUserPvaKey, //user key, output
                    unsigned long ulUserGrade,//grade, input
                    unsigned char *ucMstPubKey,//master public key, input
                    unsigned char *ucMstPvaKey) //master private key, input
{
    int i, j;
    unsigned char ucEquFlag = 0;
    mpz_t kt, kd;
    mpz_t mSigma[GRADE_KEY_NUM];
    twistpoint_fp2_t pvaD, pvaU;
    curvepoint_fp_t pubT;
    fp12e_t Et, Em;

    if((ulUserGrade == 0) || (ulUserGrade >128))
        return -1;

    mpz_init(kt);
    mpz_init(kd);

    mpz_set_ui(kd, 0);
    mpz_import(kt, 32, 1, 1, 0, 0, ucMstPvaKey);//private, share key, input

    do{
        for(i=0; i<=ulUserGrade; i++)
        {
            mpz_init(mSigma[i]);
            if(i==ulUserGrade)
            {
                mpz_sub(mSigma[i], kt, kd);
                mpz_mod(mSigma[i], mSigma[i], sm_n);
            }
            else
            {
                get_random_g(mSigma[i]);
                mpz_add(kd, kd, mSigma[i]);
            }

            twistpoint_fp2_set_bytearray(pvaD, &ucMstPvaKey[32*(1+i*4+0)], &ucMstPvaKey[32*(1+i*4+1)],
                                               &ucMstPvaKey[32*(1+i*4+2)], &ucMstPvaKey[32*(1+i*4+3)]);
            twistpoint_fp2_mul(pvaU, pvaD, mSigma[i]);//U = [sigma]D
            twistpoint_fp2_makeaffine(pvaU);

            fpe_to_bytearray(&ucUserPvaKey[32*(i*4+0)], pvaU->m_x->m_a);
            fpe_to_bytearray(&ucUserPvaKey[32*(i*4+1)], pvaU->m_x->m_b);
            fpe_to_bytearray(&ucUserPvaKey[32*(i*4+2)], pvaU->m_y->m_a);
            fpe_to_bytearray(&ucUserPvaKey[32*(i*4+3)], pvaU->m_y->m_b);
        }

        for(i=0; i<ulUserGrade; i++)
        {
            for(j=i+1; j<=ulUserGrade; j++)
            {
                if(mSigma[i]==mSigma[j])
                {
                    ucEquFlag = 1;
                    break;
                }
            }
        }
    }while(ucEquFlag);

#ifdef GRADE_TEST
    for(i=0; i<=ulUserGrade; i++)
    {
        twistpoint_fp2_set_bytearray(pvaU, &ucUserPvaKey[32*(i*4+0)], &ucUserPvaKey[32*(i*4+1)],
                                           &ucUserPvaKey[32*(i*4+2)], &ucUserPvaKey[32*(i*4+3)]);
        curvepoint_fp_set_bytearray(pubT, &ucMstPubKey[32*(12+i*2+0)], &ucMstPubKey[32*(12+i*2+1)]);

        optate(Em, pvaU, pubT);//E = e(P2, P1)
        if(i==0)
            fp12e_set(Et, Em);
        else
            fp12e_mul(Et, Et, Em);

    }
    printf("Et mul = \n");
    fp12e_print_hex(Et);
    printf("\n");
    bytearray_to_fp12e(Et, ucMstPubKey);
    printf("Et = \n");
    fp12e_print_hex(Et);
    printf("\n");

#endif

    //+------------------------------------
    //|free ram
    for(i=0; i<=ulUserGrade; i++)
    {
        mpz_clear(mSigma[i]);
    }
    mpz_clear(kt);
    mpz_clear(kd);

    return 0;
}


int enc_grade(unsigned char *ucKey, //key generated, output
              unsigned long ulKeyLen, //key len, input
              unsigned char *ucDocGradeCip, //Grade ciper for sending with encrypted data, output
              unsigned long ulDocGrade,//grade, input
              unsigned char *ucDocID, //document ID, input
              unsigned long ulDocIDLen, //document ID len, input
              unsigned char *ucMstPubKey)//master public key, input
{
    int i, j;
    fp12e_t Et;
    fp12e_t Ets;
    curvepoint_fp_t pubT, pubTs;
    mpz_t ks;
    unsigned char *ucByteTmp;
    size_t len;

    if((ulDocGrade == 0) || (ulDocGrade>128))
        return -1;

    mpz_init(ks);
    get_random_g(ks);

    bytearray_to_fp12e(Et, ucMstPubKey);
    fp12e_pow(Ets, Et, ks);//calcute Ets

    for(i=0; i<=ulDocGrade; i++)  //
    {
        curvepoint_fp_set_bytearray(pubT, &ucMstPubKey[32*(12+i*2+0)], &ucMstPubKey[32*(12+i*2+1)]);
        curvepoint_fp_mul(pubTs, pubT, ks);
        curvepoint_fp_makeaffine(pubTs);
        fpe_to_bytearray(&ucDocGradeCip[64*i], pubTs->m_x);
        fpe_to_bytearray(&ucDocGradeCip[64*i+32], pubTs->m_y);
    }

    ucByteTmp = (unsigned char *) malloc(384 + 64*(ulDocGrade+1) + ulDocIDLen);
    len = 0;
    memcpy(&ucByteTmp[len], ucDocGradeCip, 64*(ulDocGrade+1));
    len = len + 64*(ulDocGrade+1);
    fp12e_to_bytearray(&ucByteTmp[len], Ets);
    len = len + 384;
    memcpy(&ucByteTmp[len], ucDocID, ulDocIDLen);
    len = len + ulDocIDLen;
    sm9kdf(ucKey, ulKeyLen, ucByteTmp, len);

    free(ucByteTmp);
    mpz_clear(ks);
    return 0;

}

int dec_grade(unsigned char *ucKey, //key generated, output
              unsigned long ulKeyLen, //key len, input
              unsigned char *ucDocGradeCip, //Grade ciper for sending with encrypted data, output
              unsigned long ulDocGrade,//doc grade, input
              unsigned char *ucDocID, //document ID, input
              unsigned long ulDocIDLen, //document ID len, input
              unsigned long ulUserGrade,//user grade, input
              unsigned char *ucUserPvaKey)//master public key, input
{
    int i;
    size_t len;
    curvepoint_fp_t pubT;
    twistpoint_fp2_t pvaU;
    fp12e_t Em;
    fp12e_t Ets;
    unsigned char *ucByteTmp;

//    if(ulUserGrade>ulDocGrade)
//        return -1;

    for(i=0; i<=ulUserGrade; i++)
    {
        twistpoint_fp2_set_bytearray(pvaU, &ucUserPvaKey[32*(i*4+0)], &ucUserPvaKey[32*(i*4+1)],
                                           &ucUserPvaKey[32*(i*4+2)], &ucUserPvaKey[32*(i*4+3)]);
        curvepoint_fp_set_bytearray(pubT, &ucDocGradeCip[32*(i*2+0)], &ucDocGradeCip[32*(i*2+1)]);

        optate(Em, pvaU, pubT);//E = e(P2, P1)
        if(i==0)
            fp12e_set(Ets, Em);
        else
            fp12e_mul(Ets, Ets, Em);
    }

    ucByteTmp = (unsigned char *) malloc(384 + 64*(ulDocGrade+1) + ulDocIDLen);
    len = 0;
    memcpy(&ucByteTmp[len], ucDocGradeCip, 64*(ulDocGrade+1));
    len = len + 64*(ulDocGrade+1);
    fp12e_to_bytearray(&ucByteTmp[len], Ets);
    len = len + 384;
    memcpy(&ucByteTmp[len], ucDocID, ulDocIDLen);
    len = len + ulDocIDLen;
    sm9kdf(ucKey, ulKeyLen, ucByteTmp, len);

    free(ucByteTmp);
    return 0;
}
