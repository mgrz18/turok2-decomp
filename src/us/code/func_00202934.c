/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80254D44_de_9.c, func_802553B4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/cache.h"

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
 slot=(HashEntry *)((index << 4) + (u32)gHashTable);
 slot->key=key;slot->value=value;tail->next=slot;
}
