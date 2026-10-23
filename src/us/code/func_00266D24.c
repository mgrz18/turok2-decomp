typedef unsigned int u32;
typedef int s32;

/* Binary search for key in a sorted u32 array; returns its index or -1. */
s32 func_00266D24(u32 *array, u32 count, u32 key) {
    u32 lo;
    u32 hi;
    u32 mid;

    if (count != 0) {
        hi = count - 1;
        lo = 0;
        while (lo < hi) {
            mid = (lo + hi) >> 1;
            if (array[mid] < key) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        if (array[lo] == key) {
            return lo;
        }
        return -1;
    }
    return -1;
}
