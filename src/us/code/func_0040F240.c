/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU_2/80400000_3.CPP, func_80400000_de, with Turok 2's layouts. */
#include "common.h"

extern s32 **func_00204EDC(s32 heap, s32 size, s32 tag, const char *name);
extern void func_002052D8(s32 heap, s32 *block);

/* Allocates a temporary array; the block's first word points back at the handle. */
static __inline__ f32 *TempAlloc(s32 size) {
    s32 **handle;
    s32 *block;

    handle = func_00204EDC(0, size + 0x10, 0x13, "Cinema temp alloc");
    if (handle != NULL) {
        block = *handle;
        *block = (s32) handle;
        return (f32 *) (block + 4);
    }
    return NULL;
}

static __inline__ void TempFree(f32 *data) {
    if (data != NULL) {
        func_002052D8(0, ((s32 **) data)[-4]);
    }
}

/* Computes the second derivatives of a natural cubic spline through n points (ys over xs) into out. */
void func_0040F240(s32 n, f32 *ys, f32 *xs, f32 *out) {
    f32 *diagonal;
    f32 *rhs;
    f32 *width;
    s32 singular;
    s32 i;

    singular = 0;
    if (n == 1) {
        out[0] = 0;
        return;
    }
    diagonal = TempAlloc(n * 4);
    rhs = TempAlloc(n * 4);
    width = TempAlloc(n * 4);
    for (i = 1; i < n - 1; i++) {
        diagonal[i] = 2.0f * (xs[i + 1] - xs[i - 1]);
        if (diagonal[i] == 0.0f) {
            diagonal[i] = 1.0f;
            singular = 1;
        }
    }
    for (i = 0; i < n - 1; i++) {
        width[i] = xs[i + 1] - xs[i];
        if (width[i] == 0.0f) {
            width[i] = 1.0f;
            singular = 1;
        }
    }
    for (i = 1; i < n - 1; i++) {
        rhs[i] = ((ys[i + 1] - ys[i]) / width[i] - (ys[i] - ys[i - 1]) / width[i - 1]) * 6.0f;
    }
    for (i = 1; i < n - 2; i++) {
        rhs[i + 1] -= rhs[i] * width[i] / diagonal[i];
        diagonal[i + 1] -= width[i] * width[i] / diagonal[i];
    }
    for (i = 0; i < n; i++) {
        out[i] = 0;
    }
    if (!singular) {
        for (i = n - 2; i > 0; i--) {
            out[i] = (rhs[i] - width[i] * out[i + 1]) / diagonal[i];
        }
    }
    TempFree(diagonal);
    TempFree(rhs);
    TempFree(width);
}
