#ifndef SM9HV_H
#define SM9HV_H

#include <gmp.h>

void sm9h(mpz_t mHout, const unsigned char * cZin, unsigned long lZlen, mpz_t mOrd, unsigned char cflag);
void sm9kdf(unsigned char *ucKout, unsigned long lKlen, const unsigned char * cZin, unsigned long lZlen);
#endif
