#include "globals/cache.h"

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
