#include "common.h"
#include "structs/game.h"

s32 CAI__SetMode(s32, s32, s32);
f32 AI_AngleToPoint(s32, s32, s32, s32);

s32 ScriptAnimator_SetScript(void *, s32 *);           /* extern */
void *CPlayerPool__GetFirstActive(CPlayerList *);                     /* extern */
s32 AI_PlayAnim(s32, s32, s32);           /* extern */
extern s32 D_00430954;
extern u8 D_8012F3A0[];

void MantidQueen_UpdateMode6(s32 arg0, void *arg1)
{
  s32 *var_s1;
  s32 var_s0;
  void *temp_v0;
  void *var_s2;
  s32 temp_v1;
  int new_var;
  var_s0 = arg0;
  var_s2 = arg1;
  temp_v0 = CPlayerPool__GetFirstActive(&D_80119870.m_PlayerList);
  var_s1 = &D_8012F3A0;
  var_s1 = &D_8012F3A0;
  if (temp_v0 != 0)
  {
    new_var = 0xC;
    *((f32 *) (((s8 *) var_s0) + 0x50)) = (f32) ((*((f32 *) (((s8 *) var_s0) + 0x50))) + AI_AngleToPoint(var_s0, *((s32 *) (((s8 *) temp_v0) + 4)), *((s32 *) (((s8 *) temp_v0) + 8)), *((s32 *) (((s8 *) temp_v0) + new_var))));
  }
  *((s32 *) (((s8 *) var_s0) + 0xD4)) = (s32) ((*((s32 *) (((s8 *) var_s0) + 0xD4))) | 0x2000);
  if ((*((s8 *) (((s8 *) var_s2) + 0xC7))) != 0)
  {
    AI_PlayAnim(*((s32 *) (((s8 *) (&D_8012F3A0)) + 0x220)), (*((s32 *) (((s8 *) (&D_8012F3A0)) + 0x220))) + 0x140, 0x5ADC);
    CAI__SetMode(var_s0, (s32) var_s2, 7);
    temp_v1 = *((s32 *) (((s8 *) (*((void **) (((s8 *) (&D_8012F3A0)) + 0x214)))) + 0x14));
    if (temp_v1 != (-1))
    {
      ScriptAnimator_SetScript((temp_v1 * 0x68) + ((u8 *) (D_8012F3A0 + new_var)), &D_00430954);
    }
  }
}
