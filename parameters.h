#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <gmp.h>

// Parameters used in fp.c
#define BN_P "82434016654578246444830763105245969129603161266935169637912592173415460324733" //--
#define BN_PINV32 791602219UL  //--
#define BN_PINV64 9884209501243892779UL  //--

// Parameters used in fpe.c

#if(GMP_LIMB_BITS == 32)
#define N_LIMBS 8
#elif(GMP_LIMB_BITS == 64)
#define N_LIMBS 4
#else
#error "Only 32 and 64 bit architectures are supported"
#endif

// Parameters used in fp2.c
#define ALPHA (-2) // constant coefficient in the irreducible polynomial x^2 - alpha, used to construct F_{p^2}

// Parameters used in fp6.c
#define BN_XI "41217008327289123222415381552622984564801580633467584818956296086707730162366", "0"  // xi = x
#define BN_YPMINUS1 "0", "82434016654578246438872420442344325702149582327179867092849556861979152020042"
#define BN_ZETA "5958342662901643427453578939755302545063035311436308304691" //--

// Parameters used in fp12.c
#define BN_TAU "0", "0", "0", "1", "0", "0" // constant tau used to construct F_p^12 as F_p^6[Z]/ (Z^2 - tau)
#define BN_ZPMINUS1 "0", "61965731471168946719248489071114988157816539025752347677050187216983794638583" // Z^(p-1)
#define BN_ZPMINUS1INV "0", "28559166513296912346466834407128128229200165862230698399494071135560376478091" // Z^(1-p)

// Parameters used in curve.c
#define BN_X "6917529027646912906" // parameter x used to generate the curve (see "Pairing-Friendly Elliptic Curves of Prime Order") //--
#define BN_N "82434016654578246444830763105245969129316048019845143771873730126023764135717" // prime order of E(F_p) //--
#define BN_TRACE "287113247090025866038862047391696189017" // trace of Frobenius of the curve //--
#define BN_CHI "1194458535560953672760661099480730165454741775941918265852319059048708093452971670498512929573479725250879729206442775153837708511849907342184847139097417487758930491039156179726551248488268041976762987447273743641061319867690368967455851452058791851524149789475877302537832322260110255376299627351637301723869704552404002580246136018485808526647754149898670082479637966446303210920192822710600431683543435991316573340038003237603594935976757894226828033260443835526228324195731998135946468892404607348677822626929637017831240188245007333797483529802137232707273272488046972651585918171337405147980301648783476487576456615135553498972068865523047667292712740053119304683379183240113846586869403124028425928491199800013198826480317825730064241555828005151848403179709158907047141760421962590697378015670077062238414478741143791795936828211098559280" // exponent of final exponentiation  //--
#define BN_B "5" // parameter b in the curve equation x^2 = y^3 + b

// Parameters used in points.c
#define BN_CURVEGEN "66882264091090507626018961359014674407194165142668785782120083507098121854429", "15376082189538387440900211051596494061397537732182077032922339670147106514454", "1" //--
#define BN_TWISTGEN_X "60466722995555678255063009133855966015204203660483735931208565649508950630721", "24938089202703746092011716859165112598116193077772600217517878662436766275163"  //--
#define BN_TWISTGEN_Y "10545613300819485241647916378182133973234871584468018082026845794053022018454", "75902264870848191965947054240695531212838904085804444407721102074481804421063"  //--

// Parameters used for OptAte computation in ate_optate.c
#define BN_ZETA2 "82434016654578246438872420442344325702149582327179867092849556861979152020041" // zeta^2
#define BN_Z2P "0", "82434016654578246438872420442344325702149582327179867092849556861979152020042" // Z^(2p)
#define BN_Z3P "0", "33406564957872034372781654663986859928616373163521649277556116081423418160492" // Z^(3p)

#endif
