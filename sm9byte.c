//+++++++++++++++++++++++++++++++++++++++++++++++++
#include "sm9byte.h"

//+================================================
//|sign functiong, byte array in & out
//+================================================
void hk_sm9sign_by(uchar8_t *ucHPubKey, uchar8_t *ucHPvaKey)
{
    SignHKey_st HKey;
    HKey->ucPvaKey = ucHPvaKey;
    ucHPubKey[0] = 04; //flag from sm9 stantard
    HKey->ucPubKeyXa = &ucHPubKey[1];
    HKey->ucPubKeyXb = &ucHPubKey[33];
    HKey->ucPubKeyYa = &ucHPubKey[65];
    HKey->ucPubKeyYb = &ucHPubKey[97];
    hk_sm9sign(HKey);
}


int uk_sm9sign_by(uchar8_t *ucUPvaKey, uchar8_t ucHid,
                   uchar8_t *ucUid, ulong_t ulIdLen, uchar8_t *ucHPvaKey)
{
    SignHKey_st HKey;
    SignUKey_st UKey;

    HKey->ucPvaKey = ucHPvaKey;
    UKey->ucId = ucUid;
    UKey->ulIdLen = ulIdLen;
    UKey->ucHid = ucHid;
    ucUPvaKey[0] = 04; //flag from sm9 stantard
    UKey->ucPvaKeyX = &ucUPvaKey[1];
    UKey->ucPvaKeyY = &ucUPvaKey[33];

    return uk_sm9sign(UKey, HKey);
}


void sign_sm9sign_by(uchar8_t *ucHash, uchar8_t *ucSign, //hash & sign info, out
                  uchar8_t *ucMsg, ulong_t ulMsgLen,  //Msg, in
                  uchar8_t *ucUPvaKey,  //user private key, in
                  uchar8_t *ucHPubKey)  //host public key, in
{
    SignHKey_st HKey;
    SignUKey_st UKey;
    SignInf_st Inf;

    HKey->ucPubKeyXa = &ucHPubKey[1];
    HKey->ucPubKeyXb = &ucHPubKey[33];
    HKey->ucPubKeyYa = &ucHPubKey[65];
    HKey->ucPubKeyYb = &ucHPubKey[97];
    if(ucHPubKey[0] != 04){
        gen_y(HKey->ucPubKeyYa, HKey->ucPubKeyYb, ucHPubKey); //Y gen from X
    }

    UKey->ucPvaKeyX = &ucUPvaKey[1];
    UKey->ucPvaKeyY = &ucUPvaKey[33];

    Inf->ucMsg = ucMsg;
    Inf->ulMsgLen = ulMsgLen;
    Inf->ucHash = ucHash;
    ucSign[0] = 04; //flag from sm9 stantard
    Inf->ucSignX = &ucSign[1];
    Inf->ucSignY = &ucSign[33];

    sign_sm9sign(Inf, UKey, HKey);

}

//+----------------------------------------------------------------------------------------
//|chk_sm9sign:sm9-2,p6-7
//|return 0 : hash is right
//|return -1 : condition is not fullfish
//|return 1 : hash is error
//+----------------------------------------------------------------------------------------
int chk_sm9sign_by(uchar8_t *ucHash, uchar8_t *ucSign, //hash & sign info, in
                   uchar8_t *ucMsg, ulong_t ulMsgLen,  //Msg, in
                   uchar8_t ucHid, uchar8_t *ucUid, ulong_t ulIdLen, //user ID, in
                   uchar8_t *ucHPubKey)  //host public key, in
{
    SignHKey_st HKey;
    SignUKey_st UKey;
    SignInf_st Inf;

    HKey->ucPubKeyXa = &ucHPubKey[1];
    HKey->ucPubKeyXb = &ucHPubKey[33];
    HKey->ucPubKeyYa = &ucHPubKey[65];
    HKey->ucPubKeyYb = &ucHPubKey[97];
    if(ucHPubKey[0] != 04){
        gen_y(HKey->ucPubKeyYa, HKey->ucPubKeyYb, ucHPubKey); //Y gen from X
    }

    UKey->ucId = ucUid;
    UKey->ulIdLen = ulIdLen;
    UKey->ucHid = ucHid;

    Inf->ucMsg = ucMsg;
    Inf->ulMsgLen = ulMsgLen;
    Inf->ucHash = ucHash;
    ucSign[0] = 04; //flag from sm9 stantard
    Inf->ucSignX = &ucSign[1];
    Inf->ucSignY = &ucSign[33];

    return chk_sm9sign(Inf, UKey, HKey);

}

