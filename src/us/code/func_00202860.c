/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AUDIO_2/80255280.CPP, func_802552E0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/cache.h"
#include "common.h"

extern s32 D_800AB0B0;
extern u32 D_800AB0B4;
struct EntryRecAtCV1;
typedef struct EntryRecAtCV1 EntryRecAtCV1;
struct EntryRecAtCV1;
struct EntryRecAtCV1 {
    u32 key;
    CCacheEntry *value;
    s32 index;
    struct EntryRecAtCV1 *next;
};
extern s32 D_800D81C8;
extern EntryRecAtCV1 *D_800D81C4;
extern void CHashTable__Construct(s32 capacity);
extern s32 D_800D81C0;
extern char gHeap;
extern EntryRecAtCV1 *CHeap__Alloc(void *, s32);
void CHashTable__Construct(s32 capacity)
{
    s32 bits = 0;
    s32 top = 0;
    s32 i;
    s32 half;
    u32 n;
    for (i = 0; i < 32; i++) {
        s32 bit = 1 << i;
        if (capacity & bit) {
            bits++;
            top = i;
        }
    }
    if (bits != 1) {
        top++;
    }
    half = 1 << top;
    D_800AB0B4 = half * 2;
    D_800AB0B0 = half;
    D_800D81C0 = D_800AB0B4 - 2;
    D_800D81C8 = D_800AB0B4 - 1;
    D_800D81C4 = CHeap__Alloc(&gHeap, half << 5);
    for (n = 0; n < D_800AB0B4; n++) {
        D_800D81C4[n].value = 0;
        D_800D81C4[n].key = 0;
        D_800D81C4[n].next = 0;
        D_800D81C4[n].index = n;
    }
}
