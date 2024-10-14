#include "crt.h"

void matrix_print_float(float *array, size_t row_len, size_t col_len) {
    for (size_t row = 0; row < row_len; row++) {
        // for (size_t col = 0; col < col_len; col++)
        // array[row * col_len + col];
        SDL_Log("%zu", row);
        SDL_Log("\n");
    }
}

struct CRT CRT_default = {
    .unfiltered = NULL,
    .filtered = NULL,
    .unfiltered_text = NULL,
    .filtered_text = NULL,

    .target_format = SDL_PIXELFORMAT_ABGR8888,

    .num_scanlines = 6000,

    .input_width = 1600,
    .input_height = 800,

    .intermediate_width = 1600,
    .intermediate_height = 800,

    .output_width = 1600,
    .output_height = 800,

    // Trinitron settings
    .cell_width = 2,
    .cell_blank_R = 1,
    .cell_blank_G = 1,
    .cell_blank_B = 2,
    .cell_Vblank = 1,
    .stagger = 3,
    .cell_height = 5,
};

void CRT_Init(struct CRT *in_crt, uint_fast32_t width, uint_fast32_t height) {
    SDL_Log("CRT_Init");
    // ideally intermediate_height = integer * num_scanlines
    // ideally intermediate_width = integer * input_width
    in_crt->input_width = width;
    in_crt->input_height = height;
    in_crt->intermediate_width = in_crt->input_width * (in_crt->cell_width * 3 +
                                                        in_crt->cell_blank_R +
                                                        in_crt->cell_blank_G + in_crt->cell_blank_B);
    in_crt->num_scanlines = in_crt->input_height;
    in_crt->intermediate_height = in_crt->num_scanlines * 4;
    in_crt->output_width = in_crt->intermediate_width;
    in_crt->output_height = in_crt->intermediate_height;
}

void CRT_Pixels2Floats(struct FloatRGB *floatrgb, SDL_Surface *surface,
                       uint_fast32_t width,
                       uint_fast32_t height) {
    SDL_Log("CRT_Pixels2Floats");
    SDL_assert(surface);
    SDL_assert(surface->w == width);
    SDL_LockSurface(surface);
    SDL_assert(surface->format->format == SDL_PIXELFORMAT_ABGR8888);
    uint32_t *pixels = (uint32_t *)surface->pixels;
    SDL_assert(surface->pixels != NULL);
    SDL_assert(pixels != NULL);
    uint32_t pixel32;
    uint8_t *pixel8;
    for (uint_fast32_t i = 0; i < (width * height); i++) {
        pixel32 = (uint_fast32_t)pixels[i];
        pixel8 = (uint8_t *)&pixel32;
        floatrgb->R[i] = (float)pixel8[LITTLE_ENDIAN_ARGB8888_R] / 0xFF;
        floatrgb->G[i] = (float)pixel8[LITTLE_ENDIAN_ARGB8888_G] / 0xFF;
        floatrgb->B[i] = (float)pixel8[LITTLE_ENDIAN_ARGB8888_B] / 0xFF;
    }
    SDL_UnlockSurface(surface);
}

SDL_Surface *CRT_Floats2Pixels(struct FloatRGB *floatrgb, uint_fast32_t width,
                               uint_fast32_t height) {
    SDL_Log("CRT_Floats2Pixels");
    uint32_t *pixels = (uint32_t *)SDL_malloc(height * width * sizeof(*pixels));
    SDL_assert(pixels != NULL);
    uint32_t pixel32;
    uint8_t *pixel8 = (uint8_t *)&pixel32;
    pixel8[LITTLE_ENDIAN_ARGB8888_A] = 0xFF;
    for (uint_fast32_t i = 0; i < (width * height); i++) {
        floatrgb->R[i] = floatrgb->R[i] > 1.0f ? 1.0f : floatrgb->R[i];
        floatrgb->G[i] = floatrgb->G[i] > 1.0f ? 1.0f : floatrgb->G[i];
        floatrgb->B[i] = floatrgb->B[i] > 1.0f ? 1.0f : floatrgb->B[i];
        floatrgb->R[i] = floatrgb->R[i] < 0.0f ? 0.0f : floatrgb->R[i];
        floatrgb->G[i] = floatrgb->G[i] < 0.0f ? 0.0f : floatrgb->G[i];
        floatrgb->B[i] = floatrgb->B[i] < 0.0f ? 0.0f : floatrgb->B[i];
        pixel8[LITTLE_ENDIAN_ARGB8888_R] = (uint8_t)(floatrgb->R[i] * 0xFF);
        pixel8[LITTLE_ENDIAN_ARGB8888_G] = (uint8_t)(floatrgb->G[i] * 0xFF);
        pixel8[LITTLE_ENDIAN_ARGB8888_B] = (uint8_t)(floatrgb->B[i] * 0xFF);
        pixel32 = *(uint32_t *)pixel8;
        pixels[i] = pixel32;
    }
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormatFrom(pixels, width, height,
                                                              SDL_BITSPERPIXEL(SDL_PIXELFORMAT_ABGR8888),
                                                              width * SDL_BYTESPERPIXEL(SDL_PIXELFORMAT_ABGR8888),
                                                              SDL_PIXELFORMAT_ABGR8888);
    SDL_assert(surface != NULL);
    return (surface);
}

