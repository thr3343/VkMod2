#pragma once


/*
 *  Could do an SSE version but in more recent CPU SKUs AVX/AVX2 is much more well supported outside of specific pentium[s] 
    and as many Games/Applicbals also ship with AVX and do not always bother to check for proper/approaote instruction set support anyway 
    SSE is not a Primary/Major perogative or concer at this Point.Stage
*/

// #include <array>
#include <array>
#include <cassert>
#include <cstdint>
#include <immintrin.h>

using __float512 [[gnu::vector_size(64)]] = float;



struct mat4x
{
public:
  [[gnu::const]]  __m512 _a = {1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F };
  
  consteval mat4x() = default;
  explicit consteval mat4x(__m512 ax) : _a(ax) {};
   explicit mat4x(float* abs) : _a(*reinterpret_cast<const __m512*>(abs)){};
   [[gnu::const]] inline auto operator()(uint32_t  __n) noexcept -> float;
   [[gnu::const, nodiscard]] inline __vectorcall auto operator[](uint32_t=0) noexcept -> __m128;
   [[gnu::const, nodiscard]] inline auto operator()(uint32_t, uint32_t) noexcept -> float;


     [[gnu::const]] __m256 sum();
     [[gnu::const]] void mul(mat4x);

using iterator = float*;
    using const_iterator = const float*;

    // Class methods
    [[nodiscard]] auto cbegin() const -> const float* { return (float*)this; }
    [[nodiscard]] auto cend() const -> const float* { return (float*)this + 16; }
    [[nodiscard]] auto begin() const -> const float* { return cbegin(); }
    [[nodiscard]] auto end() const -> const float* { return cend(); }

    [[nodiscard]] auto begin() -> float* { return (float*)this; }
    [[nodiscard]] auto end() -> float* { return (float*)this + 16; }

         [[gnu::const, gnu::aligned(32)]] __vectorcall __m128 ret(uint32_t);
         [[gnu::aligned(32)]] void transpose();
};




[[gnu::const, nodiscard, clang::always_inline, gnu::nodebug]] inline auto mat4x::operator()(uint32_t __n=0) noexcept -> float
{
  // assert(__n<2);
  return _a[__n&15];

}
[[gnu::const, nodiscard, clang::always_inline, gnu::nodebug]] inline __vectorcall auto mat4x::operator[](uint32_t __n) noexcept -> __m128
{
  // assert(__n<2);
  return _mm_load_ps(reinterpret_cast<float*>(this)+(__n*4));

}
[[gnu::const, nodiscard, clang::always_inline, gnu::nodebug]] inline auto mat4x::operator()(uint32_t __n, uint32_t __a) noexcept -> float
{
  // assert(__n<2);
  __n<<=2;
  return _a[__n+__a];

}