//+================================================
//|exchange function, byte array in & out
//+================================================
void hk_sm9exch_by(uchar8_t *ucHPubKey, uchar8_t *ucHPvaKey)
{
    ExchHKey_st HKey;
    HKey->ucPvaKey = ucHPvaKey;
    ucHPubKey[0] = 04; //flag from sm9 stantard
    HKey->ucPubKeyX = &ucHPubKey[1];
    HKey->ucPubKeyY = &ucHPubKey[33];
    hk_sm9exch(HKey);
}

int uk_sm9exch_by(uchar8_t *ucUPvaKey,
                  uchar8_t ucHid, uchar8_t *ucUid, ulong_t ulIdLen,
                  uchar8_t *ucHPvaKey)
{
    ExchHKey_st HKey;
    ExchUKey_st UKey;

    HKey->ucPvaKey = ucHPvaKey;
    UKey->ucId = ucUid;
    UKey->ulIdLen = ulIdLen;
    UKey->ucHid = ucHid;
    ucUPvaKey[0] = 04; //flag from sm9 stantard
    UKey->ucPvaKeyXa = &ucUPvaKey[1];
    UKey->ucPvaKeyXb = &ucUPvaKey[33];
    UKey->ucPvaKeyYa = &ucUPvaKey[65];
    UKey->ucPvaKeyYb = &ucUPvaKey[97];

    return uk_sm9exch(UKey, HKey);
}


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
                    uchar8_t *ucUPvaKeyExchA) //user prviate key in
{
    ExchHKey_st Hkey_e;
    ExchUKey_st Ukey_eA;
    ExchUKey_st Ukey_eB;
    ExchInf_st Inf_eA;
    ExchInf_st Inf_eB;
    int retflag = 0;

    Hkey_e->ucPubKeyX = &ucHPubKeyExch[1];
    Hkey_e->ucPubKeyY = &ucHPubKeyExch[33];
    Ukey_eB->ucId = ucUseridB;
    Ukey_eB->ulIdLen = ulIdLenB;
    Ukey_eB->ucHid = ucHid;
    Ukey_eA->ucId = ucUseridA;
    Ukey_eA->ulIdLen = ulIdLenA;
    Ukey_eA->ucHid = ucHid;
    Ukey_eA->ucPvaKeyXa = &ucUPvaKeyExchA[1];
    Ukey_eA->ucPvaKeyXb = &ucUPvaKeyExchA[33];
    Ukey_eA->ucPvaKeyYa = &ucUPvaKeyExchA[65];
    Ukey_eA->ucPvaKeyYb = &ucUPvaKeyExchA[97];

    Inf_eA->ucRX = &ucRXA[0];
    Inf_eA->ucRY = &ucRXA[32];
    Inf_eA->ucRVaild = *ucRAVaild;
    Inf_eA->ucSh = ucSAh;
    Inf_eA->ucSVaild = *ucSAVaild;
    Inf_eA->ucSK = ucSKA;
    Inf_eA->ulKlen = ulKlen;
    Inf_eA->ucRnd = ucRndA;

    Inf_eB->ucRX = &ucRXB[0];
    Inf_eB->ucRY = &ucRXB[32];
    Inf_eB->ucRVaild = *ucRBVaild;
    Inf_eB->ucSh = ucSBh;
    Inf_eB->ucSVaild = *ucSBVaild;

    retflag = call_sm9exch(Inf_eA, Inf_eB, Ukey_eA, Ukey_eB, Hkey_e);
    ucRAVaild[0] = Inf_eA->ucRVaild;
    ucRBVaild[0] = Inf_eB->ucRVaild;
    ucSAVaild[0] = Inf_eA->ucSVaild;
    ucSBVaild[0] = Inf_eB->ucSVaild;
    return retflag;
}


