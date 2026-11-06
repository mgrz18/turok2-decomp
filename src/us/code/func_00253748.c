
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
extern s32 gEngineApp_m_Settings;
u32 CPlayer__GetPickupCountHigh(s32 arg0, s32 arg1)
{
  s32 new_var;
  if (!(gEngineApp_m_Settings & 0x10))
  {
    if (new_var)
    {
      new_var = arg0;
      return ((u8) (*((u8 *) (((s8 *) (arg1 + new_var)) + (-0xA74))))) >> 4;
    }
    else
    {
      new_var = arg0;
      return ((u8) (*((u8 *) (((s8 *) (arg1 + new_var)) + (-0xA74))))) >> 4;
    }
  }
  return 1U;
}
