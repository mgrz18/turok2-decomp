#include "common.h"

void func_0026EDA8(s32);
s32 func_00285410(s32);
s32 func_0025359C(s32 *);                   /* extern */

/* The multiplayer match state at D_8011ACB0: the fields this resets. */
typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s32 teams;
    /* 0x28 */ u8 pad28[0x34 - 0x28];
    /* 0x34 */ s32 split;
    /* 0x38 */ u8 pad38[0x44 - 0x38];
    /* 0x44 */ s32 unk44;
} MatchState;

/* The game/menu context this runs on. */
typedef struct {
    /* 0x00000 */ u8 pad0[0x23FDC];
    /* 0x23FDC */ s32 unk23FDC;
    /* 0x23FE0 */ u8 pad23FE0;
    /* 0x23FE1 */ s8 unk23FE1;
} Context;

extern s32 D_800F5A50;
extern s32 D_80119870;
extern MatchState D_8011ACB0;

void func_00469250(Context *arg0)
{
  MatchState *new_var;
  func_00285410(0);
  func_0026EDA8((s32) (&D_800F5A50));
  func_0025359C(&D_80119870);
  new_var = &D_8011ACB0;
  new_var->teams = 0;
  D_8011ACB0.split = 0;
  D_8011ACB0.unk44 = 0;
  arg0->unk23FE1 = 2;
  arg0->unk23FDC = 1;
}
