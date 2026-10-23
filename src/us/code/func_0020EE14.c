/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AI/NAV/8027451C.CPP, func_802749B4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef float f32;

/* Wrap an angle into [-PI, PI]. */
static __inline__ f32 wrapAngle(f32 a) {
    while (a < -3.1415927f) {
        a += 6.2831855f;
    }
    while (3.1415927f < a) {
        a -= 6.2831855f;
    }
    return a;
}

/* Signed difference arg1 - arg0, both wrapped. */
f32 func_0020EE14(f32 arg0, f32 arg1) {
    f32 b = wrapAngle(arg1);
    f32 d = arg0 - b;
    d = wrapAngle(d);
    return -d;
}
