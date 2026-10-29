typedef signed int s32;
typedef unsigned int u32;

/* libgcc __cmpdi2 on split halves */
s32 __cmpdi2(s32 a_hi, u32 a_lo, s32 b_hi, u32 b_lo) {
    if (a_hi < b_hi) {
        return 0;
    } else if (a_hi > b_hi) {
        return 2;
    }
    if (a_lo < b_lo) {
        return 0;
    } else if (a_lo > b_lo) {
        return 2;
    }
    return 1;
}