void CRT_Shadow_Filter(u8 *mask, float *color, uint_fast32_t size_x,
                       uint_fast32_t size_y) {
    SDL_Log("CRT_Shadow_Filter");
    for (uint_fast32_t j = 0; j < size_y; j++) {
        for (uint_fast32_t i = 0; i < size_x; i++)
            color[i + j * size_x] *= (float)mask[i + j * size_x];
    }
}

void CRT_Shadow_Mask(u8 *mask, uint_fast32_t size_x, uint_fast32_t size_y,
                     u8 cell_height, u8 cell_width, u8 Hblank,
                     u8 Vblank,
                     u8 stagger, uint_fast32_t x_offset, uint_fast32_t y_offset) {
    /* mask should be set to 0 everywhere. */
    SDL_Log("CRT_Shadow_Mask");
    for (uint_fast32_t j = 0; j < size_y; j++) {
        for (uint_fast32_t i = 0; i < size_x; i++) {
            b32 tostagger = ((i + x_offset) % (2 * (cell_width + Hblank))) >=
                            (cell_width + Hblank - 1);
            b32 horiz = ((i + x_offset) % (cell_width + Hblank)) < cell_width;
            b32 vert = ((j + y_offset + tostagger * stagger) % (cell_height + Vblank)) < cell_height;
            mask[i + j * size_x] = vert && horiz;
        }
    }
}

extern void Nearest_Scanline_Filter_2D(float *samples, float *filtered,
                                       uint_fast32_t size_samples_x, uint_fast32_t size_samples_y, uint_fast32_t size_filtered_x,
                                       uint_fast32_t size_filtered_y) {
    SDL_Log("Nearest_Scanline_Filter_2D");
    // cols and rows separate?
    for (uint_fast32_t jy = 0; jy < size_filtered_y; jy++) {
        float y = (float)size_samples_y / (float)size_filtered_y * (float)jy;
        uint_fast32_t y_nearest = (uint_fast32_t)floorf(y);
        float fraction = (y - (float)y_nearest);
        // SDL_Log("%f", fraction);
        float bright = Scanline_Brightness(SCANLINE_BRIGHT_C, fraction);
        for (uint_fast32_t jx = 0; jx < size_filtered_x; jx++) {
            float x = (float)size_samples_x / (float)size_filtered_x * (float)jx;
            uint_fast32_t x_nearest = (uint_fast32_t)floorf(x);
            filtered[jx + jy * size_filtered_x] = bright * samples[x_nearest + y_nearest *
                                                                   size_samples_x];
        }
    }
}

float Lanczos_Kernel(float x, int_fast8_t a) {
    // SDL_Log("Lanczos_Kernel");
    // kernel supposed to be 0 if |x| > a but Lanczos_Filter does not input out of bounds x
    float out = 1.0f;
    float dist = fabsf(x) - fabsf((float)a);
    if (x != 0.0f)
        out = a * sinf(x * M_PI) * sinf(x * M_PI / a) / (x * x * M_PI * M_PI);
    else if (x == 0.0f)
        out = 1.0f;
    return (out);
}

void Lanczos_Filter_2D_T(float *samples, float *filtered, uint_fast32_t size_samples_x,
                         uint_fast32_t size_samples_y, uint_fast32_t size_filtered_x,
                         uint_fast32_t size_filtered_y,
                         int_fast8_t a) {
    SDL_Log("Lanczos_Filter_2D");
    float *filtered_cols = SDL_calloc(size_samples_x * size_filtered_y, sizeof(*filtered_cols));
    memset(filtered, 0, size_filtered_x * size_filtered_y * sizeof(*filtered));
    float delta_y = (float)size_samples_y / (float)size_filtered_y;
    float delta_x = (float)size_samples_x / (float)size_filtered_x;
    /* Separate Rows and Cols is much faster*/
    /* COLS */
    for (uint_fast32_t jy = 0; jy < size_filtered_y; jy++) {
        float y = (float)jy * delta_y;
        uint_fast32_t y_floor = (uint_fast32_t)floorf(y);
        uint_fast32_t min_y = (y_floor < a) ? 0 : y_floor - a + 1;
        uint_fast32_t max_y = (y_floor > size_samples_y - a - 1) ? size_samples_y :
                              (y_floor + a + 1);
        for (uint_fast32_t jx = 0; jx < size_samples_x; jx++) {
            for (uint_fast32_t j = min_y; j < max_y; j++) {
                i32 clamped = j < 0 ? 0 : j;
                clamped = clamped > size_samples_y ?  size_samples_y : clamped;
                filtered_cols[jx + jy * size_samples_x] += samples[jx + clamped * size_samples_x] *
                                                           Lanczos_Kernel(
                                                                   y - j, a);
            }
        }
    }
    /* ROWS */
    for (uint_fast32_t jx = 0; jx < size_filtered_x; jx++) {
        float x = (float)jx * delta_x;
        uint_fast32_t x_floor = (uint_fast32_t)floorf(x);
        i32 min_x = x_floor - a + 1;
        i32 max_x = x_floor + a + 1;
        for (uint_fast32_t jy = 0; jy < size_filtered_y; jy++) {
            for (i32 i = min_x; i < max_x; i++) {
                i32 clamped = i < 0 ? 0 : i;
                clamped = clamped > size_samples_x ?  size_samples_x : clamped;
                filtered[jx + jy * size_filtered_x] += filtered_cols[clamped + jy * size_samples_x] *
                                                       Lanczos_Kernel(x - i, a);
            }
        }
    }
    SDL_free(filtered_cols);
}

