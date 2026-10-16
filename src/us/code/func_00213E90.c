/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802773D4_de_3.c, func_80278890_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
struct Vec3;
typedef struct Vec3 Vec3;
struct Vec3;
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};
struct ObjectRecAtBC;
struct ObjectRecAtBC {
    f32 falloff;
    f32 value;
    Vec3 direction;
    Vec3 axis;
    char pad20[0xB8-0x20];
    u8 flagB8;
    u8 flagB9;
    char padBA;
    u8 shape;
    u8 animationSeed;
};
struct ObjectRecAtBC;
typedef struct ObjectRecAtBC ObjectRecAtBC;
extern s8 D_800ACAF4[];
extern s32 D_800B6D18;
f32 func_00213E90(void *arg0)
{
  ObjectRecAtBC *volume = arg0;
  u32 temp_a0;
  s32 idx;
  temp_a0 = volume->animationSeed;
  idx = D_800B6D18 + ((((temp_a0 % 7) & 0xFF) + 1) * 0x10);
  idx = idx % 90;
  return (f32) D_800ACAF4[idx];
}
extern s32 D_800B6D18;
