#ifndef SM9_H
#define SM9_H

//#define TEST_SM9

#include "init.h"
#include "curvepoint_fp.h"
#include "twistpoint_fp2.h"
#include "points.h"
#include "fp12e.h"
#include "randompoints.h"
#include "bn_test.h"

#include "ate_optate.h"
#include "fp6.h"
#include "print_hex.h"
#include "sm3.h"
#include "sm4.h"
#include "sm9hv.h"


//+-----------------------------------------------------------
//| In data arrarys, high byte is stored in low location
//| such as : "090807...." ==> a[0] = 09, a[1] = 08 .....
//+-----------------------------------------------------------


//+-----------------------------------------------------------
//| Sign struct start
struct SignHKey
{
    unsigned char *ucPvaKey; //host private key
    unsigned char *ucPubKeyXa; //host public key
    unsigned char *ucPubKeyXb;
    unsigned char *ucPubKeyYa;
    unsigned char *ucPubKeyYb;
};
typedef struct SignHKey SignHKey_st[1];


struct SignUKey
{
  unsigned char *ucId; //user id
  unsigned long ulIdLen; //user id len
  unsigned char ucHid; //kgc id
  unsigned char *ucPvaKeyX; //user private key
  unsigned char *ucPvaKeyY; //user private key
};
typedef struct SignUKey SignUKey_st[1];

struct SignInf
{
  unsigned char *ucHash; //hash
  unsigned char *ucSignX; //sign
  unsigned char *ucSignY;
  unsigned char *ucMsg; //message
  unsigned long ulMsgLen; //message len
};
typedef struct SignInf SignInf_st[1];

//+-----------------------------------------------------------
//| key exchange struct start
struct ExchHKey
{
    unsigned char *ucPvaKey; //host private key
    unsigned char *ucPubKeyX; //host public key, G1
    unsigned char *ucPubKeyY;
};
typedef struct ExchHKey ExchHKey_st[1];

struct ExchUKey
{
  unsigned char *ucId; //user id
  unsigned long ulIdLen; //user id len
  unsigned char ucHid; //kgc id
  unsigned char *ucPvaKeyXa; //user private key, G2
  unsigned char *ucPvaKeyXb;
  unsigned char *ucPvaKeyYa;
  unsigned char *ucPvaKeyYb;
};
typedef struct ExchUKey ExchUKey_st[1];

struct ExchInf
{
  unsigned char *ucRX;
  unsigned char *ucRY;
  unsigned char ucRVaild;
  unsigned char *ucSh;
  unsigned char ucSVaild;
  unsigned char *ucRnd;
  unsigned char *ucSK;
  unsigned long ulKlen;
};
typedef struct ExchInf ExchInf_st[1];

//+-----------------------------------------------------------
//| package key transfer struct start
typedef struct ExchHKey PackHKey_st[1];

typedef struct ExchUKey PackUKey_st[1];

struct PackInf
{
  unsigned char *ucCX;
  unsigned char *ucCY;
  unsigned char *ucC2;
  unsigned char *ucC3;
  unsigned long ulK1len;
  unsigned long ulK2len;
  unsigned char *ucSK;
  unsigned long ulKlen;
  unsigned char *ucMsg;
  unsigned long ulMlen;
};
typedef struct PackInf PackInf_st[1];

//+-----------------------------------------------------------
//| Public key encryption struct start


//===============================================================================================
//===============================================================================================

//+-----------------------------------------------------------
//| init & clear
void init_sm9(void);//initialling pairing parameters
void clear_sm9(void);//initialling pairing parameters

//| init & clear end
//+------------------------------------------------------------

//+-----------------------------------------------------------
//|sign function

//hk_sm9sign: generater host key (public and private)
void hk_sm9sign(SignHKey_st HKey);

//uk_sm9sign: generater user key (public and private)
int uk_sm9sign(SignUKey_st UKey, SignHKey_st HKey);

//sign_sm9sign: sign function
void sign_sm9sign(SignInf_st Inf, SignUKey_st UKey, SignHKey_st HKey);

//chk_sm9sign: check function
int chk_sm9sign(SignInf_st Inf, SignUKey_st UKey, SignHKey_st HKey);


//+-----------------------------------------------------------
//|key exchange function
void hk_sm9exch(ExchHKey_st HKey);//generater host key (public and private)

int uk_sm9exch(ExchUKey_st UKey, ExchHKey_st HKey);//enerater user key (public and private)

//Master, initial a key exchange
int call_sm9exch(ExchInf_st InfA, ExchInf_st InfB, ExchUKey_st UKeyA, ExchUKey_st UKeyB, ExchHKey_st HKey);

//Slave, accept a key exchange
int accp_sm9exch(ExchInf_st InfA, ExchInf_st InfB, ExchUKey_st UKeyA, ExchUKey_st UKeyB, ExchHKey_st HKey);

//+-----------------------------------------------------------
//|key package function
void hk_sm9pack(PackHKey_st HKey);//generater host key (public and private)

int uk_sm9pack(PackUKey_st UKey, PackHKey_st HKey);//enerater user key (public and private)

//key package function
int pack_sm9pack(PackInf_st Inf, PackUKey_st UKey, PackHKey_st HKey);

//key depackage function
int depk_sm9pack(PackInf_st Inf, PackUKey_st UKey, PackHKey_st HKey);

//+-----------------------------------------------------------
//|public key encryption function
void penc_sm9pack(PackInf_st Inf, PackUKey_st UKey, PackHKey_st HKey, unsigned char encflag);

int pdec_sm9pack(PackInf_st Inf, PackUKey_st UKey, PackHKey_st HKey, unsigned char encflag);

#endif
