typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
#ifdef TARGET_PORT
/* the elements are host pointers (docs/port/64BIT.md): u32 on the N64 */
typedef __UINTPTR_TYPE__ t2_uptr;
#else
typedef u32 t2_uptr;
#endif
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
