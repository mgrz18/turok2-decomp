#include "structs/vector.h"
#include "structs/sound.h"
#include "structs/camera.h"

/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0025A864.c, func_0025A864, with Turok 2's layouts. */

f32 HalfToFloat(u16 x);                /* Trunc16ToF32 */
void alSndpSetSound(void *p, s16 id);     /* alSndpSetSound */
void alSndpSetPan(void *p, s32 pan);    /* alSndpSetPan */
s32 CAudioWorld__GetStereo(CAudioWorld *c);
s32 Sound_CalcPan(f32 *pos, CCamera *t);

void SndVoice_UpdatePan(SndVoice *o) {
    SndVoiceParams *vp;
    s32 neg;
    u8 pan;
    ALSndPlayer *pl;
    CAudioWorld *c;

    vp = &o->vp;
    if (CAudioWorld__GetStereo(o->mgr) == 0) {
        c = o->mgr;
        alSndpSetSound(&c->player, c->handles[o->index]);
        alSndpSetPan(&c->player, 0x40);
        return;
    }
    if (o->hasEnv != 0) {
        if (o->env3.delay > 0) {
            o->env3.delay = o->env3.delay - 1;
        } else {
            neg = 0;
            if (o->env3Step < 0.0f) {
                neg = 1;
            }
            if (neg) {
                if (o->env3Value < HalfToFloat(o->env3.target)) {
                    goto slide;
                }
            } else if (HalfToFloat(o->env3.target) < o->env3Value) {
slide:
                o->env3Value = o->env3Value - o->env3Step;
                if (neg) {
                    if (HalfToFloat(o->env3.target) < o->env3Value) {
                        o->env3Value = HalfToFloat(o->env3.target);
                    }
                } else {
                    if (o->env3Value < HalfToFloat(o->env3.target)) {
                        o->env3Value = HalfToFloat(o->env3.target);
                    }
                }
            }
        }
        vp->pan = (s32)o->env3Value;
        goto setpan;
    }
    if ((o->flags & 0x400) && o->reset == 0) {
        vp->pan = Sound_CalcPan(&o->vp.pos.x, o->mgr->listener);
setpan:
        pan = ((u8 *)&vp->pan)[1];
        pl = &o->mgr->player;
        alSndpSetSound(pl, o->mgr->handles[o->index]);
        alSndpSetPan(pl, pan);
        return;
    }
    vp->pan = 0x40;
}
