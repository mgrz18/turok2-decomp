typedef signed char s8;
typedef signed int s32;

typedef struct {
    /* 0x0 */ s32 active;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s32 value;
} State;

extern State D_800B8960;

s32 osPiGetCmdQueue(void)
{
  s32 *new_var;
  s32 new_var2;
  if (D_800B8960.active != 0)
  {
    if (new_var)
    {
      new_var = &D_800B8960.value;
      ;
      return *new_var;
    }
    else
    {
      new_var = &D_800B8960.value;
      ;
      return *new_var;
    }
  }
  return 0;
}
