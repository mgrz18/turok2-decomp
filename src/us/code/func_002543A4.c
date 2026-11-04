#include "common.h"
#include "structs/game_object.h"

/* The region the player is in, as read here (flags at 0x52, as in
 * func_0024E9D0.c). */
typedef struct {
    /* 0x00 */ u8 pad0[0x52];
    /* 0x52 */ u16 flags52;
} RegionInfo;

/* The player, as read here: the field at 0xBE4 (CPlayer's header is off
 * by a few words from 0xA08 on, so its name there is not used). */
typedef struct {
    /* 0x000 */ u8 pad0[0xBE4];
    /* 0xBE4 */ s32 unkBE4;
} Player;

void func_00254430(void *, void *);                 /* extern */
void func_00254584(void *, void *);                 /* extern */

void func_002543A4(Player *arg0, CGameObjectInstance *arg1, RegionInfo *arg2)
{
  int new_var;
  s32 temp_s0;
  s32 temp_s2;
  s32 temp_v1;
  if (arg2 != 0)
  {
    temp_v1 = arg1->m_AI.m_dwFlags;
    temp_s2 = arg2->flags52 & 2;
    new_var = temp_v1 & 0x100;
    temp_s0 = temp_v1 & 0x80;
    if (temp_s0 == 0)
    {
      arg0->unkBE4 = 0;
    }
    if ((!new_var) && (temp_s2 != 0))
    {
      func_00254584(arg0, arg1);
    }
    if ((temp_s0 != 0) && (temp_s2 == 0))
    {
      func_00254430(arg0, arg1);
    }
  }
}
