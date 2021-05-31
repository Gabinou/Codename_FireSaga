#ifndef Q_MATH_H
#define Q_MATH_H

#include <stdint.h>
#include <math.h>
#ifdef __TINYC__
#include "tcc/tcc_libm.h"
#endif

#ifndef log2// because tcc SUCKS, does NOT DEFINE log2
#define log2(x) (log(x)/log(2.0f))
#endif

// m:-1, p:1, z:0
#define cycle2_zp(i) (i % 2)
#define cycle2_pz(i) ((i + 1) % 2)
#define cycle2_mz(i) ((i % 2) - 1)
#define cycle2_zm(i) (((i + 1) % 2) - 1)
#define cycle2_mp(i) (cycle2_mz(i) + cycle2_zp(i))
#define cycle2_pm(i) (cycle2_pz(i) + cycle2_zm(i))

#define cycle3_mzp(i) ((i % 3) - 1)
#define cycle3_zpm(i) (((i + 1) % 3) - 1)
#define cycle3_pmz(i) (((i + 2) % 3) - 1)
#define cycle3_pzm(i) (1 - (i % 3))
#define cycle3_zmp(i) (1 - ((i + 1) % 3))
#define cycle3_mpz(i) (1 - ((i + 2) % 3))

#define cycle4_mzp2(i) ((i % 4) - 1)
#define cycle4_zp2m(i) (((i + 1) % 4) - 1)
#define cycle4_p2mz(i) (((i + 2) % 4) - 1)
#define cycle4_2mzp(i) (((i + 3) % 4) - 1)
#define cycle4_mzpz(i) (cycle2_pz(i) * cycle4_mzp2(i))
#define cycle4_pzmz(i) (cycle2_mz(i) * cycle4_mzp2(i))
#define cycle4_zmzp(i) (cycle2_zp(i) * cycle4_2mzp(i))
#define cycle4_zpzm(i) (cycle2_zp(i) * cycle4_zp2m(i))

#define cycle4_mmpp(i) (cycle4_mzpz(i) + cycle4_zmzp(i))
#define cycle4_pmmp(i) (cycle4_pzmz(i) + cycle4_zmzp(i))
#define cycle4_mppm(i) (cycle4_mzpz(i) + cycle4_zpzm(i))
#define cycle4_ppmm(i) (cycle4_pzmz(i) + cycle4_zpzm(i))

#define cycle6_mzpzzz(i) (cycle3_mzp(i) * cycle2_pz(i))
#define cycle6_zzzmzp(i) (cycle3_mzp(i) * cycle2_zp(i))
#define cycle6_zzmzpz(i) (cycle3_zpm(i) * cycle2_pz(i))
#define cycle6_zpzzzm(i) (cycle3_zpm(i) * cycle2_zp(i))
#define cycle6_pzzzmz(i) (cycle3_pmz(i) * cycle2_pz(i))
#define cycle6_zmzpzz(i) (cycle3_pmz(i) * cycle2_zp(i))

#define cycle6_mzzzpz(i) (cycle3_mpz(i) * cycle2_pz(i))
#define cycle6_zpzmzz(i) (cycle3_mpz(i) * cycle2_zp(i))
#define cycle6_pzmzzz(i) (cycle3_pzm(i) * cycle2_pz(i))
#define cycle6_zzzpzm(i) (cycle3_pzm(i) * cycle2_zp(i))
#define cycle6_zzpzmz(i) (cycle3_zmp(i) * cycle2_pz(i))
#define cycle6_zmzzzp(i) (cycle3_zmp(i) * cycle2_zp(i))

#define cycle6_mppmzz(i) (cycle6_mzpzzz(i) + cycle6_zpzmzz(i))
#define cycle6_zzmppm(i) (cycle6_zzzpzm(i) + cycle6_zzmzpz(i))
#define cycle6_pmzzmp(i) (cycle6_zmzzzp(i) + cycle6_pzzzmz(i))

#define REGISTER_ENUM(a) extern a sequence_geometric_##a(a start, a destination, a geo_factor);
#include "names/template_types.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(a) extern a sequence_pingpong_##a(a current, a upper, a lower);
#include "names/template_types.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(a) extern float carmack_sqrt_##a(a in_int);
#include "names/template_types.h"
#undef REGISTER_ENUM

#define q_sqrt_int8_t carmack_sqrt_int8_t
#define q_sqrt_uint8_t carmack_sqrt_uint8_t
#define q_sqrt_int16_t carmack_sqrt_int16_t
#define q_sqrt_uint16_t carmack_sqrt_uint16_t
#define q_sqrt_int32_t carmack_sqrt_int32_t
#define q_sqrt_uint32_t carmack_sqrt_uint32_t
#define q_sqrt_int64_t carmack_sqrt_int64_t
#define q_sqrt_uint64_t carmack_sqrt_uint64_t
#define q_sqrt_float carmack_sqrt_float
#define q_sqrt_double carmack_sqrt_double

#endif /* Q_MATH_H */
