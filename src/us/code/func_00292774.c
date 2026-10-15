
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
typedef s32 M2C_UNK;
typedef s8 M2C_UNK8;
typedef s16 M2C_UNK16;
typedef s32 M2C_UNK32;
typedef s64 M2C_UNK64;
s32 func_00292774(void *arg0, void *arg1)
{
  int new_var;
  new_var = ((s32) (((*((u8 *) (((s8 *) arg0) + 0x36))) * (*((u8 *) (((s8 *) arg0) + 0x33)))) * (*((u8 *) (((s8 *) arg0) + 0x30))))) >> 6;
  new_var = new_var * (((s32) (((*((u8 *) (((s8 *) (*((void **) (((s8 *) arg0) + 0x20)))) + 0xD))) * (*((s16 *) (((s8 *) arg1) + 0x32)))) * (*((u8 *) (((s8 *) (((*((u8 *) (((s8 *) arg0) + 0x31))) * 0x10) + (*((s32 *) (((s8 *) arg1) + 0x60))))) + 9))))) >> 0xE);
  return ((s32) (new_var * 2)) >> 0x10;
}