int accp_sm9exch_by(uchar8_t *ucSKB, ulong_t ulKlen, //key, out; key len, in
                    uchar8_t *ucRXB, uchar8_t *ucRBVaild, uchar8_t *ucSBh, uchar8_t *ucSBVaild,//to partner, out
                    uchar8_t *ucRXA, uchar8_t *ucRAVaild, uchar8_t *ucSAh, uchar8_t *ucSAVaild,  //from partner, in
                    uchar8_t ucHid, uchar8_t *ucUseridA, ulong_t ulIdLenA, uchar8_t *ucUseridB, ulong_t ulIdLenB, //user ID, in
                    uchar8_t *ucRndB, uchar8_t *ucHPubKeyExch,  //host public key, in
                    uchar8_t *ucUPvaKeyExchB) //user prviate key in
{
    ExchHKey_st Hkey_e;
    ExchUKey_st Ukey_eA;
    ExchUKey_st Ukey_eB;
    ExchInf_st Inf_eA;
    ExchInf_st Inf_eB;
    int retflag = 0;

    Hkey_e->ucPubKeyX = &ucHPubKeyExch[1];
    Hkey_e->ucPubKeyY = &ucHPubKeyExch[33];
    Ukey_eB->ucId = ucUseridB;
    Ukey_eB->ulIdLen = ulIdLenB;
    Ukey_eB->ucHid = ucHid;
    Ukey_eA->ucId = ucUseridA;
    Ukey_eA->ulIdLen = ulIdLenA;
    Ukey_eA->ucHid = ucHid;
    Ukey_eB->ucPvaKeyXa = &ucUPvaKeyExchB[1];
    Ukey_eB->ucPvaKeyXb = &ucUPvaKeyExchB[33];
    Ukey_eB->ucPvaKeyYa = &ucUPvaKeyExchB[65];
    Ukey_eB->ucPvaKeyYb = &ucUPvaKeyExchB[97];

    Inf_eA->ucRX = &ucRXA[0];
    Inf_eA->ucRY = &ucRXA[32];
    Inf_eA->ucRVaild = *ucRAVaild;
    Inf_eA->ucSh = ucSAh;
    Inf_eA->ucSVaild = *ucSAVaild;

    Inf_eB->ucRX = &ucRXB[0];
    Inf_eB->ucRY = &ucRXB[32];
    Inf_eB->ucRVaild = *ucRBVaild;
    Inf_eB->ucSh = ucSBh;
    Inf_eB->ucSVaild = *ucSBVaild;
    Inf_eB->ucSK = ucSKB;
    Inf_eB->ulKlen = ulKlen;
    Inf_eB->ucRnd = ucRndB;

    retflag = accp_sm9exch(Inf_eA, Inf_eB, Ukey_eA, Ukey_eB, Hkey_e);
    *ucRAVaild = Inf_eA->ucRVaild;
    *ucRBVaild = Inf_eB->ucRVaild;
    *ucSAVaild = Inf_eA->ucSVaild;
    *ucSBVaild = Inf_eB->ucSVaild;
    return retflag;
}


//+================================================================================================
//| sm9-4: block key package
//+================================================================================================
void hk_sm9pack_by(uchar8_t *ucHPubKey, uchar8_t *ucHPvaKey)
{
    hk_sm9exch_by(ucHPubKey, ucHPvaKey);
}

int uk_sm9pack_by(uchar8_t *ucUPvaKey,//user private key, out
                  uchar8_t ucHid, uchar8_t *ucUid, ulong_t ulIdLen,//user ID, in
                  uchar8_t *ucHPvaKey)//host public key, in
{
    return uk_sm9exch_by(ucUPvaKey,//user private key, out
                  ucHid, ucUid, ulIdLen,//user ID, in
                  ucHPvaKey);//host public key, in
}

