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
#include "sm9hv.h"
#include "sm9.h"
#include "curve.h"


//+-----------------------------------------------
//| General function
//+-----------------------------------------------
int cycflag = 1;

int BelongG1(unsigned char *ucX, unsigned char *ucY)
{
    mpz_t x3, y2;
    int chkflag = 0x3;

    mpz_init(x3);
    mpz_init(y2);
    mpz_import(x3, 32, 1, 1, 0, 0, ucX);
    mpz_import(y2, 32, 1, 1, 0, 0, ucY);
    mpz_powm_ui(x3, x3, 3, sm9_p);
    mpz_powm_ui(y2, y2, 2, sm9_p);
    mpz_add_ui(x3, x3, 5);
    if(mpz_cmp(y2, x3)){
        chkflag = -1; //check,y^2 = x^3 + 5
    }
    else{
        chkflag = 0;
    }

    mpz_clear(x3);
    mpz_clear(y2);
    return chkflag;
}

//+-----------------------------------------------
//| sm9-1 : parameter
//| init_sm9: initialling system parameters
//| clear_sm9: free ram by initialling SM9SIGN
//+-----------------------------------------------

void init_sm9(void)
{
	init_globals();//initialling pairing parameters
	
}

void clear_sm9(void)
{
	clear_globals();//initialling pairing parameters
	
}

//+-----------------------------------------------
//| sm9-2 : dsa
//| hk_sm9sign: generater host key (public and private)
//| uk_sm9sign: generater user key (public and private)
//| sign_sm9sign: sign function
//| chk_sm9sign: check function
//+-----------------------------------------------
void hk_sm9sign(SignHKey_st HKey)
{
    mpz_t ks;
    twistpoint_fp2_t pubs;
    size_t len;

    mpz_init(ks);
    get_random_r(ks);

#ifdef TEST_SM9 //set ks & pubs, used in test
    mpz_init_set_str(ks, KS_SIGN, 10);
#endif

    twistpoint_fp2_mul(pubs, twist_gen, ks);
    mpz_export(HKey->ucPvaKey, &len, 1, 1, 0, 0, ks);
    memmove(&HKey->ucPvaKey[32-len], &HKey->ucPvaKey[0], len);
    memset(&HKey->ucPvaKey[0], 0, 32-len);
    twistpoint_fp2_makeaffine(pubs);
    fpe_to_bytearray(HKey->ucPubKeyXa, pubs->m_x->m_a);
    fpe_to_bytearray(HKey->ucPubKeyXb, pubs->m_x->m_b);
    fpe_to_bytearray(HKey->ucPubKeyYa, pubs->m_y->m_a);
    fpe_to_bytearray(HKey->ucPubKeyYb, pubs->m_y->m_b);

#ifdef TEST_SM9
    printf("\n ks =");
    mpz_out_str(stdout, 16, ks);
    printf("\n ksu=");
    int i;
    for(i=0; i<32; i++)
    {
      printf("%02X", HKey->ucPvaKey[i]);
    }
    printf("\n pubs_xa1=");
    fpe_print_hex(pubs->m_x->m_a);
    printf("\n pubs_xa2=");
    for(i=0; i<32; i++)
    {
      printf("%02X", HKey->ucPubKeyXa[i]);
    }
    printf("\n");
#endif

    mpz_clear(ks);
}

//+-----------------------------------------------------
//|uk_sm9sign: sm9-2,p5-6
//+----------------------------------------------------------------------------------------
int uk_sm9sign(SignUKey_st UKey, SignHKey_st HKey)
{
    mpz_t mHout;
    mpz_t ks;
    curvepoint_fp_t CurPvaKey;
    int ret = 0;
    unsigned char *ucIdAHid;

    ucIdAHid = (unsigned char *) malloc((UKey->ulIdLen + 1) * sizeof(unsigned char));
    memcpy(ucIdAHid, UKey->ucId, UKey->ulIdLen);
    ucIdAHid[UKey->ulIdLen] = UKey->ucHid;

    mpz_init(mHout);
    mpz_init(ks);
    mpz_import(ks, 32, 1, 1, 0, 0, HKey->ucPvaKey);
    sm9h(mHout, ucIdAHid, UKey->ulIdLen+1, sm_n, 0x01);
    mpz_add(mHout, mHout, ks);
    mpz_mod(mHout, mHout, sm_n);

    if(!mpz_cmp_ui(mHout, 0)){
        ret = -1;
        goto UK_RET;
    }

    mpz_invert(mHout, mHout, sm_n);
    mpz_mul(mHout, mHout, ks);
    mpz_mod(mHout, mHout, sm_n);
    curvepoint_fp_mul(CurPvaKey, curve_gen, mHout);
    curvepoint_fp_makeaffine(CurPvaKey);
    fpe_to_bytearray(UKey->ucPvaKeyX, CurPvaKey->m_x);
    fpe_to_bytearray(UKey->ucPvaKeyY, CurPvaKey->m_y);
    ret = 0;


#ifdef TEST_SM9
    printf("\n ukx1=");
    fpe_print_hex(CurPvaKey->m_x);
    printf("\n ukx2=");
    int i;
    for(i=0; i<32; i++)
    {
      printf("%02X", UKey->ucPvaKeyX[i]);
    }
    printf("\n");
    printf("\n uky1=");
    fpe_print_hex(CurPvaKey->m_y);
    printf("\n uky2=");
    for(i=0; i<32; i++)
    {
      printf("%02X", UKey->ucPvaKeyY[i]);
    }
    printf("\n");
#endif

UK_RET:
    mpz_clear(mHout);
    mpz_clear(ks);
    free(ucIdAHid);
    return ret;
}


