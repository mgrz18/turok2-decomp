/* Second-module copy of BlindOne_UpdateFight (src/us/code/BlindOne_UpdateFight.c), with its string/data addresses 0xE0 lower. */

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
s32 Animator_Update(s32);
s32 BlindOne_UpdateMover(s32);
s32 BlindOne_GetHealthPercent(s32);
s32 COnScreen__SetBossPercent(s32 *, s32);
extern s32 D_00432330;
extern s32 D_801100F0;
void BlindOne_UpdateFight_2(void *arg0)
{
  s32 (*temp_v0)();
  s32 var_s0;
  s32 var_s1;
  temp_v0 = *((typeof(&D_00432330)) (((s8 *) (&D_00432330)) + ((*((s32 *) (((s8 *) arg0) + 0x4FC))) * 8)));
  if (temp_v0 != 0)
  {
    temp_v0();
  }
  var_s0 = 0;
  if ((*((s32 *) (((s8 *) arg0) + 0x4EC))) > 0)
  {
    var_s1 = 0x3C;
    do
    {
      Animator_Update((s32) (arg0 + var_s1));
      var_s0 += 1;
      var_s1 += 0x64;
      var_s0++;
      var_s0--;
    }
    while (var_s0 < (*((s32 *) (((s8 *) arg0) + 0x4EC))));
  }
  BlindOne_UpdateMover((s32) (arg0 + 0x518));
  BlindOne_UpdateMover((s32) (arg0 + 0x530));
  BlindOne_UpdateMover((s32) (arg0 + 0x548));
  COnScreen__SetBossPercent(&D_801100F0, BlindOne_GetHealthPercent((s32) arg0));
}
