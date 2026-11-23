/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/ANIM/80285290.CPP, func_802852C0_de, with Turok 2's layouts. */

#include "common.h"

#ifdef TARGET_PORT
/* the elements are host pointers (docs/port/64BIT.md): u32 on the N64 */
typedef __UINTPTR_TYPE__ t2_uptr;
#else
typedef u32 t2_uptr;
#endif
void Sort_Insertion(t2_uptr first, t2_uptr last, s32 size, s32 (*compare)(t2_uptr, t2_uptr), s32 (*swap)(t2_uptr, t2_uptr));

/* Sorts num elements of the given width with a comparator and a swap routine using an iterative quicksort: partitions around the middle element, pushes the larger side onto a 30-entry stack and continues with the smaller, and hands ranges of eight or fewer elements to Sort_Insertion. */

void Sort_Quick(char *base, u32 num, u32 width, s32 (*compare)(char *, char *), void (*swap)(char *, char *)) {
    char *lo;
    char *hi;
    char *loguy;
    char *higuy;
    u32 size;
    char *lostk[30];
    char *histk[30];
    s32 stkptr;

    if (num < 2 || width == 0) {
        return;
    }
    stkptr = 0;
    lo = base;
    hi = base + width * (num - 1);
recurse:
    size = (hi - lo) / width + 1;
    if (size <= 8) {
        Sort_Insertion((uptr)lo, (uptr)hi, width, (s32 (*)(u32, u32))compare, (void (*)(u32, u32))swap);
    } else {
        swap(lo + (size / 2) * width, lo);
        loguy = lo;
        higuy = hi + width;
        for (;;) {
            do {
                loguy += width;
            } while (loguy <= hi && compare(loguy, lo) <= 0);
            do {
                higuy -= width;
            } while (higuy > lo && compare(higuy, lo) >= 0);
            if (higuy < loguy) {
                break;
            }
            swap(loguy, higuy);
        }
        swap(lo, higuy);
        if (higuy - 1 - lo >= hi - loguy) {
            if (lo + width < higuy) {
                lostk[stkptr] = lo;
                histk[stkptr] = higuy - width;
                ++stkptr;
            }
            if (loguy < hi) {
                lo = loguy;
                goto recurse;
            }
        } else {
            if (loguy < hi) {
                lostk[stkptr] = loguy;
                histk[stkptr] = hi;
                ++stkptr;
            }
            if (lo + width < higuy) {
                hi = higuy - width;
                goto recurse;
            }
        }
    }
    --stkptr;
    if (stkptr >= 0) {
        lo = lostk[stkptr];
        hi = histk[stkptr];
        goto recurse;
    }
}

/* De-faked in round 21 (j4): the draft copied first into best through two identical arms; the plain
 * loops match. */
/* Sorts the elements first..last (size bytes apart) in place, largest last: picks the largest with
 * compare and swaps it to the end, then shortens the range. (A selection sort, despite the name.) */
void Sort_Insertion(t2_uptr first, t2_uptr last, s32 size, s32 (*compare)(t2_uptr, t2_uptr), s32 (*swap)(t2_uptr, t2_uptr))
{
    t2_uptr p;
    t2_uptr best;

    while (first < last) {
        best = first;
        for (p = first + size; p <= last; p += size) {
            if (compare(p, best) > 0) {
                best = p;
            }
        }
        swap(best, last);
        last -= size;
    }
}
