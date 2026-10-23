#include "common.h"

/* Binary search of a sorted array; index of key, or -1. */
static __inline__ s32 findSorted(u32 *array, u32 count, u32 key) {
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

/* Finds the run of entries equal to key; returns 0 when there is none. */
s32 func_00266D8C(u32 *array, s32 count, u32 key, s32 *first, s32 *last) {
    s32 index = findSorted(array, count, key);
    s32 i;

    if (index == -1) {
        return 0;
    }
    for (i = index - 1; i != -1; i--) {
        if (array[i] != key) {
            break;
        }
    }
    *first = i + 1;
    for (index++; index < count; index++) {
        if (array[index] != key) {
            break;
        }
    }
    *last = index - 1;
    return 1;
}
