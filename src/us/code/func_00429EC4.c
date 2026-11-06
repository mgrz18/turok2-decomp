typedef unsigned char u8;
typedef signed int s32;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 padC[0x65 - 0xC];
    /* 0x65 */ u8 fill;
} Arg0;

s32 __osContRamWrite(s32, s32, s32, s32, s32);

/* Writes 0x20 bytes of the fill value at offset 0x400. */
void __osPfsSelectBank(Arg0 *arg0)
{
  u8 sp18[0x20];
  s32 var_a0;
  u8 *var_v1;
  var_a0 = 0;
  var_v1 = sp18;
  do
  {
    var_v1 = &sp18[var_a0];
    var_a0 += 1;
    *var_v1 = arg0->fill;
  }
  while (var_a0 < 0x20);
  __osContRamWrite(arg0->unk4, arg0->unk8, 0x400, (s32) sp18, 0);
}
