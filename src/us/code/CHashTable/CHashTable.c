/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AUDIO_2/80255280.CPP, func_802552E0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/cache.h"
#include "common.h"
#include "globals/cache.h"
#include "globals/sysrc.h"

void CHashTable__Construct(s32 capacity);
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
    gHashTable = CHeap__Alloc(&gHeap, (half << 1) * sizeof(HashEntry));
    for (n = 0; n < gHashTableSize; n++) {
        gHashTable[n].value = 0;
        gHashTable[n].key = 0;
        gHashTable[n].next = 0;
        gHashTable[n].index = n;
    }
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80254D44_de_9.c, func_802553B4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void CHashTable__Insert(u32 key, CCacheEntry *value) {
 HashEntry *entry = gHashTable + (((key<<5)^(key>>1)^(key>>9)^(key>>17)) & gHashMask);
 HashEntry *tail;
 HashEntry *slot;
 s32 index;
 if (entry->value == 0) {entry->key=key;entry->value=value;return;}
 tail=entry;
 for (;;) {
  if (entry->key==key) {entry->value=value;return;}
  entry=tail->next;
  if (!entry) break;
  tail=entry;
 }
 index=tail->index|1;
 while (gHashTable[index].value != 0) {index+=2; index &= gHashTableMask;}
 slot=(HashEntry *)((index * sizeof(HashEntry)) + (uptr)gHashTable);
 slot->key=key;slot->value=value;tail->next=slot;
}

void CHashTable__Remove(u32 key)
{
  HashEntry *next;
  HashEntry *prev;
  HashEntry *entry;
  entry = &gHashTable[((key << 5) ^ (key >> 1) ^ (key >> 9) ^ (key >> 17)) & gHashMask];
  prev = 0;
  if (entry->key != key)
  {
    loop_1:
    prev = entry;

    entry = entry->next;
    if (entry != 0)
    {
 do { if (entry->key == key) { goto block_3; } } while (0);
      goto loop_1;
    }
  }
  else
  {
    block_3:
    if (entry != 0)
    {
      next = entry->next;
      if (next != 0)
      {
        entry->value = next->value;
        entry->key = next->key;
        entry->next = next->next;
        {
          HashEntry *temp = entry;
          entry = next;
          next = temp;
        }
      }
      if (prev != 0)
      {
        prev->next = next;
      }
      entry->value = 0;
      entry->key = 0;
      entry->next = 0;
    }

  }
}
s32 RoundToPowerOfTwo(s32 arg0);

s32 RoundToPowerOfTwo(s32 arg0) {
    s32 var_a1;
    s32 var_a2;
    s32 var_v1;
    var_a1 = 0;
    var_a2 = 0;
    var_v1 = 0;
    do {
        if (arg0 & (u32)(1 << var_v1)) {
            var_a1 += 1;
            var_a2 = var_v1;
        }
        var_v1 += 1;
    } while (var_v1 < 0x20);
    if (var_a1 != 1) {
        var_a2 += 1;
    }
    return 1 << var_a2;
}

void CHashTable__GetLongestChain(void) {
    s32 var_a1;
    s32 var_v1;
    u32 var_a0;
    u32 var_a0_2;
    u32 limit;
    HashEntry *var_v0;
    HashEntry *buckets;
    var_a1 = 0;
    var_a0 = var_a1;
    limit = gHashSize;
    if (limit != 0) {
        var_v1 = limit;
        do {
            var_a0 += 1;
        } while (var_a0 < (u32)var_v1);
    }
    var_a0_2 = 0;
    if (gHashSize != 0) {
        buckets = gHashTable;
        do {
            var_v0 = &buckets[var_a0_2];
            var_v1 = 0;
            if (var_v0 != 0) {
                do {
                    var_v0 = var_v0->next;
                    var_v1 += 1;
                } while (var_v0 != 0);
            }
            if (var_v1 < var_a1) {
                var_v1 = var_a1;
            }
            var_a0_2 += 1;
            var_a1 = var_v1;
        } while (var_a0_2 < gHashSize);
    }
    if (sHashDebugLongestChain != 0) {
        sHashDebugLongestChain = 0;
    }
}
