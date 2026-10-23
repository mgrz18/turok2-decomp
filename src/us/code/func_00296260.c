typedef int s32;
typedef double f64;

extern f64 fabs(f64);

/* frexp(): splits x into a mantissa in [0.5, 1) and a power of two in *exp. */
f64 func_00296260(f64 x, s32 *exp) {
    f64 m;

    *exp = 0;
    if (x == 0.0) {
        return x;
    }
    m = fabs(x);
    while (m >= 1.0) {
        m *= 0.5;
        (*exp)++;
    }
    while (m < 0.5) {
        m += m;
        (*exp)--;
    }
    if (!(x > 0.0)) {
        m = -m;
    }
    return m;
}
