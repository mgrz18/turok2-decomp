typedef signed char s8;
typedef signed int s32;

typedef struct {
    /* 0x00 */ s8 pad0[0x38];
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s8 pad3C[0xA8 - 0x3C];
    /* 0xA8 */ s32 unkA8;
} Arg;

extern Arg *D_800C1BB0;

s32 func_00284358(void)
{
  if (D_800C1BB0->unk38 != 0)
  {
    if (D_800C1BB0)
    {
      return D_800C1BB0->unkA8;
    }
    else
    {
      return D_800C1BB0->unkA8;
    }
  }
  return 0;
}
