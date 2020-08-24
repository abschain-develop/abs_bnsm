#ifndef SM9GENY_H
#define SM9GENY_H

#include <stdio.h>
#include <string.h>
#include "sm9.h"
#include "sm9geny.h"
typedef	unsigned char	uchar8_t;

int sqrt_y(mpz_t y, mpz_t in);
void gen_y(uchar8_t *Ya, uchar8_t *Yb, uchar8_t *din);


#endif
