#ifndef CRT_H
#define CRT_H

#include <math.h>
#include "SDL.h"
#include "filesystem.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846f
#endif /* M_PI */

// CRT filter of same design as Bisqwit at github.com/bisqwit/crt-filter

/* --- FORWARD DECLARATIONS --- */
struct Game;

#define LUMA_R 0.2126
#define LUMA_G 0.7152
#define LUMA_B 0.0722
#define GAMMA 2
#define SCANLINE_BRIGHT_C 0.3f
#define GAMMA_N 3
#define LANCZOS_A 2
#define BLOOM_ENUM 600
#define BLOOM_DENOM 255

struct FloatRGB {
    float *R;
    float *G;
    float *B;
};

struct CRT { /* Cathode-Ray Tube filter for pretty pixels */
    struct SDL_Surface *unfiltered;
    struct SDL_Surface *filtered;
    struct SDL_Texture *unfiltered_text;
    struct SDL_Texture *filtered_text;
    uint_fast32_t target_format;

    int_fast8_t lanczos_a;

    uint_fast32_t num_scanlines;

    uint_fast32_t input_width;
    uint_fast32_t input_height;

    uint_fast32_t intermediate_width;
    uint_fast32_t intermediate_height;

    uint_fast32_t output_height;
    uint_fast32_t output_width;

    uint_fast32_t cell_width;
    uint_fast32_t cell_blank_R;
    uint_fast32_t cell_blank_G;
    uint_fast32_t cell_blank_B;
    uint_fast32_t cell_Vblank;
    uint_fast32_t stagger;
    uint_fast32_t cell_height;
};
extern struct CRT CRT_default;

extern void CRT_Init(struct CRT *in_crt, uint_fast32_t width, uint_fast32_t height);
extern void CRT_Free(struct CRT *in_crt);

/* CRT_FILTER */
extern void CRT_Filter(struct CRT *in_crt, SDL_Texture *target,
                       SDL_Renderer *renderer);

extern void CRT_Target2Surface(struct CRT *in_crt);

extern void CRT_Shadow_Mask(u8 *mask, uint_fast32_t size_x,
                            uint_fast32_t size_y,
                            u8 cell_height, u8 cell_width, u8 Hblank,
                            u8 Vblank,
                            u8 stagger, uint_fast32_t x_offset, uint_fast32_t y_offset);
extern void CRT_Shadow_Filter(u8 *mask, float *color, uint_fast32_t size_x,
                              uint_fast32_t size_y);

/* pixels/floats conversion */
extern void CRT_Pixels2Floats(struct FloatRGB *floatrgb, SDL_Surface *surface,
                              uint_fast32_t width, uint_fast32_t height);
extern SDL_Surface *CRT_Floats2Pixels(struct FloatRGB *floatrgb, uint_fast32_t width,
                                      uint_fast32_t height);

extern float Scanline_Brightness(float c, float n);

/* LANCZOS FILTER */
extern float Lanczos_Kernel(float x, int_fast8_t a);
extern void Lanczos_Filter_1D(float *samples, float *filtered,
                              uint_fast32_t size_samples,
                              uint_fast32_t size_filtered, int_fast8_t a);
extern void Lanczos_Filter_2D(float *samples, float *filtered,
                              uint_fast32_t size_samples_x,
                              uint_fast32_t size_samples_y, uint_fast32_t size_filtered_x,
                              uint_fast32_t size_filtered_y,
                              int_fast8_t a);

/* NEAREST FILTER */
extern void Nearest_Scanline_Filter_2D(float *samples, float *filtered,
                                       uint_fast32_t size_samples_x, uint_fast32_t size_samples_y, uint_fast32_t size_filtered_x,
                                       uint_fast32_t size_filtered_y);

/* GAUSSIAN BLUR */
extern void blur( float *input, float *output, float *temp, i32 w,
                  i32 h,
                  float sigma, i32 n_boxes);

extern void gaussBlur_4(float *scl, float *tcl, uint_fast32_t width,
                        uint_fast32_t height,
                        float r, uint_fast32_t n);
extern void boxesForGauss(uint_fast32_t *sizes, float sigma, uint_fast32_t n);
extern void boxBlur_4(float *scl, float *tcl, uint_fast32_t width, uint_fast32_t height,
                      float r);
extern void boxBlurT_4(float *scl, float *tcl, uint_fast32_t width,
                       uint_fast32_t height,
                       float r);
extern void boxBlurH_4(float *scl, float *tcl, uint_fast32_t width,
                       uint_fast32_t height,
                       float r);

/* BLOOM */
extern void CRT_Bloom(float *color, uint_fast32_t width, uint_fast32_t height,
                      float radius);
extern void CRT_Bloom_Correction_Denom(float *color, uint_fast32_t size);
extern void CRT_Bloom_Correction_Enum(float *color, uint_fast32_t size);
extern void CRT_Bloom_Correction(float *color, uint_fast32_t size);
extern void CRT_Gamma_Correction(float *color, uint_fast32_t size);
extern void CRT_Gamma_Uncorrection(float *color, uint_fast32_t size);

/* CLAMPING */
extern void CRT_Clamping(float *R, float *G, float *B, uint_fast32_t width,
                         uint_fast32_t height);

#endif /* CRT_H */