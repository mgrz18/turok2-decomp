/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025A844_de_4.c, func_8025B5F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_00293E60(void *sound, s16 sample);
extern s32 func_00293580(void *sound);
extern void func_00293EC0(void *sound);
extern void func_00293E60(void *sound, s16 sample);
extern s32 func_00293580(void *sound);
extern void func_00293EC0(void *sound);
extern void func_00293E60(void *arg0, s16 arg1);
extern s32 func_00293580(void *arg0);
extern void func_00293EC0(void *arg0);
void func_0027392C(RecordRecAt4V6 *record, s32 value)
{
  s32 i;
  s32 active;
  s32 absent;
  SlotRecAtB0 *slot = record->slots;
  i = 0;
  absent = -1;
  active = 1;
  for (; i < 16; slot++, i++)
  {
    if (((slot->used != absent) && (record->header->local != i)) && ((value == absent) || (slot->value != value)))
    {
      OwnerCode8025A3EC8025B5F0 *owner = slot->owner;
      void *sound;
      slot->active = active;
      slot->flag = 0;
      if (slot->key != owner->key)
      {
        sound = owner->sound;
        func_00293E60(sound, owner->samples[slot->index]);
        if (func_00293580(sound) != 0)
        {
          if (active)
          {
            func_00293EC0(sound);
          }
          else
          {
            func_00293EC0(sound);
          }
        }
        slot->state = absent;
      }
    }
  }
}
