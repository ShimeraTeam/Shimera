#include "backend/raylib/converts/RaylibConvert.hpp"

#include "Color.inl"

Matrix shimera::toRaylibMatrix(const Mat4& s) {
    Matrix r;
    r.m0=s.m[0];  r.m1=s.m[1];  r.m2 =s.m[2];  r.m3 =s.m[3];
    r.m4=s.m[4];  r.m5=s.m[5];  r.m6 =s.m[6];  r.m7 =s.m[7];
    r.m8=s.m[8];  r.m9=s.m[9];  r.m10=s.m[10]; r.m11=s.m[11];
    r.m12=s.m[12];r.m13=s.m[13];r.m14=s.m[14]; r.m15=s.m[15];
    return r;
}

Color shimera::toRaylibColor(const shimera::Color& c) {
    Vec4<unsigned char> const rgbForm = c.toRGB();
    return { rgbForm.x, rgbForm.y, rgbForm.z, rgbForm.w };
}
