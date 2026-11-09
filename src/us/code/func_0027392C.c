/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025A844_de_4.c, func_8025B5F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void alSndpSetSound(void *sound, s16 sample);
extern s32 alSndpGetState(void *sound);
extern void alSndpStop(void *sound);
extern void alSndpSetSound(void *sound, s16 sample);
extern s32 alSndpGetState(void *sound);
extern void alSndpStop(void *sound);
extern void alSndpSetSound(void *arg0, s16 arg1);
extern s32 alSndpGetState(void *arg0);
extern void alSndpStop(void *arg0);
#ifndef TARGET_PORT
void SndVoicePool_StopAllExcept(RecordRecAt4V6 *record, s32 value)
{
  s32 i;
  s32 active;
  s32 absent;
  SlotRecAtB0 *slot = record->slots;
  i = 0;
  absent = -1;
  active = 1;
  for (; i < 16; slot++, i++)
  {
    if (((slot->used != absent) && (record->header->local != i)) && ((value == absent) || (slot->value != value)))
    {
      OwnerCode8025A3EC8025B5F0 *owner = slot->owner;
      void *sound;
      slot->active = active;
      slot->flag = 0;
      if (slot->key != owner->key)
      {
        sound = owner->sound;
        alSndpSetSound(sound, owner->samples[slot->index]);
        if (alSndpGetState(sound) != 0)
        {
          if (active)
          {
            alSndpStop(sound);
          }
          else
          {
            alSndpStop(sound);
          }
        }
        slot->state = absent;
      }
    }
  }
}
#else
#include "structs/sound.h"
/* The port: the same on the typed pool (the Rage Wars view above has N64 offsets: slot.used =
 * unk8, key = vp.frame, flag = vp.follow, value = soundId, active = stopping, owner = mgr;
 * header->local = pendingCue, owner->sound = player, samples = handles, owner->key = frame). */
void SndVoicePool_StopAllExcept(SndVoicePool *pool, s32 value) {
    s32 i;
    SndVoice *v = pool->voices;
    for (i = 0; i < 16; v++, i++) {
        if (v->unk8 != -1 && pool->owner->pendingCue != i && (value == -1 || v->soundId != value)) {
            CAudioWorld *m = v->mgr;
            v->stopping = 1;
            v->vp.follow = NULL;
            if (v->vp.frame != m->frame) {
                alSndpSetSound(&m->player, m->handles[v->index]);
                if (alSndpGetState(&m->player) != 0) {
                    alSndpStop(&m->player);
                }
                v->sound = -1;
            }
        }
    }
}
#endif
