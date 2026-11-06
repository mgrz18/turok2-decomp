#include "common.h"

s32 CAI__SetMode(s32, s32, s32);
s32 COnScreen__SetBoss(s32, s32, s32);

extern s32 sPrimagenSpawnScript1;
extern s32 sPrimagenSoundScript3;
extern s32 D_800C046C;
extern s32 gEngineApp_m_OnScreen;
extern s32 gLevelScript_script3_cursor;

void Primagen_UpdateMode28(s32 arg0, int arg1)
{
  s32 *new_var3;
  s32 **new_var2;
  int new_var;
  COnScreen__SetBoss((s32) (&gEngineApp_m_OnScreen), arg0, (s32) (&D_800C046C));
  *((s32 *) (((s8 *) (&gLevelScript_script3_cursor)) + 0)) = -1;
  *((s32 **) (((s8 *) (&gLevelScript_script3_cursor)) + (-4))) = &sPrimagenSoundScript3;
  *((s8 *) (((s8 *) (&gLevelScript_script3_cursor)) + (-0xC))) = 1;
  *((s32 *) (((s8 *) (&gLevelScript_script3_cursor)) + 4)) = -1;
  new_var3 = (s32 *) ((-0x14) + ((s8 *) (&gLevelScript_script3_cursor)));
  *new_var3 = -1;
  *((s32 *) (((s8 *) (&gLevelScript_script3_cursor)) + (new_var = -0x10))) = -1;
  *(new_var2 = (s32 **) (((s8 *) (&gLevelScript_script3_cursor)) + (-0x18))) = &sPrimagenSpawnScript1;
  CAI__SetMode(arg0, arg1, 5);
}