//+----------------------------------------------------------------------------------------
//|sign_sm9sign: sm9-2,p5-6
//+----------------------------------------------------------------------------------------
void sign_sm9sign(SignInf_st Inf, SignUKey_st UKey, SignHKey_st HKey)
{
    twistpoint_fp2_t PubsSm9;

    //set pubs
    twistpoint_fp2_set_bytearray(PubsSm9, HKey->ucPubKeyXa, HKey->ucPubKeyXb, HKey->ucPubKeyYa, HKey->ucPubKeyYb);

    fp12e_t Gpubs;
    mpz_t Rrnd;
    mpz_t mHout;
    mpz_t mL;
    fp12e_t Wgpr;
    unsigned char ucWgprInchar[384];
    unsigned char *ucMw;

    optate(Gpubs, PubsSm9, curve_gen);

    ucMw = (unsigned char *) malloc((Inf->ulMsgLen + 384) * sizeof(unsigned char));
    mpz_init(Rrnd);
    mpz_init(mHout);
    mpz_init(mL);
    do{
        get_random_r(Rrnd);

#ifdef TEST_SM9  //test data, r
    mpz_init_set_str(Rrnd, SM9_R, 16);
#endif

        fp12e_pow(Wgpr, Gpubs, Rrnd);
        fp12e_to_bytearray(ucWgprInchar, Wgpr);
        memcpy(ucMw, Inf->ucMsg, Inf->ulMsgLen);
        memcpy(&ucMw[Inf->ulMsgLen], ucWgprInchar, 384);
        sm9h(mHout, ucMw, Inf->ulMsgLen+384, sm_n, 0x02);
        mpz_sub(mL, Rrnd, mHout);
        mpz_mod(mL, mL, sm_n);
    }while(!mpz_cmp_ui(mL,0));

    curvepoint_fp_t dsa;
    curvepoint_fp_t sign;
    size_t len;
    curvepoint_fp_set_bytearray(dsa, UKey->ucPvaKeyX, UKey->ucPvaKeyY);
    curvepoint_fp_mul(sign, dsa, mL);
    curvepoint_fp_makeaffine(sign);

    mpz_export(Inf->ucHash, &len, 1, 1, 0, 0, mHout);  //export hash
    memmove(&Inf->ucHash[32-len], &Inf->ucHash[0], len);
    memset(&Inf->ucHash[0], 0, 32-len);
    fpe_to_bytearray(Inf->ucSignX, sign->m_x);  //export sign
    fpe_to_bytearray(Inf->ucSignY, sign->m_y);


#ifdef TEST_SM9
    printf("\n curve_gen_x=\n");
    fpe_print_hex(curve_gen->m_x);
    printf("\n curve_gen_y=\n");
    fpe_print_hex(curve_gen->m_y);
    printf("\n pubs_xa=\n");
    fp2e_print_hex(PubsSm9->m_x);
    printf("\n pubs_xb=\n");
    fp2e_print_hex(PubsSm9->m_y);
    printf("\n g=e(pubs, p1)=\n");
    fp12e_print_hex(Gpubs);
    printf("\n w=g^r=\n");
    fp12e_print_hex(Wgpr);
    printf("\n mw=\n");
    int i = 0, j=0;
    for(i=0;i<(Inf->ulMsgLen+384);i++)
    {
        printf("%02X", ucMw[i]);
        j++;
        if(j==40){
            j=0;
            printf("\n");
        }

    }
    printf("\n hash=\n");
    mpz_out_str(stdout, 16, mHout);
    printf("\n");
    printf("\n signx=\n");
    fpe_print_hex(sign->m_x);
    printf("\n signy=\n");
    fpe_print_hex(sign->m_y);

    printf("\n ucSignX=\n");
    for(i=0; i<32; i++)
    {
      printf("%02X", Inf->ucSignX[i]);
    }
    printf("\n");

#endif

    free(ucMw);
    mpz_clear(Rrnd);
    mpz_clear(mHout);
    mpz_clear(mL);
}


//+----------------------------------------------------------------------------------------
//|chk_sm9sign:sm9-2,p6-7
//|return 0 : hash is right
//|return -1 : condition is not fullfish
//|return 1 : hash is error
//+----------------------------------------------------------------------------------------
int chk_sm9sign(SignInf_st Inf, SignUKey_st UKey, SignHKey_st HKey)
{
  mpz_t mHash;
  curvepoint_fp_t crSign;
  int chkflag = 0x3;
  fp12e_t Gpubs;
  fp12e_t Tgph;
  fp12e_t Uchk;
  fp12e_t Wchk;
  twistpoint_fp2_t PubsSm9;
  mpz_t mHout;
  twistpoint_fp2_t twP;
  unsigned char ucWchkInchar[384];
  unsigned char *ucMw;
  unsigned char *ucIdAHid;
  mpz_init(mHout);
  mpz_init(mHash);
  ucIdAHid = (unsigned char *) malloc((UKey->ulIdLen + 1) * sizeof(unsigned char));
  ucMw = (unsigned char *) malloc((Inf->ulMsgLen + 384) * sizeof(unsigned char));

  mpz_import(mHash, 32, 1, 1, 0, 0, Inf->ucHash);
  if((mpz_cmp(mHash, sm_n)>=0) || (mpz_cmp_ui(mHash, 1)<0)){
      chkflag = -1; //check 0 < h < N
      goto CHK_RET;
  }

  if(BelongG1(Inf->ucSignX, Inf->ucSignY)){
      chkflag = -1; //check,y^2 = x^3 + 5
      goto CHK_RET;
  }

  memcpy(ucIdAHid, UKey->ucId, UKey->ulIdLen);
  ucIdAHid[UKey->ulIdLen] = UKey->ucHid;

  curvepoint_fp_set_bytearray(crSign, Inf->ucSignX, Inf->ucSignY);
  twistpoint_fp2_set_bytearray(PubsSm9, HKey->ucPubKeyXa, HKey->ucPubKeyXb, HKey->ucPubKeyYa, HKey->ucPubKeyYb);
  optate(Gpubs, PubsSm9, curve_gen);
  fp12e_pow(Tgph, Gpubs, mHash);
  sm9h(mHout, ucIdAHid, UKey->ulIdLen+1, sm_n, 0x01);
  twistpoint_fp2_mul(twP, twist_gen, mHout);
  twistpoint_fp2_mixadd(twP, twP, PubsSm9);
  twistpoint_fp2_makeaffine(twP);
  optate(Uchk, twP, crSign);
  fp12e_mul(Wchk, Uchk, Tgph);
  fp12e_to_bytearray(ucWchkInchar, Wchk);
  memcpy(ucMw, Inf->ucMsg, Inf->ulMsgLen);
  memcpy(&ucMw[Inf->ulMsgLen], ucWchkInchar, 384);
  sm9h(mHout, ucMw, Inf->ulMsgLen+384, sm_n, 0x02);

  if(mpz_cmp(mHout, mHash))
      chkflag = 1;  //hash is error
  else
      chkflag = 0;  //hash is right

#ifdef TEST_SM9
  printf("\n Wchk = \n");
  fp12e_print_hex(Wchk);
  printf("\n mHout = \n");
  mpz_out_str(stdout, 16, mHout);
  printf("\n mHash = \n");
  mpz_out_str(stdout, 16, mHash);
  printf("\n sign check is right, if chkflag = 0 \n");
#endif

CHK_RET:
  mpz_clear(mHout);
  mpz_clear(mHash);
  free(ucIdAHid);
  free(ucMw);

  return chkflag;
}

