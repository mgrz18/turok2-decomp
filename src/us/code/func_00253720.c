
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
s32 CPlayer__GetPickupCountLow(s32 arg0, s32 arg1)
{
  unsigned long long new_var;
  new_var = 1;
  if (!(gEngineApp_m_Settings & 0x10))
  {
    return (*((u8 *) (((s8 *) (arg1 + arg0)) + (-0xA74)))) & 0xF;
  }
  if (new_var)
  {
    return new_var;
  }
  else
  {
    return new_var;
  }
}
