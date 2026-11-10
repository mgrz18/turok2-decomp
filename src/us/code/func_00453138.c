/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MULTI/804453C4_7.CPP, func_80445590_de.
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
struct RecordRecAt14;
struct RecordRecAt14 {
    char pad[8];
    s32 flags;
    char padC[0x14 - 0xC];
    s32 value;
};
extern s32 GetMemSize(struct RecordRecAt14 *);
extern sptr Video_GetResolutionText_2();
s32 func_00453138(struct RecordRecAt14 *record) {
    if (GetMemSize(record) != 0x400000) {
        record->flags |= 0x800000;
    } else {
        record->flags &= ~0x800000;
    }
    record->value = Video_GetResolutionText_2();
    return 0;
}