//+================================================================================================
//| sm9-3 : key exchange
//+================================================================================================
//+----------------------------------------------------
//| hk_sm9exch: generater host key (public and private)
//+----------------------------------------------------
void hk_sm9exch(ExchHKey_st HKey)
{
    mpz_t ke;
    curvepoint_fp_t pube;
    size_t len;

    mpz_init(ke);
    get_random_r(ke);

#ifdef TEST_SM9 //set ks & pubs, used in test
    mpz_init_set_str(ke, KEP_SIGN, 16);
#endif

    curvepoint_fp_mul(pube, curve_gen, ke);
    mpz_export(HKey->ucPvaKey, &len, 1, 1, 0, 0, ke);
    memmove(&HKey->ucPvaKey[32-len], &HKey->ucPvaKey[0], len);
    memset(&HKey->ucPvaKey[0], 0, 32-len);
    curvepoint_fp_makeaffine(pube);
    fpe_to_bytearray(HKey->ucPubKeyX, pube->m_x);
    fpe_to_bytearray(HKey->ucPubKeyY, pube->m_y);


#ifdef TEST_SM9
    printf("\n ke =");
    mpz_out_str(stdout, 16, ke);
    printf("\n keu=");
    int i;
    for(i=0; i<32; i++)
    {
      printf("%02X", HKey->ucPvaKey[i]);
    }
    printf("\n pube_xa1=");
    fpe_print_hex(pube->m_x);
    printf("\n pube_xa2=");
    for(i=0; i<32; i++)
    {
      printf("%02X", HKey->ucPubKeyX[i]);
    }
    printf("\n");
#endif

    mpz_clear(ke);
}

//+-----------------------------------------------------
//|uk_sm9exch: sm9-2,p5-6
//+----------------------------------------------------------------------------------------
int uk_sm9exch(ExchUKey_st UKey, ExchHKey_st HKey)
{
    mpz_t mHout;
    mpz_t ke;
    twistpoint_fp2_t CurPvaKey;
    int ret = 0;
    unsigned char *ucIdAHid;

    ucIdAHid = (unsigned char *) malloc((UKey->ulIdLen + 1) * sizeof(unsigned char));
    memcpy(ucIdAHid, UKey->ucId, UKey->ulIdLen);
    ucIdAHid[UKey->ulIdLen] = UKey->ucHid;

    mpz_init(mHout);
    mpz_init(ke);
    mpz_import(ke, 32, 1, 1, 0, 0, HKey->ucPvaKey);
    sm9h(mHout, ucIdAHid, UKey->ulIdLen+1, sm_n, 0x01);
    mpz_add(mHout, mHout, ke);
    mpz_mod(mHout, mHout, sm_n);

    if(!mpz_cmp_ui(mHout, 0)){
        ret = -1;
        goto UK_RET;
    }

    mpz_invert(mHout, mHout, sm_n);
    mpz_mul(mHout, mHout, ke);
    mpz_mod(mHout, mHout, sm_n);
    twistpoint_fp2_mul(CurPvaKey, twist_gen, mHout);
    twistpoint_fp2_makeaffine(CurPvaKey);
    fpe_to_bytearray(UKey->ucPvaKeyXa, CurPvaKey->m_x->m_a);
    fpe_to_bytearray(UKey->ucPvaKeyXb, CurPvaKey->m_x->m_b);
    fpe_to_bytearray(UKey->ucPvaKeyYa, CurPvaKey->m_y->m_a);
    fpe_to_bytearray(UKey->ucPvaKeyYb, CurPvaKey->m_y->m_b);
    ret = 0;

#ifdef TEST_SM9
    printf("\n ida=");
    int i;
    for(i=0; i<=UKey->ulIdLen; i++)
    {
      printf("%02X", ucIdAHid[i]);
    }
    printf("\n");printf("\n ke=");
    mpz_out_str(stdout, 16, ke);
    printf("\n mHout=");
    mpz_out_str(stdout, 16, mHout);

    printf("\n ukx1=");
    fpe_print_hex(CurPvaKey->m_x->m_a);
    printf("\n ukx2=");
    for(i=0; i<32; i++)
    {
      printf("%02X", UKey->ucPvaKeyXa[i]);
    }
    printf("\n");
    printf("\n uky1=");
    fpe_print_hex(CurPvaKey->m_y->m_a);
    printf("\n uky2=");
    for(i=0; i<32; i++)
    {
      printf("%02X", UKey->ucPvaKeyYa[i]);
    }
    printf("\n");
#endif

UK_RET:
    mpz_clear(mHout);
    mpz_clear(ke);
    free(ucIdAHid);
    return ret;
}

