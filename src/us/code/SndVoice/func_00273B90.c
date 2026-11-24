/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025A844_de_4.c, func_8025B854_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "protos/sndp.h"

extern s32 SndVoicePool_GetVoiceState(s32 arg0, s16 arg1);
s32 SndVoicePool_GetVoiceState(s32 arg0, s16 arg1)
{
  sptr temp_v0;
  sptr temp_v1;
  void *temp_s0;
  RecordCode8025A3EC *slot;
  OwnerCode8025A3EC8025B5F0 *owner;
  temp_v0 = (arg1 * 0xCC) + arg0;
  temp_v0 = temp_v0 + 4;
  slot = (RecordCode8025A3EC *)temp_v0;
  temp_v1 = slot->owner;
  owner = (OwnerCode8025A3EC8025B5F0 *)temp_v1;
  temp_s0 = owner->sound;
  alSndpSetSound(temp_s0, owner->samples[slot->index]);
  alSndpGetState(temp_s0);
}
