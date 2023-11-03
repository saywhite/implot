#include "implot.h"
#include "implot_internal.h"

//-----------------------------------------------------------------------------
// [SECTION] Macros and Defines
//-----------------------------------------------------------------------------

#define SQRT_1_2 0.70710678118f
#define SQRT_3_2 0.86602540378f

#ifndef IMPLOT_NO_FORCE_INLINE
    #ifdef _MSC_VER
        #define IMPLOT_INLINE __forceinline
    #elif defined(__GNUC__)
        #define IMPLOT_INLINE inline __attribute__((__always_inline__))
    #elif defined(__CLANG__)
        #if __has_attribute(__always_inline__)
            #define IMPLOT_INLINE inline __attribute__((__always_inline__))
        #else
            #define IMPLOT_INLINE inline
        #endif
    #else
        #define IMPLOT_INLINE inline
    #endif
#else
    #define IMPLOT_INLINE inline
#endif

#if defined __SSE__ || defined __x86_64__ || defined _M_X64
#ifndef IMGUI_ENABLE_SSE
#include <immintrin.h>
#endif
static IMPLOT_INLINE float  ImInvSqrt(float x) { return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(x))); }
#else
static IMPLOT_INLINE float  ImInvSqrt(float x) { return 1.0f / sqrtf(x); }
#endif

#define IMPLOT_NORMALIZE2F_OVER_ZERO(VX,VY) do { float d2 = VX*VX + VY*VY; if (d2 > 0.0f) { float inv_len = ImInvSqrt(d2); VX *= inv_len; VY *= inv_len; } } while (0)

// Support for pre-1.82 versions. Users on 1.82+ can use 0 (default) flags to mean "all corners" but in order to support older versions we are more explicit.
#if (IMGUI_VERSION_NUM < 18102) && !defined(ImDrawFlags_RoundCornersAll)
#define ImDrawFlags_RoundCornersAll ImDrawCornerFlags_All
#endif

namespace ImPlot {

//-----------------------------------------------------------------------------
// [SECTION] Getters
//-----------------------------------------------------------------------------

struct GetterXY2 {
    GetterXY2(double pt_x, double pt_y) : PtX(pt_x), PtY(pt_y), Count(1) { }
    template <typename I> IMPLOT_INLINE ImPlotPoint operator()(I) const {
        return ImPlotPoint(PtX, PtY);
    }
    const double PtX;
    const double PtY;
    const int Count;
};

//-----------------------------------------------------------------------------
// [SECTION] Markers
//-----------------------------------------------------------------------------

template <typename _Getter>
void RenderMarkers(const _Getter& getter, ImPlotMarker marker, float size, bool rend_fill, ImU32 col_fill, bool rend_line, ImU32 col_line, float weight);

}