//+----------------------------------------------------------------------------------------
//|call_sm9exch: sm9-3,p6-7, init a key exchange, A to B
//|accp_sm9exch: accept a key exchange, B to A
//+----------------------------------------------------------------------------------------
void genQ_sm9exch(curvepoint_fp_t QaR, ExchUKey_st UKey, ExchHKey_st HKey)
{
    mpz_t mHout;
    curvepoint_fp_t pube;
    unsigned char *ucIdAHid;

    mpz_init(mHout);
    ucIdAHid = (unsigned char *) malloc((UKey->ulIdLen + 1) * sizeof(unsigned char));
    memcpy(ucIdAHid, UKey->ucId, UKey->ulIdLen);
    ucIdAHid[UKey->ulIdLen] = UKey->ucHid;
    sm9h(mHout, ucIdAHid, UKey->ulIdLen+1, sm_n, 0x01);
    curvepoint_fp_mul(QaR, curve_gen, mHout);
    curvepoint_fp_set_bytearray(pube, HKey->ucPubKeyX, HKey->ucPubKeyY);
    curvepoint_fp_mixadd(QaR, QaR, pube);
    curvepoint_fp_makeaffine(QaR);

    free(ucIdAHid);
    mpz_clear(mHout);
}

void genRar_sm9exch(ExchInf_st Inf, ExchUKey_st UKey, ExchHKey_st HKey)
{   //key B generate inf A

    curvepoint_fp_t QaR;
    mpz_t mRnd;
    size_t len;

    mpz_init(mRnd);
    genQ_sm9exch(QaR, UKey, HKey);
    get_random_r(mRnd);

#ifdef TEST_SM9
    if(cycflag) {
        mpz_set_str(mRnd, RND_A, 16);
    }else {
        mpz_set_str(mRnd, RND_B, 16);
    }
#endif

    curvepoint_fp_mul(QaR, QaR, mRnd);
    curvepoint_fp_makeaffine(QaR);
    fpe_to_bytearray(Inf->ucRX, QaR->m_x);
    fpe_to_bytearray(Inf->ucRY, QaR->m_y);
    Inf->ucRVaild = 1;  //Ra or Rb is generated
    mpz_export(Inf->ucRnd, &len, 1, 1, 0, 0, mRnd);//ra or rb stored
    memmove(&Inf->ucRnd[32-len], &Inf->ucRnd[0], len);
    memset(&Inf->ucRnd[0], 0, 32-len);

#ifdef TEST_SM9
    int i = 0;
        printf("\n ra = \n");
        mpz_out_str(stdout, 16, mRnd);
        printf("\n");
        printf("\n Ra = \n");
        fpe_print_hex(QaR->m_x);
        printf("\n");
        printf("\n IDB = \n");
#endif

    mpz_clear(mRnd);
}

void hg_sm9exch(unsigned char *ucHout, fp12e_t g2, fp12e_t g3, ExchInf_st InfA, ExchInf_st InfB, ExchUKey_st UKeyA, ExchUKey_st UKeyB)
{
    unsigned char *ucHinTmp;
    unsigned char *ucByteTmp;
    unsigned long ulLen = 0;

    ucHinTmp = (unsigned char *) malloc(((UKeyA->ulIdLen) + (UKeyB->ulIdLen) + 1280) * sizeof(unsigned char));
    ucByteTmp = (unsigned char *) malloc(384 * sizeof(unsigned char));
    ulLen = 0;
    fp12e_to_bytearray(ucByteTmp, g2);
    memcpy(&ucHinTmp[ulLen], ucByteTmp, 384);
    fp12e_to_bytearray(ucByteTmp, g3);
    ulLen = ulLen + 384;
    memcpy(&ucHinTmp[ulLen], ucByteTmp, 384);
    ulLen = ulLen + 384;
    memcpy(&ucHinTmp[ulLen], UKeyA->ucId, UKeyA->ulIdLen);
    ulLen = ulLen + UKeyA->ulIdLen;
    memcpy(&ucHinTmp[ulLen], UKeyB->ucId, UKeyB->ulIdLen);
    ulLen = ulLen + UKeyB->ulIdLen;
    memcpy(&ucHinTmp[ulLen], InfA->ucRX, 32);
    ulLen = ulLen + 32;
    memcpy(&ucHinTmp[ulLen], InfA->ucRY, 32);
    ulLen = ulLen + 32;
    memcpy(&ucHinTmp[ulLen], InfB->ucRX, 32);
    ulLen = ulLen + 32;
    memcpy(&ucHinTmp[ulLen], InfB->ucRY, 32);
    ulLen = ulLen + 32;
    sm3(ucHinTmp, ulLen, ucHout);

    free(ucHinTmp);
    free(ucByteTmp);
}

void sk_sm9exch(ExchInf_st Inf, fp12e_t g1, fp12e_t g2, fp12e_t g3, ExchInf_st InfA, ExchInf_st InfB, ExchUKey_st UKeyA, ExchUKey_st UKeyB)
{
    unsigned char *ucHinTmp;
    unsigned char *ucByteTmp;
    unsigned long ulLen = 0;

    ucHinTmp = (unsigned char *) malloc(((UKeyA->ulIdLen) + (UKeyB->ulIdLen) + 1280) * sizeof(unsigned char));
    ucByteTmp = (unsigned char *) malloc(384 * sizeof(unsigned char));
    ulLen = 0;
    memcpy(&ucHinTmp[ulLen], UKeyA->ucId, UKeyA->ulIdLen);
    ulLen = ulLen + UKeyA->ulIdLen;
    memcpy(&ucHinTmp[ulLen], UKeyB->ucId, UKeyB->ulIdLen);
    ulLen = ulLen + UKeyB->ulIdLen;
    memcpy(&ucHinTmp[ulLen], InfA->ucRX, 32);
    ulLen = ulLen + 32;
    memcpy(&ucHinTmp[ulLen], InfA->ucRY, 32);
    ulLen = ulLen + 32;
    memcpy(&ucHinTmp[ulLen], InfB->ucRX, 32);
    ulLen = ulLen + 32;
    memcpy(&ucHinTmp[ulLen], InfB->ucRY, 32);
    ulLen = ulLen + 32;
    fp12e_to_bytearray(ucByteTmp, g1);
    memcpy(&ucHinTmp[ulLen], ucByteTmp, 384);
    ulLen = ulLen + 384;
    fp12e_to_bytearray(ucByteTmp, g2);
    memcpy(&ucHinTmp[ulLen], ucByteTmp, 384);
    ulLen = ulLen + 384;
    fp12e_to_bytearray(ucByteTmp, g3);
    memcpy(&ucHinTmp[ulLen], ucByteTmp, 384);
    ulLen = ulLen + 384;
    sm9kdf(Inf->ucSK, Inf->ulKlen, ucHinTmp, ulLen);

#ifdef TEST_SM9
//    int i = 0, j = 0;
//    printf("\n SKin = \n");
//    for(j=0; j<33; j++){
//        printf("\n");
//    for(i=0; i<40; i++)
//        printf("%02X", ucHinTmp[i+j*40]);
//    }
//    printf("\n");
//    printf("\n SKB = \n");
//    for(i=0; i<InfB->ulKlen; i++)
//        printf("%02X", InfB->ucSK[i]);
//    printf("\n");
#endif

    free(ucHinTmp);
    free(ucByteTmp);
}

