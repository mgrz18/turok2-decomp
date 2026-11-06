/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AUDIO_2/80255280.CPP, func_802552E0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/cache.h"
#include "common.h"
#include "globals/cache.h"
#include "globals/sysrc.h"

extern void CHashTable__Construct(s32 capacity);
extern HashEntry *CHeap__Alloc(CHeap *, s32);
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
    gHashTableSize = half * 2;
    gHashSize = half;
    gHashMask = gHashTableSize - 2;
    gHashTableMask = gHashTableSize - 1;
    gHashTable = CHeap__Alloc(&gHeap, half << 5);
    for (n = 0; n < gHashTableSize; n++) {
        gHashTable[n].value = 0;
        gHashTable[n].key = 0;
        gHashTable[n].next = 0;
        gHashTable[n].index = n;
    }
}