void Lanczos_Filter_2D(float *samples, float *filtered, uint_fast32_t size_samples_x,
                       uint_fast32_t size_samples_y, uint_fast32_t size_filtered_x,
                       uint_fast32_t size_filtered_y,
                       int_fast8_t a) {
    SDL_Log("Lanczos_Filter_2D");
    float *filtered_rows = SDL_calloc(size_filtered_x * size_samples_y, sizeof(*filtered_rows));
    memset(filtered, 0, size_filtered_x * size_filtered_y * sizeof(*filtered));
    /* Separate Rows and Cols is much faster*/
    /* ROWS */
    float delta_y = (float)(size_samples_y - 1) / (float)(size_filtered_y - 1);
    float delta_x = (float)(size_samples_x - 1) / (float)(size_filtered_x - 1);
    for (uint_fast32_t jx = 0; jx < size_filtered_x; jx++) {
        float x = (float)jx * delta_x;
        uint_fast32_t x_floor = (uint_fast32_t)floorf(x);
        i32 min_x = x_floor - a + 1;
        i32 max_x = x_floor + a + 1;
        for (uint_fast32_t jy = 0; jy < size_samples_y; jy++) {
            for (i32 i = min_x; i < max_x; i++) {
                i32 clamped = i < 0 ? 0 : i;
                clamped = clamped > size_samples_x ?  size_samples_x : clamped;
                filtered_rows[jx + jy * size_filtered_x] += samples[clamped + jy * size_samples_x] *
                                                            Lanczos_Kernel(
                                                                    x - i, a);
            }
        }
    }
    /* COLS */
    for (uint_fast32_t jy = 0; jy < size_filtered_y; jy++) {
        float y = (float)jy * delta_y;
        uint_fast32_t y_floor = (uint_fast32_t)floorf(y);
        uint_fast32_t min_y = (y_floor < a) ? 0 : y_floor - a + 1;
        uint_fast32_t max_y = (y_floor > size_samples_y - a - 1) ? size_samples_y :
                              (y_floor + a + 1);
        for (uint_fast32_t jx = 0; jx < size_filtered_x; jx++) {
            for (uint_fast32_t j = min_y; j < max_y; j++) {
                i32 clamped = j < 0 ? 0 : j;
                clamped = clamped > size_samples_x ?  size_samples_x : clamped;
                filtered[jx + jy * size_filtered_x] += filtered_rows[jx + clamped * size_filtered_x] *
                                                       Lanczos_Kernel(y - j, a);
            }
        }
    }
    SDL_free(filtered_rows);
}

void Lanczos_Filter_1D(float *samples, float *filtered, uint_fast32_t size_samples,
                       uint_fast32_t size_filtered, int_fast8_t a) {
    SDL_Log("CRT_dest2Surface");
    memset(filtered, 0, size_filtered * sizeof(*filtered));
    for (uint_fast32_t j = 0; j < size_filtered; j++) {
        /* x is interpolated abscissa, j is associated index in filtered*/
        float x = j * size_samples / size_filtered;
        uint_fast32_t x_floor = (uint_fast32_t)floorf(x);
        uint_fast32_t min = x_floor - a + 1;
        uint_fast32_t max = x_floor + a + 1;
        for (uint_fast32_t i = min; i <= max; i++) {
            i32 clamped = i < 0 ? 0 : i;
            clamped = clamped > size_samples ?  size_samples : clamped;
            filtered[j] += samples[clamped] * Lanczos_Kernel(x - i, a);
        }
    }
}

float Scanline_Brightness(float c, float n) {
    // SDL_Log("Scanline_Brightness");
    /* c = 0.3 is good according to Bisqwit */
    float factor = -0.5 * (n - 0.5) * (n - 0.5) / c / c;
    return (exp(factor));
}

/* --- BLOOM --- */
void CRT_Bloom(float *color, uint_fast32_t width, uint_fast32_t height, float radius) {
    SDL_Log("CRT_Bloom");
    // for Gamma == 2
    float *copy = SDL_malloc(width * height * sizeof(*copy));
    memcpy(copy, color, width * height * sizeof(*copy));
    float *copy_blurred = SDL_malloc(width * height * sizeof(*copy));
    float *temp = SDL_malloc(width * height * sizeof(*copy));
    CRT_Bloom_Correction_Enum(copy, width * height);
    blur(copy, copy_blurred, temp, width, height, radius, GAMMA_N);
    // gaussBlur_4(copy, copy_blurred, width, height, radius, GAMMA_N);
    CRT_Bloom_Correction_Denom(copy, width * height);
    CRT_Gamma_Correction(color, width * height);
    for (uint_fast32_t j = 0; j < height; j++) {
        for (uint_fast32_t i = 0; i < width; i++)
            color[i + j * width] += copy_blurred[i + j * width];
    }
    SDL_free(copy);
    SDL_free(copy_blurred);
}

void CRT_Bloom_Correction_Denom(float *color, uint_fast32_t size) {
    SDL_Log("CRT_Bloom_Correction_Enum");
    // for Gamma == 2
    for (uint_fast32_t j = 0; j < size; j++)
        color[j] = color[j] * color[j] / BLOOM_DENOM;
}

