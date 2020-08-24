#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "sm9hv.h"
#include "sm3.h"


//+-----------------------------------------------
//| H1(Z, n), detail in sm9-2,p4-p5
//| flag = 0x01, H1
//| flag = 0x02, H2
//+-----------------------------------------------
void sm9h(mpz_t mHout, const unsigned char * cZin, unsigned long lZlen, mpz_t mOrd, unsigned char cflag)
{  
  unsigned long ct = 0x00000001;
  unsigned long hlen;
  double dd;
  double loutj;
  mp_limb_signed_t expdd;
  int i,j;
  unsigned char *hai;
  unsigned char *ha;
  mpz_t hmod;
  
  mpz_init(hmod);
  dd = mpz_get_d_2exp(&expdd, mOrd);
  loutj = logb(dd) + expdd;
  hlen = ceil((loutj*5)/32);
  hlen = hlen * 8;
  j = hlen>>8;
  if(hlen&0xff)
    j++;
  ha = (unsigned char *) malloc(32 * j * sizeof(unsigned char));
  hai = (unsigned char *) malloc((lZlen+5) * sizeof(unsigned char));
  
  for(i=1; i<=j; i++)
  {
    hai[0] = cflag;
    memcpy(&hai[1], cZin, lZlen);
    hai[lZlen+1] = (ct>>24)&0xff;
    hai[lZlen+2] = (ct>>16)&0xff;
    hai[lZlen+3] = (ct>>8)&0xff;
    hai[lZlen+4] = ct & 0xff;
    sm3(hai, lZlen+5, &ha[(i-1)*32]);
    ct++;	
  }
  
  if(hlen&0xff)
  {
    i = (hlen - 256*floor((double)hlen/256))/8;
    mpz_import(mHout, 32*(j-1)+i, 1, sizeof(unsigned char), 0, 0, ha);
  }
  else
  {
  	mpz_import(mHout, 32*j, 1, sizeof(unsigned char), 0, 0, ha);
  }
  
  mpz_sub_ui(hmod, mOrd, 1);
  mpz_mod(mHout, mHout, hmod);
  mpz_add_ui(mHout, mHout, 1);
  mpz_clear(hmod);
  free(ha);
  free(hai);
}

//+-----------------------------------------------
//| kdf, detail in sm9-3,p5-p6
//+-----------------------------------------------
void sm9kdf(unsigned char *ucKout, unsigned long lKlen, const unsigned char * cZin, unsigned long lZlen)
{
  unsigned long ct = 0x00000001;
  int i,j;
  unsigned char *hai;
  unsigned char *ha;

  j = lKlen>>5;
  if(lKlen&0x1f)
    j++;
  ha = (unsigned char *) malloc(32 * j * sizeof(unsigned char));
  hai = (unsigned char *) malloc((lZlen+4) * sizeof(unsigned char));

  for(i=1; i<=j; i++)
  {
    memcpy(&hai[0], cZin, lZlen);
    hai[lZlen+0] = (ct>>24)&0xff;
    hai[lZlen+1] = (ct>>16)&0xff;
    hai[lZlen+2] = (ct>>8)&0xff;
    hai[lZlen+3] = ct & 0xff;
    sm3(hai, lZlen+4, &ha[(i-1)*32]);
    ct++;
  }

  if(lKlen&0x1f)
  {
    i = lKlen - 32*floor((double)lKlen/32);
    memcpy(ucKout, ha, 32*(j-1)+i);
  }
  else
  {
    memcpy(ucKout, ha, lKlen);
  }


  free(ha);
  free(hai);
}





