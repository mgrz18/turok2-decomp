#include "common.h"
#include "globals/engine.h"

s32 osJamMesg(s32, s32, s32);
s32 osRecvMesg(s32, s32, s32);
s32 osSetThreadPri(s32, s32);
s32 osCreateMesgQueue(s32 *, s32 *, s32);
s32 CDecompressor__DecompressSync(s32 arg0, void *arg1)
{
  s32 new_var3;
  u8 sp10[0x18];
  s32 sp28;
  unsigned char new_var2;
  s32 sp2C;
  s32 *new_var;
  new_var2 = 1;
  new_var = &sp28;
  osCreateMesgQueue(sp10, new_var, new_var2);
  new_var3 = gThreadPriorityDecompressorHigh;
  *((s32 **) (((s8 *) arg1) + 0x20)) = sp10;
  gDecompressorBoostRetraces = 2;
  osSetThreadPri(arg0, new_var3);
  osJamMesg(arg0 + 0x230, (s32) arg1, 1);
  osRecvMesg((s32) sp10, (s32) (&sp2C), new_var2);
  return 1;
}
