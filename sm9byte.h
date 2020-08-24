#ifndef SM9BYTE_H
#define SM9BYTE_H

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
#include "sm9geny.h"

typedef unsigned char uchar8_t;
typedef mp_limb_signed_t ulong_t;

//+====================================================================================================
//+=== Sign ===========================================================================================
//+====================================================================================================
void hk_sm9sign_by(uchar8_t *ucHPubKey,//host public key, out
                   uchar8_t *ucHPvaKey);//host private key, out

int uk_sm9sign_by(uchar8_t *ucUPvaKey,//user private key, out
                  uchar8_t ucHid, uchar8_t *ucUid, ulong_t ulIdLen,//user ID, in
                  uchar8_t *ucHPvaKey);//host public key, in

//+----------------------------------------------------------------------------------------
//|sign_sm9sign: sm9-2, p5-6
//+----------------------------------------------------------------------------------------
void sign_sm9sign_by(uchar8_t *ucHash, uchar8_t *ucSign, //hash & sign info, out
                     uchar8_t *ucMsg, ulong_t ulMsgLen,  //Msg, in
                     uchar8_t *ucUPvaKey,  //user private key, in
                     uchar8_t *ucHPubKey);  //host public key, in

//+----------------------------------------------------------------------------------------
//|chk_sm9sign:sm9-2, p6-7
//|return 0 : hash is right
//|return -1 : condition is not fullfish
//|return 1 : hash is error
//+----------------------------------------------------------------------------------------
int chk_sm9sign_by(uchar8_t *ucHash, uchar8_t *ucSign, //hash & sign info, in
                   uchar8_t *ucMsg, ulong_t ulMsgLen,  //Msg, in
                   uchar8_t ucHid, uchar8_t *ucUid, ulong_t ulIdLen, //user ID, in
                   uchar8_t *ucHPubKey);  //host public key, in

//+===================================================================================================
//+=== Exchange Key ==================================================================================
//+===================================================================================================
void hk_sm9exch_by(uchar8_t *ucHPubKey,//host public key, out
                   uchar8_t *ucHPvaKey);//host private key, out


int uk_sm9exch_by(uchar8_t *ucUPvaKey,//user private key, out
                  uchar8_t ucHid, uchar8_t *ucUid, ulong_t ulIdLen,//user ID, in
                  uchar8_t *ucHPvaKey);//host public key, in

//+--------------------------------------------------------------------
//|call_sm9exch, return state:
//|retflag = 0;  //do nothing
//|retflag = 1;  //A contacting to B is ok
//|retflag = 2;  //A: key recieved from B is right
//|retflag = -1;  //A contacting to B is ERROR
//|retflag = -2; //A: key recieved from B is error
//+--------------------------------------------------------------------
int call_sm9exch_by(uchar8_t *ucSKA, ulong_t ulKlen, //key, out; key len, in
                    uchar8_t *ucRXA, uchar8_t *ucRAVaild, uchar8_t *ucSAh, uchar8_t *ucSAVaild,//to partner, out
                    uchar8_t *ucRXB, uchar8_t *ucRBVaild, uchar8_t *ucSBh, uchar8_t *ucSBVaild,  //from partner, in
                    uchar8_t ucHid, uchar8_t *ucUseridA, ulong_t ulIdLenA, uchar8_t *ucUseridB, ulong_t ulIdLenB, //user ID, in
                    uchar8_t *ucRndA, uchar8_t *ucHPubKeyExch,  //host public key, in
                    uchar8_t *ucUPvaKeyExchA); //user prviate key in

//+--------------------------------------------------------------------
//|accp_sm9exch, return state:
//|retflag = 0;  //do nothing
//|retflag = 1;  //B contacting to A is ok
//|retflag = 2;  //B: key recieved from A is right
//|retflag = -1;  //B contacting to A is ERROR
//|retflag = -2; //B: key recieved from A is error
//+--------------------------------------------------------------------
int accp_sm9exch_by(uchar8_t *ucSKB, ulong_t ulKlen, //key, out; key len, in
                    uchar8_t *ucRXB, uchar8_t *ucRBVaild, uchar8_t *ucSBh, uchar8_t *ucSBVaild,//to partner, out
                    uchar8_t *ucRXA, uchar8_t *ucRAVaild, uchar8_t *ucSAh, uchar8_t *ucSAVaild,  //from partner, in
                    uchar8_t ucHid, uchar8_t *ucUseridA, ulong_t ulIdLenA, uchar8_t *ucUseridB, ulong_t ulIdLenB, //user ID, in
                    uchar8_t *ucRndB, uchar8_t *ucHPubKeyExch,  //host public key, in
                    uchar8_t *ucUPvaKeyExchB); //user prviate key in

//+====================================================================================================
//+=== Package Key & Encrypt messages =================================================================
//+====================================================================================================
void hk_sm9pack_by(uchar8_t *ucHPubKey, uchar8_t *ucHPvaKey);

int uk_sm9pack_by(uchar8_t *ucUPvaKey,//user private key, out
                  uchar8_t ucHid, uchar8_t *ucUid, ulong_t ulIdLen,//user ID, in
                  uchar8_t *ucHPvaKey);//host public key, in

int pack_sm9pack_by(uchar8_t *ucSK, ulong_t ulKlen, //key, out; key len, in
                    uchar8_t *ucCip, //ciper, out
                    uchar8_t ucHid, uchar8_t *ucUseridP, ulong_t ulIdLenP, //user ID, in
                    uchar8_t *ucHPubKeyPack); //host public key, in

//+--------------------------------------------------------------------
//|depk_sm9pack_by, return state:
//|retflag = 0;  //depacking right
//|retflag = -1; //C is not in G1, error
//|retflag = -2; //Key is error
//+--------------------------------------------------------------------
int depk_sm9pack_by(uchar8_t *ucSK, ulong_t ulKlen, //key, out; key len, in
                    uchar8_t *ucCip, //ciper, in
                    uchar8_t ucHid, uchar8_t *ucUseridP, ulong_t ulIdLenP, //user ID, in
                    uchar8_t *ucUPvaKeyPack); //host private key, in

//+----------------------------------------------------------------------------
//| sm9-4: Public key encryption
//| encflag : 1, seq enc; 0, blk enc
//+----------------------------------------------------------------------------
int penc_sm9pack_by(uchar8_t *ucCip, ulong_t *ulCiplen, //ciper, out; ciper len, out
                    uchar8_t *ucMsg, ulong_t ulMlen, //message in, msg len, in
                    ulong_t ulK1len, ulong_t ulK2len, uchar8_t encflag, //K1 len is the block enc key len, K2 Len is the hash key len
                    uchar8_t ucHid, uchar8_t *ucUseridP, ulong_t ulIdLenP, //user ID, in
                    uchar8_t *ucHPubKeyPack); //host public key, in

//+--------------------------------------------------------------------
//|pdec_sm9pack, return state:
//|retflag = 0;  //depacking right
//|retflag = -1; //C is not in G1, error
//|retflag = -2; //Key is error
//|retflag = -3;  //Hash is error
//+--------------------------------------------------------------------
int pdec_sm9pack_by(uchar8_t *ucMsg, ulong_t *ulMlen, //message out, msg len, out
                    uchar8_t *ucCip, ulong_t ulCiplen, //ciper, in; ciper len, in
                    ulong_t ulK1len, ulong_t ulK2len, uchar8_t encflag, //K1 len is the block enc key len, K2 Len is the hash key len
                    uchar8_t ucHid, uchar8_t *ucUseridP, ulong_t ulIdLenP, //user ID, in
                    uchar8_t *ucUPvaKeyPack); //host public key, in


#endif