//+--------------------------------------------------------------------
//|call_sm9exch, return state:
//|retflag = 0;  //do nothing
//|retflag = 1;  //A contacting to B is ok
//|retflag = 2;  //A: key recieved from B is right
//|retflag = -1;  //A contacting to B is ERROR
//|retflag = -2; //A: key recieved from B is error
//+--------------------------------------------------------------------
int call_sm9exch(ExchInf_st InfA, ExchInf_st InfB, ExchUKey_st UKeyA, ExchUKey_st UKeyB, ExchHKey_st HKey)
{
    int retflag = 0;
    fp12e_t g1, g2, g3;
    curvepoint_fp_t pube;
    curvepoint_fp_t Rb;
    twistpoint_fp2_t dea;
    mpz_t mRndA;
    unsigned char ucS1[32];
    unsigned char *ucHinTmp;
    unsigned char *ucByteTmp;
    unsigned long ulLen = 0;

    mpz_init(mRndA);
    ucHinTmp = (unsigned char *) malloc(((UKeyA->ulIdLen) + (UKeyB->ulIdLen) + 1280) * sizeof(unsigned char));
    ucByteTmp = (unsigned char *) malloc(384 * sizeof(unsigned char));
    retflag = 0;
    if(!(InfA->ucRVaild))
    {// exchange parameter
        genRar_sm9exch(InfA, UKeyB, HKey);
        retflag = 1;  //A contacting to B is ok
    }

    if(InfB->ucRVaild)
    {//judge key
      if(!BelongG1(InfB->ucRX, InfB->ucRY)){
          curvepoint_fp_set_bytearray(pube, HKey->ucPubKeyX, HKey->ucPubKeyY);
          curvepoint_fp_set_bytearray(Rb, InfB->ucRX, InfB->ucRY);
          twistpoint_fp2_set_bytearray(dea, UKeyA->ucPvaKeyXa, UKeyA->ucPvaKeyXb, UKeyA->ucPvaKeyYa, UKeyA->ucPvaKeyYb);
          mpz_import(mRndA, 32, 1, 1, 0, 0, InfA->ucRnd);
          optate(g1, twist_gen, pube);
          fp12e_pow(g1,g1, mRndA);
          optate(g2, dea, Rb);
          fp12e_pow(g3,g2, mRndA);

          ulLen = 0;
          ucHinTmp[ulLen] = 0x82;
          fp12e_to_bytearray(ucByteTmp, g1);
          ulLen = 1;
          memcpy(&ucHinTmp[ulLen], ucByteTmp, 384);
          ulLen = ulLen + 384;
          hg_sm9exch(ucByteTmp, g2, g3, InfA, InfB, UKeyA, UKeyB);
          memcpy(&ucHinTmp[ulLen], ucByteTmp, 32);
          ulLen = ulLen + 32;
          sm3(ucHinTmp, ulLen, ucS1);  //generate S1

          sk_sm9exch(InfA, g1, g2, g3, InfA, InfB, UKeyA, UKeyB);  //generate SKA

          ulLen = 0;
          ucHinTmp[ulLen] = 0x83;
          fp12e_to_bytearray(ucByteTmp, g1);
          ulLen = 1;
          memcpy(&ucHinTmp[ulLen], ucByteTmp, 384);
          ulLen = ulLen + 384;
          hg_sm9exch(ucByteTmp, g2, g3, InfA, InfB, UKeyA, UKeyB);
          memcpy(&ucHinTmp[ulLen], ucByteTmp, 32);
          ulLen = ulLen + 32;
          sm3(ucHinTmp, ulLen, InfA->ucSh);  //generate SA
          InfA->ucSVaild = 1;

          if(InfB->ucSVaild){//SB is vaild
            if(memcmp(ucS1, InfB->ucSh, 32))
                retflag = -2; //A: key recieved from B is error
            else
                retflag = 2;  //A: key recieved from B is right
          }
      }
      else{
          retflag = -1;  //A contacting to B is ERROR
      }
    }

#ifdef TEST_SM9
    int i = 0;
    printf("\n g1 = \n");
    fp12e_print_hex(g1);
    printf("\n g2 = \n");
    fp12e_print_hex(g2);
    printf("\n g3 = \n");
    fp12e_print_hex(g3);
    printf("\n");
    printf("\n InfA->ucRX = \n");
    for(i=0; i<32; i++)
        printf("%02X", InfA->ucRX[i]);
    printf("\n InfA->ucRY = \n");
    for(i=0; i<32; i++)
        printf("%02X", InfA->ucRY[i]);
    printf("\n");
    printf("\n SA = \n");
    for(i=0; i<32; i++)
        printf("%02X", ucS1[i]);
    printf("\n");
#endif


EXCH_RET:
    mpz_clear(mRndA);
    free(ucHinTmp);
    free(ucByteTmp);
    return retflag;

}

