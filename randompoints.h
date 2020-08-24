#ifndef RANDOMPOINTS_H
#define RANDOMPOINTS_H

#include "curvepoint_fp.h"

void get_random_r(mpz_t scalar);
void init_random_g(void);
void get_random_g(mpz_t scalar);
void clear_random_g(void);
#endif // ifdef RANDOMPOINTS_H
