#include "common.h"
#include "structs/game.h"

void func_0026EDA8(s32);
s32 func_00285410(s32);
s32 func_0025359C(s32 *);                   /* extern */

/* The multiplayer match state at D_8011ACB0: the fields this resets. */

/* The game/menu context this runs on. */
typedef struct {
    /* 0x00000 */ u8 pad0[0x23FDC];
    /* 0x23FDC */ s32 unk23FDC;
    /* 0x23FE0 */ u8 pad23FE0;
    /* 0x23FE1 */ s8 unk23FE1;
} Context;

extern s32 D_800F5A50;
extern s32 D_80119870;

void func_00469250(Context *arg0)
{
  GameMatch *new_var;
  func_00285410(0);
  func_0026EDA8((s32) (&D_800F5A50));
  func_0025359C(&D_80119870);
  new_var = &D_8011ACB0;
  new_var->teams = 0;
  D_8011ACB0.tag = 0;
  D_8011ACB0.unk44 = 0;
  arg0->unk23FE1 = 2;
  arg0->unk23FDC = 1;
}
