
#include "q_math.h"

// sequence_geometric: Geometrically decreasing integer/float
// Ex: distance/geo_factor -> distance/geo_factor**2 -> distance/geo_factor**3
// sign*distance more elegant than std_abs()
#define REGISTER_ENUM(a) a sequence_geometric_##a(a start, a destination, a geo_factor) {\
    a distance = start - destination;\
    a sign = (a)copysign(1, distance);\
    a out = (sign * (distance / geo_factor) < 1) ? sign : (distance / geo_factor);\
    return (out);\
}
#include "names/template_types_int.h"
#undef REGISTER_ENUM

// sequence_pingpong: oscillating integer sequence. upper is excluded.
// modulo: x % 2 = 0,1,2,0,1,2,0...for x++
// pingpong(x, 2, 0) = 0,1,2,1,0,1,2... for x++
#define REGISTER_ENUM(a) a sequence_pingpong_##a(a current, a upper, a lower) {\
    a mod_factor = (2 * (upper - lower) - 2);\
    a term1 = mod_factor - (current % mod_factor);\
    a term2 = current % mod_factor;\
    return (fmin(term1, term2) + lower);\
}
#include "names/template_types_int.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(a) float carmack_sqrt_##a(a in_int) {\
    const float x2 = in_int * 0.5f;\
    const float threehalfs = 1.5f;\
    union {\
        float f;\
        unsigned long i;\
    } conv  = { .f = in_int };\
    conv.i  = 0x5f3759df - (conv.i >> 1);\
    conv.f  *= (threehalfs - (x2 * conv.f * conv.f));\
    return conv.f;\
}
#include "names/template_types.h"
#undef REGISTER_ENUM