void CRT_Bloom_Correction_Enum(float *color, uint_fast32_t size) {
    SDL_Log("CRT_Bloom_Correction_Enum");
    // for Gamma == 2
    for (uint_fast32_t j = 0; j < size; j++)
        color[j] = color[j] * color[j] * BLOOM_ENUM;
}

void CRT_Bloom_Correction(float *color, uint_fast32_t size) {
    SDL_Log("CRT_Bloom_Correction");
    // for Gamma == 2
    for (uint_fast32_t j = 0; j < size; j++)
        color[j] = color[j] * color[j] * BLOOM_ENUM / BLOOM_DENOM;
}

void CRT_Gamma_Correction(float *color, uint_fast32_t size) {
    SDL_Log("CRT_Bloom_Correction");
    // for Gamma == 2
    for (uint_fast32_t j = 0; j < size; j++)
        color[j] = color[j] * color[j] ;
}

void CRT_Gamma_Uncorrection(float *color, uint_fast32_t size) {
    SDL_Log("CRT_Gamma_Uncorrection");
    // for Gamma == 2
    for (uint_fast32_t j = 0; j < size; j++)
        color[j] = sqrtf(color[j]);
}

/* CLAMPING */
void CRT_Clamping(float *R, float *G, float *B, uint_fast32_t width,
                  uint_fast32_t height) {
    SDL_Log("CRT_Clamping");
    float saturation, temp_sat, temp_value;
    for (uint_fast32_t j = 0; j < height; j++) {
        for (uint_fast32_t i = 0; i < width; i++) {
            float luma = LUMA_R * R[i + j * width] + LUMA_G * G[i + j * width] + LUMA_B * B[i + j *
                         width];
            if (luma < 0.0f)
                saturation = 0.0f;
            else if (luma > 1.0f)
                saturation = 1.0f;
            else {
                saturation = 1.0f;
                if (R[i + j * width] > 1.0f) {
                    temp_sat = (luma - 1.0f) / (luma - R[i + j * width]);
                    saturation = saturation < temp_sat ? saturation : temp_sat;
                } else if (R[i + j * width] < 0.0f) {
                    temp_sat = luma / (luma - R[i + j * width]);
                    saturation = saturation < temp_sat ? saturation : temp_sat;
                }
                if (G[i + j * width] > 1.0f) {
                    temp_sat = (luma - 1.0f) / (luma - G[i + j * width]);
                    saturation = saturation < temp_sat ? saturation : temp_sat;
                } else if (G[i + j * width] < 0.0f) {
                    temp_sat = luma / (luma - G[i + j * width]);
                    saturation = saturation < temp_sat ? saturation : temp_sat;
                }
                if (B[i + j * width] > 1.0f) {
                    temp_sat = (luma - 1.0f) / (luma - B[i + j * width]);
                    saturation = saturation < temp_sat ? saturation : temp_sat;
                } else if (B[i + j * width] < 0.0f) {
                    temp_sat = luma / (luma - B[i + j * width]);
                    saturation = saturation < temp_sat ? saturation : temp_sat;
                }
            }
            temp_value = (R[i + j * width] - luma) * (saturation + luma);
            temp_value = temp_value > 0.0f ? temp_value : 0.0f;
            temp_value = temp_value < 1.0f ? temp_value : 1.0f;
            R[i + j * width] = temp_value;
            temp_value = (G[i + j * width] - luma) * (saturation + luma);
            temp_value = temp_value > 0.0f ? temp_value : 0.0f;
            temp_value = temp_value < 1.0f ? temp_value : 1.0f;
            G[i + j * width] = temp_value;
            temp_value = (B[i + j * width] - luma) * (saturation + luma);
            temp_value = temp_value > 0.0f ? temp_value : 0.0f;
            temp_value = temp_value < 1.0f ? temp_value : 1.0f;
            B[i + j * width] = temp_value;
        }
    }
}

/* --- GAUSSIAN FILTER --- */
// NOTE: BORH BLUR ALGORITHMS WORK FOR VALUES SCALED ABOVE 1.0  MULTIP^LY By 600 to prepare for bloom
// Adapted from Bisqwit:
/* blur(): Really fast O(n) gaussian blur algorithm (gaussBlur_4)
 * Converted to C by Gabriel Taillon
 * Adapted by Bisqwit aka Joel Yliluoma  https://github.com/bisqwit/crt-filter
 * Original implementation Ivan Kuckir with ideas from Wojciech Jarosz
 * from http://blog.ivank.net/fastest-gaussian-blur.html
 *
 * input:  The two-dimensional array of input signal. Must contain w*h elements.
 * output: Where the two-dimensional array of blurred signal will be written
 * temp:   Another array, for temporary use. Same size as input and output.
 * w:      Width of array
 * h:      Height of array.
 * sigma:  Blurring kernel size. Must be smaller than w and h.
 * n_boxes: Controls the blurring quality. 1 = box filter. 3 = pretty good filter.
 *          Higher number = diminishingly better results, but linearly slower.
 * float: Type of elements. Should be integer type.
 */