int pack_sm9pack_by(uchar8_t *ucSK, ulong_t ulKlen, //key, out; key len, in
                    uchar8_t *ucCip, //ciper, out
                    uchar8_t ucHid, uchar8_t *ucUseridP, ulong_t ulIdLenP, //user ID, in
                    uchar8_t *ucHPubKeyPack) //host public key, in
{
    PackInf_st Inf;
    PackUKey_st UKey;
    PackHKey_st HKey;

    HKey->ucPubKeyX = &ucHPubKeyPack[1];
    HKey->ucPubKeyY = &ucHPubKeyPack[33];
    UKey->ucId = ucUseridP;
    UKey->ulIdLen = ulIdLenP;
    UKey->ucHid = ucHid;
    Inf->ucSK = ucSK;
    Inf->ulKlen = ulKlen;
    Inf->ucCX = &ucCip[0];
    Inf->ucCY = &ucCip[32];

    return pack_sm9pack(Inf, UKey, HKey);
}

//+--------------------------------------------------------------------
//|depk_sm9pack_by, return state:
//|retflag = 0;  //depacking right
//|retflag = -1; //C is not in G1, error
//|retflag = -2; //Key is error
//+--------------------------------------------------------------------
int depk_sm9pack_by(uchar8_t *ucSK, ulong_t ulKlen, //key, out; key len, in
                    uchar8_t *ucCip, //ciper, in
                    uchar8_t ucHid, uchar8_t *ucUseridP, ulong_t ulIdLenP, //user ID, in
                    uchar8_t *ucUPvaKeyPack) //host private key, in
{
    PackInf_st Inf;
    PackUKey_st UKey;
    PackHKey_st HKey;

    Inf->ucSK = ucSK;
    Inf->ulKlen = ulKlen;
    Inf->ucCX = &ucCip[0];
    Inf->ucCY = &ucCip[32];
    UKey->ucId = ucUseridP;
    UKey->ulIdLen = ulIdLenP;
    UKey->ucHid = ucHid;
    UKey->ucPvaKeyXa = &ucUPvaKeyPack[1];
    UKey->ucPvaKeyXb = &ucUPvaKeyPack[33];
    UKey->ucPvaKeyYa = &ucUPvaKeyPack[65];
    UKey->ucPvaKeyYb = &ucUPvaKeyPack[97];

    return depk_sm9pack(Inf, UKey, HKey);
}

int penc_sm9pack_by(uchar8_t *ucCip, ulong_t *ulCiplen, //ciper, out; ciper len, out
                    uchar8_t *ucMsg, ulong_t ulMlen, //message in, msg len, in
                    ulong_t ulK1len, ulong_t ulK2len, uchar8_t encflag, //K1 len is the block enc key len, K2 Len is the hash key len
                    uchar8_t ucHid, uchar8_t *ucUseridP, ulong_t ulIdLenP, //user ID, in
                    uchar8_t *ucHPubKeyPack) //host public key, in
{
    PackInf_st Inf;
    PackUKey_st UKey;
    PackHKey_st HKey;
    unsigned char *ucMsg1;
    unsigned char *ucSK1;
    unsigned char ucC1[64];
    unsigned char *ucC2;
    unsigned char *ucC3;
	ucMsg1 = (unsigned char *)malloc(ulMlen + ulK1len);
	ucSK1 = (unsigned char *)malloc(ulMlen + ulK1len + ulK2len);
	ucC2 = (unsigned char *)malloc(ulMlen + ulK1len);
	ucC3 = (unsigned char *)malloc(ulK2len);
    memcpy(ucMsg1, ucMsg, ulMlen);
    HKey->ucPubKeyX = &ucHPubKeyPack[1];
    HKey->ucPubKeyY = &ucHPubKeyPack[33];
    UKey->ucId = ucUseridP;
    UKey->ulIdLen = ulIdLenP;
    UKey->ucHid = ucHid;
    Inf->ucMsg = ucMsg1;
    Inf->ulMlen = ulMlen;
    Inf->ucCX = &ucC1[0];
    Inf->ucCY = &ucC1[32];
    Inf->ucC2 = ucC2;
    Inf->ucC3 = ucC3;
    Inf->ulK1len = ulK1len;
    Inf->ulK2len = ulK2len;
    Inf->ucSK = ucSK1;

    penc_sm9pack(Inf, UKey, HKey, encflag);

    if(encflag){
        *ulCiplen = 64+ulMlen+ulK2len;
        memcpy(&ucCip[0], ucC1, 64);
        memcpy(&ucCip[64], ucC3, ulK2len);
        memcpy(&ucCip[64+ulK2len], ucC2, ulMlen);
    }
    else{
        *ulCiplen = ulMlen >> 5;
        *ulCiplen = *ulCiplen << 5;
        if(ulMlen&0x1f)
          *ulCiplen = *ulCiplen + 32;
        memcpy(&ucCip[0], ucC1, 64);
        memcpy(&ucCip[64], ucC3, ulK2len);
        memcpy(&ucCip[64+ulK2len], ucC2, *ulCiplen);
        *ulCiplen = *ulCiplen + 64 + ulK2len;
    }

	free(ucMsg1);
	free(ucSK1);
	free(ucC2);
	free(ucC3);
    return 0;
}


