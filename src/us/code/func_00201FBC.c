#include "common.h"

s32 osPiRawStartDma(s32, s32, s32, s32);
s32 osRecvMesg(s32, s32, s32);
s32 osSendMesg(s32, s32, s32);
void osInvalDCache(s32, s32);
void osWritebackDCache(s32, s32);

/* A queued ROM->RAM load request. */
typedef struct {
    /* 0x00 */ s32 romAddr;
    /* 0x04 */ s32 ramAddr;
    /* 0x08 */ s32 size;
    /* 0x0C */ s32 doneQueue;
    /* 0x10 */ s32 doneMsg;
} LoaderEntry;

s32 CLoader__DeallocLoaderEntry(s32, LoaderEntry *);                 /* extern */

/* Loader thread: takes requests off the queue at +0x230, DMAs them in and
 * waits for completion on the queue at +0xA48. */
void func_00201FBC(s32 arg0)
{
  LoaderEntry *sp10;
  s32 sp14;
  s32 temp_a0;
  s32 var_a0;
  var_a0 = arg0 + 0x230;
  loop_1:
  osRecvMesg(arg0 + 0x230, (s32) (&sp10), 1);
  osWritebackDCache(sp10->ramAddr, sp10->size);
  osPiRawStartDma(0, sp10->romAddr, sp10->ramAddr, sp10->size);
  osRecvMesg(arg0 + 0xA48, (s32) (&sp14), 1);
  osInvalDCache(sp10->ramAddr, sp10->size);
  temp_a0 = sp10->doneQueue;
  if (temp_a0 != 0)
  {
    osSendMesg(temp_a0, sp10->doneMsg, 1);
  }
  CLoader__DeallocLoaderEntry(arg0, sp10);
  var_a0 = arg0 + 0x230;
  goto loop_1;
}