void blur( float *input, float *output, float *temp, i32 w,
           i32 h,
           float sigma, i32 n_boxes) {
    float wIdeal = sqrtf((12.0f * sigma * sigma / n_boxes) +
                         1.0f); // Ideal averaging filter width
    uint_fast32_t wl = (i32)wIdeal;
    if ((wl % 2) == 0)
        wl--;
    i32 wu = wl + 2;
    float mIdeal = (12 * sigma * sigma - n_boxes * wl * wl - 4 * n_boxes * wl - 3 * n_boxes) /
                   (-4.0f * wl - 4);
    i32 m = floorf(mIdeal);
    float *data = input;
    for (i32 n = 0; n < n_boxes; ++n) {
        i32 r = ((n < m ? wl : wu) - 1) / 2; // IDK should this be float?
        // boxBlur_4:
        float iarr = 1.0f / (r + r + 1);
        // boxBlurH_4 (blur horizontally for each row):
        float *scl = data;
        float *tcl = temp;
        for (i32 i = 0; i < h; ++i) {
            i32 ti = i * w, li = ti, ri = ti + r;
            i32 fv = scl[ti], lv = scl[ti + w - 1];
            int val = 0;
            for (i32 j = 0; j < r; j++)
                val += scl[ti + j];
            val += (r + 1) * fv;
            for (i32 j = 0  ; j <= r ; j++) {
                val += scl[ri++] - fv;
                tcl[ti++] = floorf(((float)val * iarr));
            }
            for (i32 j = r + 1; j < w - r; j++) {
                val += scl[ri++] - scl[li++];
                tcl[ti++] = floorf(((float)val * iarr));
            }
            for (i32 j = w - r; j < w  ; j++) {
                val += lv - scl[li++];
                tcl[ti++] = floorf(((float)val * iarr));
            }
        }
        // boxBlurT_4 (blur vertically for each column)
        scl = temp;
        tcl = output;
        for (i32 i = 0; i < w; ++i) {
            i32 ti = i, li = ti, ri = ti + r * w;
            i32 fv = scl[ti], lv = scl[ti + w * (h - 1)];
            int val = 0;
            for (i32 j = 0; j < r;  ++j)
                val += scl[ti + j * w];
            val += (r + 1) * fv;
            for (i32 j = 0; j <= r; ++j)    {
                val += scl[ri] - fv;
                tcl[ti] = floorf(((float)val * iarr));
                ri += w;
                ti += w;
            }
            for (i32 j = r + 1; j < h - r; ++j) {
                val += scl[ri] - scl[li];
                tcl[ti] = floorf(((float)val * iarr));
                li += w;
                ri += w;
                ti += w;
            }
            for (i32 j = h - r; j < h; ++j)   {
                val += lv - scl[li];
                tcl[ti] = floorf(((float)val * iarr));
                li += w;
                ti += w;
            }
        }
        data = output;
    }
}

// from: https://blog.ivank.net/fastest-gaussian-blur.html
void gaussBlur_4(float *src, float *dest, uint_fast32_t width, uint_fast32_t height,
                 float radius,
                 uint_fast32_t n) {
    SDL_Log("gaussBlur_4 %lu %lu", width, height);
    // SDL_Log("n %d", n);
    uint_fast32_t *sizes = SDL_malloc(n * sizeof(*sizes));
    boxesForGauss(sizes, radius, n);
    float radiuss = (sizes[0] - 1.0f) / 2.0f;
    boxBlur_4(src, dest, width, height, radiuss);
    radiuss = (sizes[1] - 1.0f) / 2.0f;
    boxBlur_4(dest, src, width, height, radiuss);
    radiuss = (sizes[2] - 1.0f) / 2.0f;
    boxBlur_4(src, dest, width, height, radiuss);
}

void boxesForGauss(uint_fast32_t *sizes, float sigma, uint_fast32_t n) {
    SDL_Log("boxesForGauss");
    // sizes has length n
    // float wideal = q_sqrt_float((12.0f * sigma * sigma / (float)n) + 1.0f); // ideal averaging filter width
    float wideal = (float)sqrtf((12.0f * sigma * sigma / (float)n) +
                                1.0f); // ideal averaging filter width
    uint_fast32_t wl = (uint_fast32_t)floorf(wideal);
    if ((wl % 2) == 0)
        wl--;
    uint_fast32_t wu = wl + 2;
    float mideal = (12.0f * sigma * sigma - n * wl * wl - 4 * n * wl - 3 * n) / (-4 * wl - 4);
    uint_fast32_t m = (uint_fast32_t)floorf(mideal);
    for (size_t i = 0; i < n; i++)
        sizes[i] = i < m ? wl : wu;
    // SDL_Log("sizes %d %d %d", sizes[0], sizes[1], sizes[2]);
}

void boxBlur_4(float *src, float *dest, uint_fast32_t width, uint_fast32_t height,
               float radius) {
    SDL_Log("boxBlur_4");
    for (uint_fast32_t i = 0; i < (width * height); i++)
        dest[i] = src[i];
    boxBlurH_4(dest, src, width, height, radius);
    boxBlurT_4(src, dest, width, height, radius);
}

