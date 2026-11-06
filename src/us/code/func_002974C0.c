/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/cosf.c, cosf, with Turok 2's layouts. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -fforce-addr */

typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef double f64;
f32 func_002974C0(f32 x);
typedef union {
    struct {
        u32 hi;
        u32 lo;
    } w;
    f64 d;
} du;
typedef union {
    u32 w;
    f32 f;
} fu;
extern const du sCosfP[]; /* P: polynomial coefficients */
extern const du sCosfRpi; /* 1/pi */
extern const du sCosfPiHi; /* pi, high part */
extern const du sCosfPiLo; /* pi, low part */
extern const fu sCosfZero; /* 0.0f */
f32 func_002974C0(f32 x) {
    f64 dx;
    f64 xsq;
    f64 poly;
    f64 dn;
    s32 n;
    s32 ix;
    s32 xpt;
    ix = *(s32 *)&x;
    xpt = (ix >> 22) & 0x1FF;
    if (xpt < 0x136) {
        dx = __builtin_fabsf(x);
        dn = dx * sCosfRpi.d + 0.5;
        if (dn >= 0.0) {
            n = dn + 0.5;
        } else {
            n = dn - 0.5;
        }
        dn = n;
        dn -= 0.5;
        dx = dx - dn * sCosfPiHi.d;
        dx = dx - dn * sCosfPiLo.d;
        xsq = dx * dx;
        poly = (((sCosfP[4].d * xsq + sCosfP[3].d) * xsq + sCosfP[2].d) * xsq + sCosfP[1].d);
        poly = dx + dx * xsq * poly;
        if (!(n & 1)) {
            return (f32)poly;
        }
        return -(f32)poly;
    }
    return sCosfZero.f;
}
