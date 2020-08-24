#include <stdio.h>
#include <string.h>
#include "grade.h"

#define GRADE_TEST

int main(void)
{
    int i;
    unsigned char *ucMstPubKey;  //master public key
    unsigned char *ucMstPvaKey;  //master prviate key
    unsigned char *ucUserPvaKey;  //user prviate key
    unsigned long ulUserGrade;  //user grade
    unsigned long ulDocGrade; //doc grade

    ucMstPvaKey = (unsigned char *) malloc(32*(4*GRADE_KEY_NUM+1));
    ucMstPubKey = (unsigned char *) malloc(32*(2*GRADE_KEY_NUM+12));
    //+------------------------------
    //|init bilinear pairs
    init_grade();
    init_random_g();
    //fpe_test();//test

    //+------------------------------
    //|generate master key
    mst_key_grade(ucMstPubKey, ucMstPvaKey);

    //+------------------------------
    //|generate user key
    ulUserGrade = 1;
    ucUserPvaKey = (unsigned char *) malloc(32*4*(ulUserGrade+1));
    user_key_grade(ucUserPvaKey, //user key, output
                   ulUserGrade,//grade, input
                   ucMstPubKey,//master public key, input
                   ucMstPvaKey); //master private key, input

    //+------------------------------
    //|enc : generate an encrption key
    unsigned char *ucKeyEnc; //key generated, output
    unsigned char *ucKeyDec; //key generated, output
    unsigned long ulKeyLen; //key len, input
    unsigned char *ucDocGradeCip;//Grade ciper for sending with encrypted data, output
    unsigned char ucDocID[20] = {0x43,0x68,0x69,0x6E,0x65,0x73,0x65,0x20,0x49,0x42,0x45,0x20,0x73,0x74,0x61,0x6E,0x64,0x61,0x72,0x64};; //document ID, input
    unsigned long ulDocIDLen = 20; //document ID len, input

    ulKeyLen = 32;
    ucKeyEnc = (unsigned char *) malloc(ulKeyLen);
    ucKeyDec = (unsigned char *) malloc(ulKeyLen);
    ulDocGrade = 2;
    ucDocGradeCip = (unsigned char *) malloc(64*(ulDocGrade+1));

    enc_grade(ucKeyEnc, //key generated, output
              ulKeyLen, //key len, input
              ucDocGradeCip, //Grade ciper for sending with encrypted data, output
              ulDocGrade, //grade, input
              ucDocID, //document ID, input
              ulDocIDLen, //document ID len, input
              ucMstPubKey);//master public key, input

    dec_grade(ucKeyDec, //key generated, output
              ulKeyLen, //key len, input
              ucDocGradeCip, //Grade ciper for sending with encrypted data, output
              ulDocGrade,//doc grade, input
              ucDocID, //document ID, input
              ulDocIDLen, //document ID len, input
              ulUserGrade,//user grade, input
              ucUserPvaKey);//master public key, input

    printf("EncKey = \n");
    for(i=0; i<ulKeyLen; i++)
    {
        printf("%02x", ucKeyEnc[i]);
    }
    printf("\n");

    printf("DecKey = \n");
    for(i=0; i<ulKeyLen; i++)
    {
        printf("%02x", ucKeyDec[i]);
    }
    printf("\n");

    //+-------------------------------
    //|free ram
    clear_grade();
    clear_random_g();
}