void boxBlurH_4(float *src, float *dest, uint_fast32_t width, uint_fast32_t height,
                float radius) {
    SDL_Log("boxBlurH_4");
    uint_fast32_t intradius = (uint_fast32_t)radius;
    float iarr = 1.0f / (radius + radius + 1.0f);
    for (uint_fast32_t i = 0; i < height; i++) {
        uint_fast32_t ti = i * width;
        uint_fast32_t li = ti;
        uint_fast32_t ri = ti + (uint_fast32_t)radius;
        float fv = src[ti];
        float lv = src[ti + width - 1];
        float val = (radius + 1) * fv;
        for (uint_fast32_t j = 0; j < intradius; j++)
            val += src[ti + j];
        for (uint_fast32_t j = 0; j <= intradius ; j++) {
            val += src[ri++] - fv;
            dest[ti++] = floorf(val * iarr);
        }
        for (uint_fast32_t j = (intradius + 1); j < (uint_fast32_t)((float)width - radius); j++) {
            val += src[ri++] - src[li++];
            dest[ti++] = floorf(val * iarr);
        }
        for (uint_fast32_t j = (uint_fast32_t)((float)width - radius); j < width; j++) {
            val += lv - src[li++];
            dest[ti++] = floorf(val * iarr);
        }
    }
}

void boxBlurT_4(float *src, float *dest, uint_fast32_t width, uint_fast32_t height,
                float radius) {
    SDL_Log("boxBlurT_4");
    float iarr = 1.0f / (radius + radius + 1.0f);
    for (uint_fast32_t  i = 0; i < width; i++) {
        uint_fast32_t ti = i;
        uint_fast32_t li = ti;
        uint_fast32_t ri = ti + (uint_fast32_t)(radius * (float)width);
        float fv = src[ti];
        float lv = src[ti + width * (height - 1)];
        float val = (radius + 1.0f) * fv;
        for (uint_fast32_t j = 0; j < radius; j++)
            val += src[ti + j * width];
        for (uint_fast32_t j = 0  ; j <= radius ; j++) {
            val += src[ri] - fv;
            dest[ti] = floorf(val * iarr);
            ri += width;
            ti += width;
        }
        for (uint_fast32_t j = radius + 1; j < height - radius; j++) {
            val += src[ri] - src[li];
            dest[ti] = floorf(val * iarr);
            li += width;
            ri += width;
            ti += width;
        }
        for (uint_fast32_t j = height - radius; j < height  ; j++) {
            val += lv - src[li];
            dest[ti] = floorf(val * iarr);
            li += width;
            ti += width;
        }
    }
}

