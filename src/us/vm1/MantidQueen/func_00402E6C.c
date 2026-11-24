#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/ai.h"
#include "protos/player.h"

#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define AI_AngleToPoint AI_AngleToPoint_i3
#endif
f32 AI_AngleToPoint(s32, s32, s32, s32);

s32 ScriptAnimator_SetScript(void *, s32 *);           /* extern */
extern s32 sMantidQueenMode6SpotScript; /* local type: f32[] in globals/boss.h, whose structs/boss.h clashes with this file's u8 [] gMantidQueen */
extern u8 gMantidQueen[]; /* local type: globals/boss.h declares it; this view (its type here) is what keeps the ROM's code */

#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/player.h"
#include "structs/boss.h"
#undef AI_AngleToPoint
extern f32 AI_AngleToPoint(void *actor, CVector3 point);

/* Host body: the ROM reads the actor's yaw (0x50) and flags (0xD4), the CAI's
 * m_AnimRepeatedCount (0xC7), the player's position (4..0xC) and gMantidQueen's boss (0x220),
 * step (0x214) and weak spots (0x8 + 0x68 * i, the animator at +4) at N64 offsets. */
void MantidQueen_UpdateMode6(sptr arg0, void *arg1)
{
  CGameObjectInstance *obj = (CGameObjectInstance *) arg0;
  CAI *ai = (CAI *) arg1;
  WaveBossState *s = (WaveBossState *) gMantidQueen; /* the u8[] view declared above */
  CGameObjectInstance *player;
  s32 spot;

  player = (CGameObjectInstance *) CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
  if (player != NULL)
  {
    obj->m_RotY += AI_AngleToPoint(obj, player->header.m_vPos);
  }
  obj->m_dwFlags |= 0x2000;
  if (ai->m_AnimRepeatedCount != 0)
  {
    AI_PlayAnim((sptr) s->boss, &s->boss->m_AI, 0x5ADC);
    CAI__SetMode(obj, ai, 7);
    spot = s->step->weakSpot;
    if (spot != -1)
    {
      ScriptAnimator_SetScript(&s->spots[spot].anim, &sMantidQueenMode6SpotScript);
    }
  }
}
#else
void MantidQueen_UpdateMode6(sptr arg0, void *arg1)
{
  s32 *var_s1;
  sptr var_s0;
  void *temp_v0;
  void *var_s2;
  s32 temp_v1;
  int new_var;
  var_s0 = arg0;
  var_s2 = arg1;
  temp_v0 = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
  var_s1 = &gMantidQueen;
  var_s1 = &gMantidQueen;
  if (temp_v0 != 0)
  {
    new_var = 0xC;
    *((f32 *) (((s8 *) var_s0) + 0x50)) = (f32) ((*((f32 *) (((s8 *) var_s0) + 0x50))) + AI_AngleToPoint(var_s0, *((s32 *) (((s8 *) temp_v0) + 4)), *((s32 *) (((s8 *) temp_v0) + 8)), *((s32 *) (((s8 *) temp_v0) + new_var))));
  }
  *((s32 *) (((s8 *) var_s0) + 0xD4)) = (s32) ((*((s32 *) (((s8 *) var_s0) + 0xD4))) | 0x2000);
  if ((*((s8 *) (((s8 *) var_s2) + 0xC7))) != 0)
  {
    AI_PlayAnim(*((s32 *) (((s8 *) (&gMantidQueen)) + 0x220)), (*((s32 *) (((s8 *) (&gMantidQueen)) + 0x220))) + 0x140, 0x5ADC);
    CAI__SetMode(var_s0, var_s2, 7);
    temp_v1 = *((s32 *) (((s8 *) (*((void **) (((s8 *) (&gMantidQueen)) + 0x214)))) + 0x14));
    if (temp_v1 != (-1))
    {
      ScriptAnimator_SetScript((temp_v1 * 0x68) + ((u8 *) (gMantidQueen + new_var)), &sMantidQueenMode6SpotScript);
    }
  }
}
#endif