int pdec_sm9pack_by(uchar8_t *ucMsg, ulong_t *ulMlen, //message out, msg len, out
                    uchar8_t *ucCip, ulong_t ulCiplen, //ciper, in; ciper len, in
                    ulong_t ulK1len, ulong_t ulK2len, uchar8_t encflag, //K1 len is the block enc key len, K2 Len is the hash key len
                    uchar8_t ucHid, uchar8_t *ucUseridP, ulong_t ulIdLenP, //user ID, in
                    uchar8_t *ucUPvaKeyPack) //host public key, in
{
    int retflag = 0;
    ulong_t len;
    PackInf_st Inf;
    PackUKey_st UKey;
    PackHKey_st HKey;

	unsigned char *ucTmp;
    unsigned char *ucSK1;
    unsigned char *ucMsg1;
    unsigned char ucC1[64];
	unsigned char *ucC2;
    unsigned char *ucC3;

	ucTmp = (unsigned char *)malloc(3 * ulCiplen - ulK2len - 128);
	len = 0;
	ucSK1 = &ucTmp[len ];
	len = ulCiplen;
	ucMsg1 = &ucTmp[len];
	len = len + ulCiplen - ulK2len - 64;
	ucC2 = &ucTmp[len];
	len = len + ulCiplen - ulK2len - 64;
	ucC3 = &ucTmp[len];
    UKey->ucPvaKeyXa = &ucUPvaKeyPack[1];
    UKey->ucPvaKeyXb = &ucUPvaKeyPack[33];
    UKey->ucPvaKeyYa = &ucUPvaKeyPack[65];
    UKey->ucPvaKeyYb = &ucUPvaKeyPack[97];
    UKey->ucId = ucUseridP;
    UKey->ulIdLen = ulIdLenP;
    UKey->ucHid = ucHid;
    Inf->ucMsg = ucMsg1;
    Inf->ucCX = &ucC1[0];
    Inf->ucCY = &ucC1[32];
    Inf->ucC2 = ucC2;
    Inf->ucC3 = ucC3;
    Inf->ulK1len = ulK1len;
    Inf->ulK2len = ulK2len;
    Inf->ucSK = ucSK1;

    len = ulCiplen - 64 - ulK2len;
    Inf->ulMlen = len;

    memcpy(ucC1, &ucCip[0], 64);
    memcpy(ucC3, &ucCip[64], ulK2len);
    memcpy(ucC2, &ucCip[64+ulK2len], len);

    retflag = pdec_sm9pack(Inf, UKey, HKey, encflag);
	if (!encflag) //blk enc
		*ulMlen = len - ucMsg1[len - 1];
	else  //seq enc
		*ulMlen = Inf->ulMlen;
	memcpy(ucMsg, ucMsg1, *ulMlen);

	free(ucTmp);
    return retflag;
}
