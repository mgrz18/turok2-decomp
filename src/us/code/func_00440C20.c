
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
extern s32 D_004312D4;
extern s32 D_004315BC;
extern s32 D_800C046C;
extern s32 D_801100F0;
extern s32 D_8012FA24;
void Primagen_UpdateMode28_2(s32 arg0, s32 arg1)
{
  COnScreen__SetBoss(&D_801100F0, arg0, &D_800C046C);
  *((s32 *) (((s8 *) (&D_8012FA24)) + 0)) = -1;
  *((s32 **) (((s8 *) (&D_8012FA24)) + (-4))) = &D_004315BC;
  *((s8 *) (((s8 *) (&D_8012FA24)) + (-0xC))) = 1;
  *((s32 *) (((s8 *) (&D_8012FA24)) + 4)) = -1;
  *((s32 *) (((s8 *) (&D_8012FA24)) + (-0x14))) = -1;
  *((s32 *) (((s8 *) (&D_8012FA24)) + (-0x10))) = -1;
  *((s32 **) (((s8 *) (&D_8012FA24)) + (-0x18))) = &D_004312D4;
  CAI__SetMode(arg0, arg1, 5);
}
