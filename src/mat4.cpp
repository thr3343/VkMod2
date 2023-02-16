#include "mat4x.hpp"



[[gnu::const]] __m256 mat4x::sum()
{
  auto a =  *(const __m256 *)&_a;
  return a;
}
[[clang::always_inline, gnu::nodebug]] __vectorcall auto xM512(__m128 a, __m128 b, __m128 c, __m128 d)
{
  return __v16sf{a[0],a[1],a[2],a[3],b[0],b[1],b[2],b[3],c[0],c[1],c[2],c[3],d[0],d[1],d[2],d[3]};
}
[[clang::always_inline, gnu::nodebug]] __vectorcall auto xM5122(__m128 a)
{
  return __v16sf{a[0],a[1],a[2],a[3],a[0],a[1],a[2],a[3],a[0],a[1],a[2],a[3],a[0],a[1],a[2],a[3]};
}
[[clang::always_inline, gnu::nodebug]] __vectorcall auto xM128(float a)
{
  return __extension__ (__m128)(__v4sf){ a, a, a, a };;
}
__vectorcall auto xM256(float a)
{
  return __v8sf{a,a,a,a,a,a,a,a};
}

[[gnu::const]] void mat4x::mul(mat4x a)
{
  // a.transpose();
  // auto abs = a.operator()();
  // auto abs1 = a.operator()(1);
  // auto abs2 = a.operator()(2);
  // auto abs3 = a.operator()(3);




  // auto x=operator()(0);
  // auto y=operator()(1);
  // auto z=operator()(2);
  // auto w=operator()(3);

  // auto xa=_mm_fmadd_ps(abs,x,_mm_fmadd_ps(abs1,y,_mm_fmadd_ps(abs2,z,(abs3*w))));
  // auto yb=_mm_fmadd_ps(abs,y,_mm_fmadd_ps(abs1,x,_mm_fmadd_ps(abs1,z,(abs1*w))));
  // auto zc=_mm_fmadd_ps(abs,z,_mm_fmadd_ps(abs2,y,_mm_fmadd_ps(abs2,x,(abs2*w))));
  // auto wd=_mm_fmadd_ps(abs,w,_mm_fmadd_ps(abs3,y,_mm_fmadd_ps(abs3,z,(abs3*x))));
  // auto mx=;
  auto x = this->operator[](0);
  auto y = this->operator[](1);
  auto z = this->operator[](2);
  auto w = this->operator[](3);
  // float xa=a(0,0);auto xv = _mm_broadcast_ss(&xa);
  // float ya=a(1,0);auto yv = _mm_broadcast_ss(&ya);
  // float za=a(2,0);auto zv = _mm_broadcast_ss(&za);
  // float wa=a(3,0);auto wv = _mm_broadcast_ss(&wa);
auto ayslx = __m128{a(0,3), a(1,3), a(2,3), a(3,3)}*w;
auto xy0=xM128(ayslx[0]);
auto xy1=xM128(ayslx[1]);
auto xy2=xM128(ayslx[2]);
auto xy3=xM128(ayslx[3]);


  auto xx = _mm_fmadd_ps(x,xM128(a[0][0]),_mm_fmadd_ps(y,xM128(a[0][1]),_mm_fmadd_ps(z,xM128(a[0][2]),xy0)));
  auto yy = _mm_fmadd_ps(x,xM128(a[1][0]),_mm_fmadd_ps(y,xM128(a[1][1]),_mm_fmadd_ps(z,xM128(a[1][2]),xy1)));
  auto zz = _mm_fmadd_ps(x,xM128(a[2][0]),_mm_fmadd_ps(y,xM128(a[2][1]),_mm_fmadd_ps(z,xM128(a[2][2]),xy2)));
  auto ww = _mm_fmadd_ps(x,xM128(a[3][0]),_mm_fmadd_ps(y,xM128(a[3][1]),_mm_fmadd_ps(z,xM128(a[3][2]),xy3)));
        // float nm00 = fmaf(x[0], a(0,0), fmaf(y[0], a(0,1), fmaf(z[0], a(0,2), w[0] * a(0,3))));
        // float nm01 = fmaf(x[1], a(0,0), fmaf(y[1], a(0,1), fmaf(z[1], a(0,2), w[1] * a(0,3))));
        // float nm02 = fmaf(x[2], a(0,0), fmaf(y[2], a(0,1), fmaf(z[2], a(0,2), w[2] * a(0,3))));
        // float nm03 = fmaf(x[3], a(0,0), fmaf(y[3], a(0,1), fmaf(z[3], a(0,2), w[3] * a(0,3))));
        // float nm10 = fmaf(x[0], a(1,0), fmaf(y[0], a(1,1), fmaf(z[0], a(1,2), w[0] * a(1,3))));
        // float nm11 = fmaf(x[1], a(1,0), fmaf(y[1], a(1,1), fmaf(z[1], a(1,2), w[1] * a(1,3))));
        // float nm12 = fmaf(x[2], a(1,0), fmaf(y[2], a(1,1), fmaf(z[2], a(1,2), w[2] * a(1,3))));
        // float nm13 = fmaf(x[3], a(1,0), fmaf(y[3], a(1,1), fmaf(z[3], a(1,2), w[3] * a(1,3))));
        // float nm20 = fmaf(x[0], a(2,0), fmaf(y[0], a(2,1), fmaf(z[0], a(2,2), w[0] * a(2,3))));
        // float nm21 = fmaf(x[1], a(2,0), fmaf(y[1], a(2,1), fmaf(z[1], a(2,2), w[1] * a(2,3))));
        // float nm22 = fmaf(x[2], a(2,0), fmaf(y[2], a(2,1), fmaf(z[2], a(2,2), w[2] * a(2,3))));
        // float nm23 = fmaf(x[3], a(2,0), fmaf(y[3], a(2,1), fmaf(z[3], a(2,2), w[3] * a(2,3))));
        // float nm30 = fmaf(x[0], a(3,0), fmaf(y[0], a(3,1), fmaf(z[0], a(3,2), w[0] * a(3,3))));
        // float nm31 = fmaf(x[1], a(3,0), fmaf(y[1], a(3,1), fmaf(z[1], a(3,2), w[1] * a(3,3))));
        // float nm32 = fmaf(x[2], a(3,0), fmaf(y[2], a(3,1), fmaf(z[2], a(3,2), w[2] * a(3,3))));
        // float nm33 = fmaf(x[3], a(3,0), fmaf(y[3], a(3,1), fmaf(z[3], a(3,2), w[3] * a(3,3))));
   
  _a=xM512(xx,yy,zz,ww);

  
  
 
}
void mat4x::transpose()
{
  //  auto x = ret(0);
  //  auto y = ret(1);
  //  auto z = ret(2);
  //  auto w = ret(3);
  //  _a=xM512(x, y, z, w);

     _a = __builtin_shufflevector(_a, _a, 0,4,8,12,1,4,9,13,2,5,10,14,3,6,11,15);
}

[[gnu::const]] __vectorcall __m128 mat4x::ret(uint32_t a=0)
{
  // a&=15;
  const __v4su ax={0U+a, 1U+a, 2U+a, 3U+a};

  return __extension__(__v4sf){(*this)(0U+a),(*this)(4U+a),(*this)(8U+a),(*this)(12U+a)};
}