//+--------------------------------------------------------------------
//|accp_sm9exch, return state:
//|retflag = 0;  //do nothing
//|retflag = 1;  //B contacting to A is ok
//|retflag = 2;  //B: key recieved from A is right
//|retflag = -1;  //B contacting to A is ERROR
//|retflag = -2; //B: key recieved from A is error
//+--------------------------------------------------------------------
int accp_sm9exch(ExchInf_st InfA, ExchInf_st InfB, ExchUKey_st UKeyA, ExchUKey_st UKeyB, ExchHKey_st HKey)
{
    int retflag = 0;
    fp12e_t g1, g2, g3;
    curvepoint_fp_t pube;
    curvepoint_fp_t Ra;
    twistpoint_fp2_t deb;
    mpz_t mRndB;
    unsigned char ucS2[32];
    unsigned char *ucHinTmp;
    unsigned char *ucByteTmp;
    unsigned long ulLen = 0;

    mpz_init(mRndB);
    ucHinTmp = (unsigned char *) malloc(((UKeyA->ulIdLen) + (UKeyB->ulIdLen) + 1280) * sizeof(unsigned char));
    ucByteTmp = (unsigned char *) malloc(384 * sizeof(unsigned char));
    retflag = 0;
    if(!(InfB->ucRVaild))
    {// exchange parameter
        genRar_sm9exch(InfB, UKeyA, HKey);
        retflag = 1;  //B contacting to A is ok
    }

    if(InfA->ucRVaild)
    {//judge key
      if(!BelongG1(InfA->ucRX, InfA->ucRY)){
          curvepoint_fp_set_bytearray(pube, HKey->ucPubKeyX, HKey->ucPubKeyY);
          curvepoint_fp_set_bytearray(Ra, InfA->ucRX, InfA->ucRY);
          twistpoint_fp2_set_bytearray(deb, UKeyB->ucPvaKeyXa, UKeyB->ucPvaKeyXb, UKeyB->ucPvaKeyYa, UKeyB->ucPvaKeyYb);
          mpz_import(mRndB, 32, 1, 1, 0, 0, InfB->ucRnd);
          optate(g1, deb, Ra);
          optate(g2, twist_gen, pube);
          fp12e_pow(g2,g2, mRndB);
          fp12e_pow(g3,g1, mRndB);

          sk_sm9exch(InfB, g1, g2, g3, InfA, InfB, UKeyA, UKeyB);  //generate SKB

          ulLen = 0;
          ucHinTmp[ulLen] = 0x82;
          fp12e_to_bytearray(ucByteTmp, g1);
          ulLen = 1;
          memcpy(&ucHinTmp[ulLen], ucByteTmp, 384);
          ulLen = ulLen + 384;
          hg_sm9exch(ucByteTmp, g2, g3, InfA, InfB, UKeyA, UKeyB);
          memcpy(&ucHinTmp[ulLen], ucByteTmp, 32);
          ulLen = ulLen + 32;
          sm3(ucHinTmp, ulLen, InfB->ucSh);  //generate SB
          InfB->ucSVaild = 1;

          ulLen = 0;
          ucHinTmp[ulLen] = 0x83;
          fp12e_to_bytearray(ucByteTmp, g1);
          ulLen = 1;
          memcpy(&ucHinTmp[ulLen], ucByteTmp, 384);
          ulLen = ulLen + 384;
          hg_sm9exch(ucByteTmp, g2, g3, InfA, InfB, UKeyA, UKeyB);
          memcpy(&ucHinTmp[ulLen], ucByteTmp, 32);
          ulLen = ulLen + 32;
          sm3(ucHinTmp, ulLen, ucS2);  //generate S2

          if(InfA->ucSVaild){//SA is vaild
              if(memcmp(ucS2, InfA->ucSh, 32))
                  retflag = -2; //B: key recieved from A is error
              else
                  retflag = 2;  //B: key recieved from A is right
          }
      }
      else{
          retflag = -1;  //B contacting to A is ERROR
      }
    }

#ifdef TEST_SM9
    int i = 0;
    printf("\n g1 = \n");
    fp12e_print_hex(g1);
    printf("\n g2 = \n");
    fp12e_print_hex(g2);
    printf("\n g3 = \n");
    fp12e_print_hex(g3);
    printf("\n");
    printf("\n SKB = \n");
    for(i=0; i<InfB->ulKlen; i++)
        printf("%02X", InfB->ucSK[i]);
    printf("\n SB = \n");
    for(i=0; i<32; i++)
        printf("%02X", InfB->ucSh[i]);
    printf("\n");
    printf("\n S2 = \n");
    for(i=0; i<32; i++)
        printf("%02X", ucS2[i]);
    printf("\n");
#endif

EXCH_RET:
    mpz_clear(mRndB);
    free(ucHinTmp);
    free(ucByteTmp);
    return retflag;

}

//+================================================================================================
//| sm9-4: block key package
//+================================================================================================
void hk_sm9pack(PackHKey_st HKey)
{
    hk_sm9exch(HKey);
}

int uk_sm9pack(PackUKey_st UKey, PackHKey_st HKey)
{
    return uk_sm9exch(UKey, HKey);
}

void genk_sm9pack(PackInf_st Inf, fp12e_t g, PackUKey_st UKey)
{
    unsigned char *ucByteTmp;
    size_t len;

    ucByteTmp = (unsigned char *) malloc((448 + UKey->ulIdLen) * sizeof(unsigned char));
    len = 0;
    memcpy(&ucByteTmp[len], Inf->ucCX, 32);
    len = len + 32;
    memcpy(&ucByteTmp[len], Inf->ucCY, 32);
    len = len + 32;
    fp12e_to_bytearray(&ucByteTmp[len], g);
    len = len + 384;
    memcpy(&ucByteTmp[len], UKey->ucId, UKey->ulIdLen);
    len = len + UKey->ulIdLen;
    sm9kdf(Inf->ucSK, Inf->ulKlen, ucByteTmp, len);

#ifdef TEST_SM9
    int i = 0, j = 0;
    printf("\n Hin = \n");
    for(j=0; j<13; j++){
        printf("\n");
        for(i=0; i<36; i++)
            printf("%02X", ucByteTmp[i+j*36]);
    }
    printf("\n");
    printf("\n ucSK = \n");
    for(j=0; j<2; j++){
        printf("\n");
        for(i=0; i<36; i++)
            printf("%02X", Inf->ucSK[i+j*36]);
    }
    printf("\n");
#endif

    free(ucByteTmp);
}

