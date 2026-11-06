/* Update one sound: refresh its volume, notice when it stopped or must stop, then slide its volume, pan and pitch. */

#include "common.h"
#include "structs/vector.h"
#include "structs/sound.h"

/* The bank seen from 0x7C: the ROM addresses its sound ids from there. */
typedef struct SoundVoice {
    /* 0x00 */ u8 pad0[0x60];
    /* 0x60 */ s16 ids[1];                     /* SndMgr.handles */
} SoundVoice;

extern u8 D_801198B0[];

extern s32 CCameraPool__GetNearestCamera(void *game, CVector3 *pos);
extern void alSndpSetSound(void *player, s32 id);    /* alSndpSetSound */
extern s32 alSndpGetState(void *player);             /* alSndpGetState */
extern void alSndpStop(void *player);            /* alSndpStop */
extern void alSndpPlay(void *player);
extern void alSndpDeallocate(void *player, s32 id);    /* alSndpDeallocate */
extern void SndVoice_UpdateVolume(SndVoice *s);
extern void SndVoice_UpdatePan(SndVoice *s);
extern void SndVoice_UpdatePitch(SndVoice *s);

void SndVoice_Update(SndVoice *s) {
    SndMgr *bank;
    s32 playing;
    void *player;

    s->mgr->listener = CCameraPool__GetNearestCamera(D_801198B0, &s->vp.pos);
    bank = s->mgr;
    alSndpSetSound(&bank->player, bank->handles[s->index]);
    playing = alSndpGetState(&bank->player);
    if ((s->vp.scale <= 0.075f && s->soundId != 0x228) || ((s->flags & 4) && s->vp.vol == 0)) {
        s->stopping = 1;
    }
    if (s->hasTrack && s->mgr->track != s->soundId) {
        s->stopping = 1;
    }
    if (playing) {
        if (s->stopping) {
            SndMgr *b = s->mgr;
            s->stopping = 1;
            s->vp.follow = NULL;
            player = &b->player;
            if (s->vp.frame != b->frame) {
                alSndpSetSound(player, b->handles[s->index]);
                if (alSndpGetState(player)) {
                    alSndpStop(player);
                }
                s->sound = -1;
            }
            return;
        }
    } else if (s->delay > 0) {
        s->delay--;
        s->flags |= 0x10;
        return;
    }
    if ((s->flags & 0x18) == 0x10) {
        alSndpPlay(&s->mgr->player);
        return;
    }
    if (!playing) {
        SndMgr *b = s->mgr;
        SoundVoice *v = (SoundVoice *)((u8 *)b + 0x7C);
        alSndpDeallocate(&b->player, v->ids[s->index]);
        v->ids[s->index] = -1;
        s->vp.priority = 0;
        s->id = -1;
        s->unk8 = -1;
        s->soundId = -1;
        s->vp.channel = -1;
        s->unkB4 = -1;
        return;
    }
    if (s->vp.follow != NULL) {
        s->vp.pos = *s->vp.follow;
    }
    SndVoice_UpdateVolume(s);
    SndVoice_UpdatePan(s);
    SndVoice_UpdatePitch(s);
}
