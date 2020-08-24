#ifndef GRADE_H
#define GRADE_H

#define GRADE_TEST

#define GRADE_NUM 3  //the max grade num of doc
#define GRADE_KEY_NUM 4

void fpe_test(void);

void init_grade(void);

void clear_grade(void);

void mst_key_grade(unsigned char *ucMstPubKey, unsigned char *ucMstPvaKey);

int user_key_grade(unsigned char *ucUserPvaKey, //user key, output
                   unsigned long ulUserGrade,//grade, input
                   unsigned char *ucMstPubKey,//master public key, input
                   unsigned char *ucMstPvaKey); //master private key, input

int enc_grade(unsigned char *ucKey, //key generated, output
              unsigned long ulKeyLen, //key len, input
              unsigned char *ucdocGradeCip, //Grade ciper for sending with encrypted data, output
              unsigned long ulDocGrade,//grade, input
              unsigned char *ucDocID, //document ID, input
              unsigned long ulDocIDLen, //document ID len, input
              unsigned char *ucMstPubKey);//master public key, input

int dec_grade(unsigned char *ucKey, //key generated, output
              unsigned long ulKeyLen, //key len, input
              unsigned char *ucdocGradeCip, //Grade ciper for sending with encrypted data, output
              unsigned long uldocGrade,//doc grade, input
              unsigned char *ucDocID, //document ID, input
              unsigned long ulDocIDLen, //document ID len, input
              unsigned long ulUserGrade,//user grade, input
              unsigned char *ucUserPvaKey);//master public key, input


#endif
