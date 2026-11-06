
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
s32 CAI__SetMode(s32, s32, s32);
s32 COnScreen__SetBoss(s32 *, s32, s32 *);
extern s32 sPrimagenSpawnScript1_2; /* local type: BossStep2[] in globals/boss.h; this file typedefs its own s8...f64 instead of common.h */
extern s32 sPrimagenSoundScript3_2; /* local type: BossStep3[] in globals/boss.h (same reason) */
extern s32 D_800C046C;
extern s32 gEngineApp_m_OnScreen;
extern s32 gLevelScript_script3_cursor;
void Primagen_UpdateMode28_2(s32 arg0, s32 arg1)
{
  COnScreen__SetBoss(&gEngineApp_m_OnScreen, arg0, &D_800C046C);
  *((s32 *) (((s8 *) (&gLevelScript_script3_cursor)) + 0)) = -1;
  *((s32 **) (((s8 *) (&gLevelScript_script3_cursor)) + (-4))) = &sPrimagenSoundScript3_2;
  *((s8 *) (((s8 *) (&gLevelScript_script3_cursor)) + (-0xC))) = 1;
  *((s32 *) (((s8 *) (&gLevelScript_script3_cursor)) + 4)) = -1;
  *((s32 *) (((s8 *) (&gLevelScript_script3_cursor)) + (-0x14))) = -1;
  *((s32 *) (((s8 *) (&gLevelScript_script3_cursor)) + (-0x10))) = -1;
  *((s32 **) (((s8 *) (&gLevelScript_script3_cursor)) + (-0x18))) = &sPrimagenSpawnScript1_2;
  CAI__SetMode(arg0, arg1, 5);
}