int pack_sm9pack(PackInf_st Inf, PackUKey_st UKey, PackHKey_st HKey)
{
    mpz_t mRnd;
    mpz_t mKey;
    curvepoint_fp_t QaR;
    curvepoint_fp_t pube;
    fp12e_t g;

    mpz_init(mRnd);
    mpz_init(mKey);
    genQ_sm9exch(QaR, UKey, HKey);
    curvepoint_fp_set_bytearray(pube, HKey->ucPubKeyX, HKey->ucPubKeyY);
    optate(g, twist_gen, pube);

    do{
        get_random_r(mRnd);

#ifdef TEST_SM9
    mpz_set_str(mRnd, RND_C, 16);
#endif

        fp12e_pow(g, g, mRnd);
        curvepoint_fp_mul(QaR, QaR, mRnd);
        curvepoint_fp_makeaffine(QaR);
        fpe_to_bytearray(Inf->ucCX, QaR->m_x);
        fpe_to_bytearray(Inf->ucCY, QaR->m_y);
        genk_sm9pack(Inf, g, UKey);
        mpz_import(mKey, Inf->ulKlen, 1, 1, 0, 0, Inf->ucSK);
    }while(!mpz_cmp_ui(mKey, 0));

#ifdef TEST_SM9
    int i = 0;
        printf("\n ra = \n");
        mpz_out_str(stdout, 16, mRnd);
        printf("\n");
        printf("\n CX= \n");
        fpe_print_hex(QaR->m_x);
        printf("\n");
        printf("\n key = \n");
        for(i=0; i<Inf->ulKlen; i++)
            printf("%02X", Inf->ucSK[i]);
        printf("\n");
#endif

    mpz_clear(mKey);
    mpz_clear(mRnd);
    return 0;
}

//+--------------------------------------------------------------------
//|pack_sm9exch, return state:
//|retflag = 0;  //depacking right
//|retflag = -1; //C is not in G1, error
//|retflag = -2; //Key is error
//+--------------------------------------------------------------------
int depk_sm9pack(PackInf_st Inf, PackUKey_st UKey, PackHKey_st HKey)
{
    twistpoint_fp2_t deb;
    curvepoint_fp_t cip;
    fp12e_t w;
    int retflag = 0;  //depacking right
    mpz_t mKey;

    mpz_init(mKey);

    if(BelongG1(Inf->ucCX, Inf->ucCY)){
      retflag = -1;
      goto DEPK_RET;  //C is not in G1, error
    }

    curvepoint_fp_set_bytearray(cip, Inf->ucCX, Inf->ucCY);
    twistpoint_fp2_set_bytearray(deb, UKey->ucPvaKeyXa, UKey->ucPvaKeyXb, UKey->ucPvaKeyYa, UKey->ucPvaKeyYb);
    optate(w, deb, cip);
    genk_sm9pack(Inf, w, UKey);
    mpz_import(mKey, Inf->ulKlen, 1, 1, 0, 0, Inf->ucSK);
    if(!mpz_cmp_ui(mKey, 0)){
        retflag = -2;  //Key is error
    }

#ifdef TEST_SM9
    printf("\n mKey = \n");
    mpz_out_str(stdout, 16, mKey);
    printf("\n");
#endif

DEPK_RET:
    mpz_clear(mKey);
    return retflag;
}

//+================================================================================================
//| sm9-4: Public key encryption
//| encflag : 1, seq enc; 2, blk enc
//+================================================================================================
void penc_sm9pack(PackInf_st Inf, PackUKey_st UKey, PackHKey_st HKey, unsigned char encflag)
{
    unsigned int i = 0;
    unsigned char ucPk = 0;
    mpz_t mRnd;
    mpz_t mKey;
    curvepoint_fp_t QaR;
    curvepoint_fp_t pube;
    fp12e_t g;
    sm4_context ctx;
    unsigned char *ucHinTmp;

    mpz_init(mRnd);
    mpz_init(mKey);

    genQ_sm9exch(QaR, UKey, HKey);
    curvepoint_fp_set_bytearray(pube, HKey->ucPubKeyX, HKey->ucPubKeyY);
    optate(g, twist_gen, pube);

    if(encflag){//seq enc
        Inf->ulKlen = Inf->ulMlen + Inf->ulK2len;
    }else{//blk enc
        Inf->ulKlen = Inf->ulK1len + Inf->ulK2len;
    }

    do{
        get_random_r(mRnd);

#ifdef TEST_SM9
        mpz_set_str(mRnd, RND_D, 16);
#endif

        fp12e_pow(g, g, mRnd);
        curvepoint_fp_mul(QaR, QaR, mRnd);
        curvepoint_fp_makeaffine(QaR);
        fpe_to_bytearray(Inf->ucCX, QaR->m_x);
        fpe_to_bytearray(Inf->ucCY, QaR->m_y);
        genk_sm9pack(Inf, g, UKey);
        mpz_import(mKey, Inf->ulKlen, 1, 1, 0, 0, Inf->ucSK);
    }while(!mpz_cmp_ui(mKey, 0));

    ucHinTmp = (unsigned char *) malloc(((Inf->ulMlen)+(Inf->ulK1len)+(Inf->ulK2len)) * sizeof(unsigned char));
    if(encflag){
        for(i=0; i<Inf->ulMlen; i++){
            Inf->ucC2[i] = Inf->ucSK[i] ^ Inf->ucMsg[i];
        }
        memcpy(ucHinTmp, Inf->ucC2, Inf->ulMlen);
        memcpy(&ucHinTmp[Inf->ulMlen], &Inf->ucSK[Inf->ulMlen], Inf->ulK2len);
        sm3(ucHinTmp, Inf->ulMlen+Inf->ulK2len, Inf->ucC3);
    }else{
        ucPk = Inf->ulMlen % Inf->ulK1len;//mod 16 or mod 32
        ucPk = Inf->ulK1len - ucPk;
        for(i=0; i<ucPk; i++){
            Inf->ucMsg[Inf->ulMlen+i] = ucPk;
        }
        sm4_setkey_enc(&ctx,Inf->ucSK);
        sm4_crypt_ecb(&ctx, 1, Inf->ulMlen+ucPk, Inf->ucMsg, Inf->ucC2);
        memcpy(ucHinTmp, Inf->ucC2, Inf->ulMlen+ucPk);
        memcpy(&ucHinTmp[Inf->ulMlen+ucPk], &Inf->ucSK[Inf->ulK1len], Inf->ulK2len);
        sm3(ucHinTmp, Inf->ulMlen+ucPk+Inf->ulK2len, Inf->ucC3);
    }


#ifdef TEST_SM9
        printf("\n mKey = \n");
        mpz_out_str(stdout, 16, mKey);
        printf("\n");
        printf("\n CX= \n");
        fpe_print_hex(QaR->m_x);
        printf("\n");
        printf("\n key = \n");
        for(i=0; i<Inf->ulKlen; i++)
            printf("%02X", Inf->ucSK[i]);
        printf("\n");
        printf("\n C2 = \n");
        for(i=0; i<Inf->ulMlen+ucPk; i++)
            printf("%02X", Inf->ucC2[i]);
        printf("\n");
        printf("\n C3 = \n");
        for(i=0; i<Inf->ulK2len; i++)
            printf("%02X", Inf->ucC3[i]);
        printf("\n");
#endif

    free(ucHinTmp);
    mpz_clear(mRnd);
    mpz_clear(mKey);
}

