/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002917E8.c, func_002917E8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

extern DlCmd *D_800D8E24;
extern B21Hdr *D_800F7070;
extern u8 D_800AB100[];
extern u8 D_800AB130[];
extern s32 D_80130A78;
extern B21Vp D_8011B0A8[];
extern s32 D_800B6D1C;
extern s32 D_800C2030;
extern s32 D_800C2034;
void func_00286A04(void)
{
  f32 new_var;
  s32 new_var2;
  s32 new_var3;
  DlCmd *d1;
  DlCmd *d2;
  DlCmd *d3;
  DlCmd *d4;
  DlCmd *d5;
  DlCmd *d6;
  B21Vp *vp;
  D_800D8E24 = D_800F7070->unk114;
  d1 = D_800D8E24++;
  d1->w0 = 0xDB060000;
  d1->w1 = 0;
  d2 = D_800D8E24++;
  d2->w0 = 0xDE000000;
  d2->w1 = (u32) D_800AB100;
  d3 = D_800D8E24++;
  d3->w0 = 0xDE000000;
  d3->w1 = (u32) D_800AB130;
  d4 = D_800D8E24++;
  d4->w0 = 0xFE000000;
  vp = &D_8011B0A8[D_800B6D1C];
  d4->w1 = D_80130A78;
  vp->vscale[1] = D_800C2034 * 2;
  vp->vscale[0] = D_800C2030 * 2;
  vp->vscale[2] = 0x3FF;
  vp->vtrans[1] = D_800C2034 * 2;
  vp->vscale[2] = 0x3FF;
  vp->vscale[3] = 0;
  vp->vtrans[0] = D_800C2030 * 2;
  vp->vtrans[2] = 0;
  vp->vtrans[3] = 0;
  new_var = (f32) (D_800C2030 - 1);
  d5 = D_800D8E24++;
  d5->w0 = 0xDC080008;
  d5->w1 = (u32) vp;
  d6 = D_800D8E24++;
  d6->w0 = 0xED000000;
  d6->w1 = ((((s32) (new_var * 4.0f)) & 0xFFF) << 12) | (((s32) (((f32) (D_800C2034 - 1)) * 4.0f)) & 0xFFF);
}
