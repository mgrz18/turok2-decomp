#include "common.h"
#include "structs/player.h"

extern s32 D_80130970;          /* the character of the pak profile D_80130968 (structs/pak.h), after its 4 counts */
extern u8 D_80130974[];

/* Copies the saved values into the options of the player. */
void func_004214C4(CPlayer *arg0)
{
  s32 var_a0;
  u8 temp_v0;
  CPlayerOptions *temp_a1;
  CPlayerOptions *var_v1;
  temp_a1 = arg0->m_pOptions;
  temp_a1->m_nCharacter = *(u8 *) &D_80130970;
  temp_a1->m_Suicides = ((u16 *) &D_80130970)[-4];
  temp_a1->m_Deaths = ((u16 *) &D_80130970)[-3];
  temp_a1->m_Kills = ((u16 *) &D_80130970)[-2];
  var_a0 = 0;
  temp_a1->m_Tags = ((u16 *) &D_80130970)[-1];
  var_v1 = temp_a1;
  do
  {
    temp_v0 = D_80130974[var_a0];
    var_v1 = (CPlayerOptions *) ((u8 *) temp_a1 + var_a0);
    var_a0 += 1;
    var_v1->m_Name[0] = temp_v0;
  }
  while (var_a0 < 8);
}
