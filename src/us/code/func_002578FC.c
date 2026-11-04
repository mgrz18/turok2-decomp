#include "common.h"

/* The player: the fields this reads. */
typedef struct {
    /* 0x000 */ u8 pad0[0x9F8];
    /* 0x9F8 */ s32 mask;               /* one bit per value 1..6 of func_0026EFB8 */
    /* 0x9FC */ u8 pad9FC[0xA50 - 0x9FC];
    /* 0xA50 */ s32 controller;
    /* 0xA54 */ u8 padA54[0xC00 - 0xA54];
    /* 0xC00 */ s32 pending;
} Player;

s32 func_0026EFB8(s32);
s32 func_00416644(s32, s32, s32, s32, s32);
extern s32 D_0043CAB4;
extern s32 D_80110048;
extern s32 D_80119E2C;

void func_002578FC(s32 arg0, Player *arg1)
{
  int new_var;
  u32 temp_a0;
  if (arg1->pending != 0)
  {
    temp_a0 = func_0026EFB8(D_80110048) - (new_var = 1);
    if ((temp_a0 < 6U) && (arg1->mask & (new_var << temp_a0)))
    {
      func_00416644((s32) (&D_80119E2C), (s32) (&D_0043CAB4), (s32) arg1, arg1->controller, 0);
    }
    arg1->pending = 0;
  }
}