/* Filtering Algorithm */
void CRT_Filter(struct CRT *in_crt, SDL_Texture *render_target,
                SDL_Renderer *renderer) {
    SDL_Log("CRT_Filter");
    // MUCH too slow to be used realistically.
    SDL_assert(render_target != NULL);
    in_crt->unfiltered_text = render_target;
    // in_crt->unfiltered = Filesystem_TexturetoSurface(renderer,
    // in_crt->unfiltered_text,
    // in_crt->target_format);
    Filesystem_Texture_Dump("CRT_Unfiltered.png", renderer, in_crt->unfiltered_text,
                            in_crt->target_format, render_target);
    // Convert RGB to float
    struct FloatRGB input_floatrgb;
    input_floatrgb.R = SDL_calloc(in_crt->input_width * in_crt->input_height,
                                  sizeof(*input_floatrgb.R));
    input_floatrgb.G = SDL_calloc(in_crt->input_width * in_crt->input_height,
                                  sizeof(*input_floatrgb.G));
    input_floatrgb.B = SDL_calloc(in_crt->input_width * in_crt->input_height,
                                  sizeof(*input_floatrgb.B));
    SDL_assert(in_crt->input_width == in_crt->unfiltered->w);
    SDL_assert(in_crt->input_height == in_crt->unfiltered->h);
    SDL_assert(in_crt->unfiltered->pitch == in_crt->unfiltered->w * SDL_BYTESPERPIXEL(
                       in_crt->unfiltered->format->format));
    CRT_Pixels2Floats(&input_floatrgb, in_crt->unfiltered, in_crt->input_width,
                      in_crt->input_height);
    /* TESTING PIXELS2FLOATS -> OKAY */
    // in_crt->filtered = CRT_Floats2Pixels(&input_floatrgb, in_crt->input_width, in_crt->input_height);
    // SDL_SaveBMP(in_crt->filtered, "CRT_PIXELS.png");
    // Un - gamma correction  UNNECESSARY ?
    CRT_Gamma_Uncorrection(input_floatrgb.R, in_crt->input_width * in_crt->input_height);
    CRT_Gamma_Uncorrection(input_floatrgb.G, in_crt->input_width * in_crt->input_height);
    CRT_Gamma_Uncorrection(input_floatrgb.B, in_crt->input_width * in_crt->input_height);
    // /* TESTING GAMMA -> AS EXPECTED. */
    // in_crt->filtered = CRT_Floats2Pixels(&input_floatrgb, in_crt->input_width, in_crt->input_height);
    // SDL_SaveBMP(in_crt->filtered, "CRT_UNGAMMA.png");
    // Scale to scanline count using lanczos
    struct FloatRGB scanline_floatrgb;
    scanline_floatrgb.R = SDL_malloc(in_crt->input_width * in_crt->num_scanlines * sizeof(
                                             *scanline_floatrgb.R));
    scanline_floatrgb.G = SDL_malloc(in_crt->input_width * in_crt->num_scanlines * sizeof(
                                             *scanline_floatrgb.G));
    scanline_floatrgb.B = SDL_malloc(in_crt->input_width * in_crt->num_scanlines * sizeof(
                                             *scanline_floatrgb.B));
    SDL_Log("in_crt->input_height, in_crt->num_scanlines %lu %luy", in_crt->input_height,
            in_crt->num_scanlines);
    // Lanczos_Filter_2D is borked. it makes stripes in the y axis.
    Lanczos_Filter_2D(input_floatrgb.R, scanline_floatrgb.R, in_crt->input_width,
                      in_crt->input_height,
                      in_crt->input_width, in_crt->num_scanlines, LANCZOS_A);
    Lanczos_Filter_2D(input_floatrgb.G, scanline_floatrgb.G, in_crt->input_width,
                      in_crt->input_height,
                      in_crt->input_width, in_crt->num_scanlines, LANCZOS_A);
    Lanczos_Filter_2D(input_floatrgb.B, scanline_floatrgb.B, in_crt->input_width,
                      in_crt->input_height,
                      in_crt->input_width, in_crt->num_scanlines, LANCZOS_A);
    /* TESTING SCANLINE SCALING */
    in_crt->filtered = CRT_Floats2Pixels(&scanline_floatrgb, in_crt->input_width,
                                         in_crt->num_scanlines);
    SDL_SaveBMP(in_crt->filtered, "CRT_SCANLINES.png");
    // Scale to intermediate using nearest
    // Note: nearest also scales brightness
    struct FloatRGB intermediate_floatrgb;
    intermediate_floatrgb.R = SDL_calloc(in_crt->intermediate_width * in_crt->intermediate_height,
                                         sizeof(*intermediate_floatrgb.R));
    intermediate_floatrgb.G = SDL_calloc(in_crt->intermediate_width * in_crt->intermediate_height,
                                         sizeof(*intermediate_floatrgb.G));
    intermediate_floatrgb.B = SDL_calloc(in_crt->intermediate_width * in_crt->intermediate_height,
                                         sizeof(*intermediate_floatrgb.B));
    Nearest_Scanline_Filter_2D(scanline_floatrgb.R, intermediate_floatrgb.R,
                               in_crt->input_width,
                               in_crt->num_scanlines, in_crt->intermediate_width, in_crt->intermediate_height);
    Nearest_Scanline_Filter_2D(scanline_floatrgb.G, intermediate_floatrgb.G,
                               in_crt->input_width,
                               in_crt->num_scanlines, in_crt->intermediate_width, in_crt->intermediate_height);
    Nearest_Scanline_Filter_2D(scanline_floatrgb.B, intermediate_floatrgb.B,
                               in_crt->input_width,
                               in_crt->num_scanlines, in_crt->intermediate_width, in_crt->intermediate_height);
    in_crt->filtered = CRT_Floats2Pixels(&intermediate_floatrgb, in_crt->intermediate_width,
                                         in_crt->intermediate_height);
    SDL_SaveBMP(in_crt->filtered, "CRT_INTERMEDIATE.png");
    // Filtering with Shadow masks
    u8 *temp_mask = SDL_calloc(in_crt->intermediate_width *
                               in_crt->intermediate_height,
                               sizeof(*temp_mask));
    u8 x_offset_B = 0;
    u8 x_offset_G = in_crt->cell_width + in_crt->cell_blank_R ;
    u8 x_offset_R = in_crt->cell_width * 2 + in_crt->cell_blank_G +
                    in_crt->cell_blank_R;
    u8 y_offset = 0;
    // From any cell color perspective, 2 other cells are blank
    u8 Hblank = in_crt->cell_blank_R + in_crt->cell_blank_G + in_crt->cell_blank_B +
                in_crt->cell_width * 2;
    u8 Vblank = in_crt->cell_Vblank;
    // red mask
    CRT_Shadow_Mask(temp_mask, in_crt->intermediate_width, in_crt->intermediate_height,
                    in_crt->cell_height, in_crt->cell_width, Hblank, Vblank, in_crt->stagger, x_offset_R,
                    y_offset);
    CRT_Shadow_Filter(temp_mask, intermediate_floatrgb.R, in_crt->intermediate_height,
                      in_crt->intermediate_width);
    memset(temp_mask, 0, in_crt->intermediate_width * in_crt->intermediate_height * sizeof(
                   *temp_mask));
    // blue mask
    CRT_Shadow_Mask(temp_mask, in_crt->intermediate_width, in_crt->intermediate_height,
                    in_crt->cell_height, in_crt->cell_width, Hblank, Vblank, in_crt->stagger, x_offset_G,
                    y_offset);
    CRT_Shadow_Filter(temp_mask, intermediate_floatrgb.G, in_crt->intermediate_height,
                      in_crt->intermediate_width);
    memset(temp_mask, 0, in_crt->intermediate_width * in_crt->intermediate_height * sizeof(
                   *temp_mask));
    // green mask
    CRT_Shadow_Mask(temp_mask, in_crt->intermediate_width, in_crt->intermediate_height,
                    in_crt->cell_height, in_crt->cell_width, Hblank, Vblank, in_crt->stagger, x_offset_B,
                    y_offset);
    CRT_Shadow_Filter(temp_mask, intermediate_floatrgb.B, in_crt->intermediate_height,
                      in_crt->intermediate_width);
    memset(temp_mask, 0, in_crt->intermediate_width * in_crt->intermediate_height * sizeof(
                   *temp_mask));
    in_crt->filtered = CRT_Floats2Pixels(&intermediate_floatrgb, in_crt->intermediate_width,
                                         in_crt->intermediate_height);
    SDL_SaveBMP(in_crt->filtered, "CRT_SHADOW_MASK.png");
    // Rescaling to render_target size
    struct FloatRGB output_floatrgb;
    output_floatrgb.R = SDL_calloc(in_crt->output_width * in_crt->output_height,
                                   sizeof(*output_floatrgb.R));
    output_floatrgb.G = SDL_calloc(in_crt->output_width * in_crt->output_height,
                                   sizeof(*output_floatrgb.G));
    output_floatrgb.B = SDL_calloc(in_crt->output_width * in_crt->output_height,
                                   sizeof(*output_floatrgb.B));
    // Downscaling should be vertical first
    if ((in_crt->intermediate_width != in_crt->output_width)
        && (in_crt->intermediate_height != in_crt->output_height)) {
        Lanczos_Filter_2D_T(intermediate_floatrgb.R, output_floatrgb.R,
                            in_crt->intermediate_width,
                            in_crt->intermediate_height, in_crt->output_width, in_crt->output_height, LANCZOS_A);
        Lanczos_Filter_2D_T(intermediate_floatrgb.G, output_floatrgb.G,
                            in_crt->intermediate_width,
                            in_crt->intermediate_height, in_crt->output_width, in_crt->output_height, LANCZOS_A);
        Lanczos_Filter_2D_T(intermediate_floatrgb.B, output_floatrgb.B,
                            in_crt->intermediate_width,
                            in_crt->intermediate_height, in_crt->output_width, in_crt->output_height, LANCZOS_A);
    } else {
        memcpy(output_floatrgb.R, intermediate_floatrgb.R,
               in_crt->output_width * in_crt->output_height * sizeof(*output_floatrgb.R));
        memcpy(output_floatrgb.G, intermediate_floatrgb.G,
               in_crt->output_width * in_crt->output_height * sizeof(*output_floatrgb.G));
        memcpy(output_floatrgb.B, intermediate_floatrgb.B,
               in_crt->output_width * in_crt->output_height * sizeof(*output_floatrgb.B));
    }
    in_crt->filtered = CRT_Floats2Pixels(&output_floatrgb, in_crt->output_width,
                                         in_crt->output_height);
    SDL_SaveBMP(in_crt->filtered, "CRT_DOWNSCALED.png");
    SDL_Log("CRT_DOWNSCALED");
    // CRT_Floats2Pixels(in_crt, &output_floatrgb, in_crt->filtered, in_crt->output_width, in_crt->output_height);
    // SDL_assert(in_crt->filtered != NULL);
    // in_crt->filtered_text = SDL_CreateTextureFromSurface(renderer, in_crt->filtered);
    // SDL_assert(in_crt->filtered_text != NULL);
    // SDL_Log("CRT_Floats2Pixels OUT");
    // Filesystem_Texture_Dump("CRT_Filtered.png", renderer, in_crt->filtered_text, in_crt->target_format);
    // Bloom BORKED
    CRT_Bloom(output_floatrgb.G, in_crt->output_width, in_crt->output_height,
              (float)in_crt->output_width / (float)in_crt->input_width);
    CRT_Bloom(output_floatrgb.R, in_crt->output_width, in_crt->output_height,
              (float)in_crt->output_width / (float)in_crt->input_width);
    CRT_Bloom(output_floatrgb.B, in_crt->output_width, in_crt->output_height,
              (float)in_crt->output_width / (float)in_crt->input_width);
    in_crt->filtered = CRT_Floats2Pixels(&output_floatrgb, in_crt->output_width,
                                         in_crt->output_height);
    SDL_SaveBMP(in_crt->filtered, "CRT_BLOOM.png");
    // Clamping BORKED?
    CRT_Clamping(output_floatrgb.R, output_floatrgb.G, output_floatrgb.B,
                 in_crt->output_width,
                 in_crt->output_height);
    in_crt->filtered = CRT_Floats2Pixels(&output_floatrgb, in_crt->output_width,
                                         in_crt->output_height);
    SDL_SaveBMP(in_crt->filtered, "CRT_CLAMPED.png");
    // Float2
    in_crt->filtered = CRT_Floats2Pixels(&output_floatrgb, in_crt->output_width,
                                         in_crt->output_height);
    SDL_SaveBMP(in_crt->filtered, "CRT_Filtered_Surface.png");
    SDL_assert(in_crt->filtered != NULL);
    SDL_Log("CRT_Floats2Pixels OUT");
    Filesystem_Texture_Dump("CRT_Filtered.png", renderer, in_crt->filtered_text,
                            in_crt->target_format, render_target);
    in_crt->filtered_text = SDL_CreateTextureFromSurface(renderer, in_crt->filtered);
    SDL_assert(in_crt->filtered_text != NULL);
    SDL_free(temp_mask);
    SDL_free(intermediate_floatrgb.R);
    SDL_free(intermediate_floatrgb.G);
    SDL_free(intermediate_floatrgb.B);
    SDL_free(input_floatrgb.R);
    SDL_free(input_floatrgb.G);
    SDL_free(input_floatrgb.B);
    SDL_free(scanline_floatrgb.R);
    SDL_free(scanline_floatrgb.G);
    SDL_free(scanline_floatrgb.B);
}

void CRT_Free(struct CRT *in_crt) {
    if (in_crt->unfiltered != NULL) {
        SDL_LockSurface(in_crt->unfiltered);
        SDL_free(in_crt->unfiltered->pixels);
        SDL_UnlockSurface(in_crt->unfiltered);
        SDL_FreeSurface(in_crt->unfiltered);
    }
}
