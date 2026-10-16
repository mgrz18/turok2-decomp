/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025A844_de_4.c, func_8025B854_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
typedef float f32;
struct RecordCode8025A3EC;
typedef struct RecordCode8025A3EC RecordCode8025A3EC;
struct RecordCode8025A3EC;
struct RecordCode8025A3EC {
    s32 index;
    char pad04[4];
    s32 f08;
    s32 f0C;
    char pad10[4];
    s32 f14;
    char pad18[0x14];
    f32 f2C;
    char pad30[4];
    f32 f34;
    s16 f38;
    s16 f3A;
    char pad3C[4];
    s32 f40;
    char pad44[0x14];
    s32 f58;
    s32 f5C;
    char pad60[0x44];
    s32 fA4;
    char padA8[4];
    s32 fAC;
    s32 owner;
    s32 fB4;
    f32 fB8;
    s32 fBC;
    s32 fC0;
    s32 fC4;
    f32 fC8;
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
extern s32 func_00273B90(s32 arg0, s16 arg1);
struct OwnerCode8025A3EC8025B5F0;
extern void func_00293E60(void *sound, s16 sample);
extern s32 func_00293580(void *sound);
extern void func_00293E60(void *sound, s16 sample);
extern s32 func_00293580(void *sound);
extern void func_00293E60(void *arg0, s16 arg1);
extern s32 func_00293580(void *arg0);
s32 func_00273B90(s32 arg0, s16 arg1)
{
  s32 temp_v0;
  s32 temp_v1;
  void *temp_s0;
  RecordCode8025A3EC *slot;
  OwnerCode8025A3EC8025B5F0 *owner;
  temp_v0 = (arg1 * 0xCC) + arg0;
  temp_v0 = temp_v0 + 4;
  slot = (RecordCode8025A3EC *)temp_v0;
  temp_v1 = slot->owner;
  owner = (OwnerCode8025A3EC8025B5F0 *)temp_v1;
  temp_s0 = owner->sound;
  func_00293E60(temp_s0, owner->samples[slot->index]);
  func_00293580(temp_s0);
}
