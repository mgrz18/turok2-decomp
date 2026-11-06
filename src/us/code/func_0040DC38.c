
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
s32 AI_UpdateTargets(s32, s32, s32, s32, s32);
void BlindOne_BossMode3Update(s32 arg0, s32 arg1)
{
  u8 sp18[0x88];
  AI_UpdateTargets(arg0, arg1, (s32) (&sp18), 0, 1);
  if ((*((f32 *) (((s8 *) arg1) + 0x40))) > (*((f32 *) (((s8 *) arg1) + 0x64))))
  {
    CAI__SetMode(arg0, arg1, 5);
  }
}
