/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040184C.c, func_0040184C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef float f32;
typedef struct O01Ctx {
               u8 pad0[4];
               void *tree;
               u8 pad8[0x10];
               s32 unk18;
               u8 pad1C[0x1C];
               s32 unk38;
               u8 pad3C[0x1C];
               s32 unk58;
               u8 pad5C[0x58];
               s32 unkB4;
               u8 padB8[0x20];
               s32 unkD8;
               s32 unkDC;
               s32 unkE0;
} O01Ctx;
typedef struct O01Node {
              s32 unk0;
              s32 unk4;
} O01Node;
typedef struct O01Key8 { f32 val; f32 time; } O01Key8;
extern O01Ctx *D_800C1BB0;
extern f32 D_800C0994;
extern f32 D_800C0998;
extern f32 D_800C099C;
void *func_002017D4(void *tree, s32 idx);
f32 func_00449BB4(f32 t) {
  O01Node *n = func_002017D4(D_800C1BB0->tree, 3);
  register O01Key8 *keys asm("$5");
  register s32 cnt asm("$4");
  f32 s; s32 i;
  keys = (O01Key8 *) (((u8 *) n) + 8);
  cnt = n->unk4;
  for (i = 0; i < cnt; i++) { }
  if (cnt == 0) { return D_800C0994; }
  if (t <= keys[0].time) { return keys[0].val; }
  { register s32 off asm("$2"); off = cnt << 3;
  if (((O01Key8 *)(off + (s32)keys))[-1].time <= t) { return ((O01Key8 *)(off + (s32)keys))[-1].val; } }
  if (keys[0].time < t) { keys++; while (keys->time < t) { keys++; } }
  s = (t - keys[-1].time) / (keys->time - keys[-1].time);
  s = (s * (s * D_800C0998)) - (((s + s) * s) * s);
  return (keys[-1].val * (D_800C099C - s)) + (keys->val * s);
}
