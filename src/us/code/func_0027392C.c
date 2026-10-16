/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025A844_de_4.c, func_8025B5F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
struct SlotRecAtB0;
typedef struct SlotRecAtB0 SlotRecAtB0;
struct HeaderRecAt102;
typedef struct HeaderRecAt102 HeaderRecAt102;
struct HeaderRecAt102;
struct HeaderRecAt102 {
    char pad[0x102];
    s16 local;
};
struct OwnerCode8025A3EC8025B5F0;
typedef struct OwnerCode8025A3EC8025B5F0 OwnerCode8025A3EC8025B5F0;
struct OwnerCode8025A3EC8025B5F0;
struct OwnerCode8025A3EC8025B5F0 {
    char pad[0x84];
    char sound[0x58];
    s16 samples[20];
    char pad2[(0x104 - 0xDC) - 40];
    s32 key;
};
struct OwnerCode8025A3EC8025B5F0;
struct SlotRecAtB0;
struct SlotRecAtB0 {
    s32 index;
    s32 state;
    s32 used;
    char pad0[4];
    s32 key;
    char pad1[0x3C];
    s32 flag;
    char pad2[0x54];
    s32 value;
    s32 active;
    struct OwnerCode8025A3EC8025B5F0 *owner;
    char pad3[0x18];
};
struct HeaderRecAt102;
struct RecordRecAt4V6;
struct RecordRecAt4V6 {
    struct HeaderRecAt102 *header;
    SlotRecAtB0 slots[17];
};
struct RecordRecAt4V6;
typedef struct RecordRecAt4V6 RecordRecAt4V6;
struct OwnerCode8025A3EC8025B5F0;
struct HeaderRecAt102;
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
