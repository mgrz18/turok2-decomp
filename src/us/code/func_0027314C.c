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

extern s32 func_00281444(void *game, CVector3 *pos);
extern void func_00293E60(void *player, s32 id);    /* alSndpSetSound */
extern s32 func_00293580(void *player);             /* alSndpGetState */
extern void func_00293EC0(void *player);            /* alSndpStop */
extern void func_00293CE0(void *player);
extern void func_00293530(void *player, s32 id);    /* alSndpDeallocate */
extern void func_002726DC(SndVoice *s);
extern void func_00272B54(SndVoice *s);
extern void func_00272EC4(SndVoice *s);

void func_0027314C(SndVoice *s) {
    SndMgr *bank;
    s32 playing;
    void *player;

    s->mgr->listener = func_00281444(D_801198B0, &s->vp.pos);
    bank = s->mgr;
    func_00293E60(bank->player, bank->handles[s->index]);
    playing = func_00293580(bank->player);
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
            player = b->player;
            if (s->vp.frame != b->frame) {
                func_00293E60(player, b->handles[s->index]);
                if (func_00293580(player)) {
                    func_00293EC0(player);
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
        func_00293CE0(s->mgr->player);
        return;
    }
    if (!playing) {
        SndMgr *b = s->mgr;
        SoundVoice *v = (SoundVoice *)((u8 *)b + 0x7C);
        func_00293530(b->player, v->ids[s->index]);
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
    func_002726DC(s);
    func_00272B54(s);
    func_00272EC4(s);
}
