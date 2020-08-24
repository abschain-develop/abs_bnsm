#include <gmp.h>
#include <time.h>

#include "randompoints.h"
#include "curve.h"

gmp_randstate_t state_g;

void get_random_r(mpz_t scalar)
{
    gmp_randstate_t state;
    gmp_randinit_default(state);
    unsigned long int seed;
    seed = (unsigned long int)time(NULL);
    gmp_randseed_ui(state, seed);

    do
        mpz_urandomm(scalar, state, sm_n);
    while(!mpz_cmp_ui(scalar, 0));

    gmp_randclear(state);
}

void init_random_g(void)
{
    gmp_randinit_default(state_g);
    unsigned long int seed;
    seed = (unsigned long int)time(NULL);
    gmp_randseed_ui(state_g, seed);
}

void get_random_g(mpz_t scalar)
{
    do
        mpz_urandomm(scalar, state_g, sm_n);
    while(!mpz_cmp_ui(scalar, 0));
}

void clear_random_g(void)
{
    gmp_randclear(state_g);
}
