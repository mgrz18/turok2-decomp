#include "common.h"
#include "structs/sound.h"
#include "protos/ultra.h"

s32 __osDisableInt(void);
void __osRestoreInt(s32);
/* How many of the manager's voices play the given sound (CMutex taken
 * around the scan). */
s32 CAudioWorld__CountHandleVoices(CAudioWorld *mgr, s32 sound)
{
  s32 mask;
  s32 mask2;
  s32 count;
  s32 count2;
  s32 i;
  s32 n;
  CMutex *lock;
  CMutex *lock2;
  CAudioWorld *m;
  lock = &mgr->lock;
  n = 0;
  mask = __osDisableInt();
  count = lock->count + 1;
  lock->count = count;
  if (count != 1)
  {
    __osRestoreInt(mask);
    osRecvMesg(lock, 0, 1);
    i = 0;
  }
  else
  {
    __osRestoreInt(mask);
    i = 0;
  }
 do { m = mgr; } while (0); /* walked by whole voices, the sound field read at a fixed offset: the ROM's loop */
  do
  {
    if (m->pool.voices[0].sound == sound)
    {
      n += 1;
    }
    i += 1;
    m = (CAudioWorld *) ((u8 *) m + sizeof(SndVoice));
  }
  while (i < 0x11);
  lock2 = &mgr->lock;
  mask2 = __osDisableInt();
  count2 = lock2->count - 1;
  lock2->count = count2;
  if (count2 != 0)
  {
    __osRestoreInt(mask2);
    osSendMesg(lock2, 0, 1);
    return n;
  }
  if (mgr)
  {
    __osRestoreInt(mask2);
    return n;
  }
  else
  {
    __osRestoreInt(mask2);
    return n;
  }
}