//+--------------------------------------------------------------------
//|pdec_sm9pack, return state:
//|retflag = 0;  //depacking right
//|retflag = -1; //C is not in G1, error
//|retflag = -2; //Key is error
//|retflag = -3;  //Hash is error
//+--------------------------------------------------------------------
int pdec_sm9pack(PackInf_st Inf, PackUKey_st UKey, PackHKey_st HKey, unsigned char encflag)
{
    int i;
    unsigned char ucPk;
    twistpoint_fp2_t deb;
    curvepoint_fp_t cip;
    fp12e_t w;
    int retflag = 0;  //public encryption is right
    mpz_t mKey;
    sm4_context ctx;
    mpz_init(mKey);
    unsigned char *ucHinTmp;

    ucHinTmp = (unsigned char *) malloc(((Inf->ulMlen)+(Inf->ulK1len)+(Inf->ulK2len)) * sizeof(unsigned char));


    if(BelongG1(Inf->ucCX, Inf->ucCY)){
      retflag = -1;
      goto PDEC_RET;  //C is not in G1, error
    }

    curvepoint_fp_set_bytearray(cip, Inf->ucCX, Inf->ucCY);
    twistpoint_fp2_set_bytearray(deb, UKey->ucPvaKeyXa, UKey->ucPvaKeyXb, UKey->ucPvaKeyYa, UKey->ucPvaKeyYb);
    optate(w, deb, cip);
    if(encflag){//seq enc
        Inf->ulKlen = Inf->ulMlen + Inf->ulK2len;
        genk_sm9pack(Inf, w, UKey);
        mpz_import(mKey, Inf->ulMlen, 1, 1, 0, 0, Inf->ucSK);
        for(i=0; i<Inf->ulMlen; i++){
            Inf->ucMsg[i] = Inf->ucSK[i] ^ Inf->ucC2[i];
        }
        memcpy(ucHinTmp, Inf->ucC2, Inf->ulMlen);
        memcpy(&ucHinTmp[Inf->ulMlen], &Inf->ucSK[Inf->ulMlen], Inf->ulK2len);
        sm3(ucHinTmp, Inf->ulMlen+Inf->ulK2len, Inf->ucC3);

    }else{//blk enc
        Inf->ulKlen = Inf->ulK1len + Inf->ulK2len;
        genk_sm9pack(Inf, w, UKey);
        mpz_import(mKey, Inf->ulK1len, 1, 1, 0, 0, Inf->ucSK);
        ucPk = Inf->ulMlen % Inf->ulK1len;//mod 16 or mod 32
        ucPk = Inf->ulK1len - ucPk;
        sm4_setkey_dec(&ctx,Inf->ucSK);
        sm4_crypt_ecb(&ctx,0,Inf->ulMlen+ucPk,Inf->ucC2,Inf->ucMsg);
        memcpy(ucHinTmp, Inf->ucC2, Inf->ulMlen+ucPk);
        memcpy(&ucHinTmp[Inf->ulMlen+ucPk], &Inf->ucSK[Inf->ulK1len], Inf->ulK2len);
        sm3(ucHinTmp, Inf->ulMlen+ucPk+Inf->ulK2len, Inf->ucC3);
    }

    if(!mpz_cmp_ui(mKey, 0)){
        retflag = -2;  //Key is error
        goto PDEC_RET;
    }

    if(!memcmp(ucHinTmp, Inf->ucC3, 32)){
        retflag = -3;  //Hash is error
        goto PDEC_RET;
    }

#ifdef TEST_SM9
        printf("\n de mKey = \n");
        mpz_out_str(stdout, 16, mKey);
        printf("\n de key = \n");
        for(i=0; i<Inf->ulKlen; i++)
            printf("%02X", Inf->ucSK[i]);
        printf("\n");
        printf("\n de C2 = \n");
        for(i=0; i<Inf->ulMlen+ucPk; i++)
            printf("%02X", Inf->ucC2[i]);
        printf("\n");
        printf("\n de C3 = \n");
        for(i=0; i<Inf->ulK2len; i++)
            printf("%02X", Inf->ucC3[i]);
        printf("\n");

        printf("\n de Msg = \n");
        for(i=0; i<Inf->ulMlen; i++)
            printf("%02X", Inf->ucMsg[i]);
        printf("\n");
#endif

PDEC_RET:
    mpz_clear(mKey);
    free(ucHinTmp);
    return retflag;
}
