#include "common.h"

#ifdef TARGET_PORT
/* the host layout: the queue and its items through the real types (structs/libaudio.h) */
#include "structs/libaudio.h"
void alLink(ALLink *ln, ALLink *to);
void alUnlink(ALLink *ln);
void alCopy(void *src, void *dst, s32 size);
s32 osSetIntMask(s32);
ALMicroTime alEvtqNextEvent(ALEventQueue *evtq, ALEvent *evt) {
    ALEventListItem *item;
    ALMicroTime delta;
    s32 mask = osSetIntMask(1);
    item = (ALEventListItem *)evtq->allocList.next;
    if (item != 0) {
        alUnlink(&item->node);
        alCopy(&item->evt, evt, sizeof(*evt));
        alLink(&item->node, &evtq->freeList);
        delta = item->delta;
    } else {
        evt->type = -1;
        delta = 0;
    }
    osSetIntMask(mask);
    return delta;
}
#else

s32 alLink(s32, s32);
s32 osSetIntMask(s32);

s32 alCopy(sptr, s16 *, s32);         /* extern */
s32 alUnlink(sptr);                         /* extern */

s32 alEvtqNextEvent(void *arg0, s16 *arg1)
{
  sptr temp_s0;
  s32 temp_s2;
  s32 var_s0;
  unsigned int new_var;
  new_var = osSetIntMask(1);
  temp_s0 = *((s32 *) (((s8 *) arg0) + 8));
  temp_s2 = new_var;
  if (temp_s0 != 0)
  {
    alUnlink(temp_s0);
    alCopy(temp_s0 + 0xC, arg1, 0x10);
    alLink(temp_s0, (sptr) arg0);
    var_s0 = *((s32 *) (((s8 *) temp_s0) + 8));
  }
  else
  {
    *arg1 = -1;
    var_s0 = 0;
  }
  osSetIntMask(temp_s2);
  return var_s0;
}
#endif
