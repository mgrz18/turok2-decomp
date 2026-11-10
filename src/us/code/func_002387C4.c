/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_3/80268160.CPP, func_80268150_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
#ifndef COMMON_H
#ifdef TARGET_PORT
typedef __INTPTR_TYPE__ sptr;
typedef __UINTPTR_TYPE__ uptr;
#else
typedef signed int sptr;
typedef unsigned int uptr;
#endif
#endif
struct Triple;
struct Triple;
struct Triple {
    s32 x;
    s32 y;
    s32 z;
};
struct Triple;
typedef struct Triple Triple;
struct IntPair;
struct IntPair {
    int field_0;
    int field_4;
};
struct IntPair;
struct IntPair;
struct IntPair;
struct Triple;
struct Triple;
struct Triple;
struct IntPair;
struct IntPair;
struct Triple;
struct Triple;
struct Triple;
extern void ImpactEvent_ApplyBlast(void *, sptr, s32, Triple, struct IntPair, s32, s32, s32);
void ImpactEvent_AreaDamagePush(s32 arg0, s32 arg1, s32 arg2, Triple arg3, struct IntPair arg6) {
    ImpactEvent_ApplyBlast((void *)arg0, arg1, arg2, arg3, arg6, 1, 1, 0x100000);
}
