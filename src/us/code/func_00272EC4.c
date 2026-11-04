/* A looping sound's pitch: slide its offset (in cents) toward the target and set the pitch. */

#include "common.h"
#include "structs/sound.h"

extern f32 D_800B2848[];                    /* pitch ratio every 60 cents from -6000 */
#define PITCH_TABLE (D_800B2848 + 1)

extern void func_00272D3C(SndVoice *s);
extern f32 func_0026D210(s32 halfFloat);
extern void func_00293E60(void *player, s32 id);    /* alSndpSetSound */
extern void func_00293DE0(void *player, f32 pitch); /* alSndpSetPitch */

static __inline__ f32 centsToPitch(f32 x) {
    f32 pos = (x + 6000.0f) * 0.016666668f;
    s16 i = pos;
    f32 fi = i;
    s16 j;
    f32 a;

    if (fi != pos) {
        j = i + 1;
        a = PITCH_TABLE[i];
        return a + (pos - fi) * (PITCH_TABLE[j] - a);
    }
    return PITCH_TABLE[i];
}

static __inline__ void setPitch(SndVoice *s, f32 pitch) {
    SndMgr *bank = s->mgr;

    func_00293E60(bank->player, bank->handles[s->index]);
    func_00293DE0(bank->player, pitch * s->vp.doppler * s->pitchScale);
}

void func_00272EC4(SndVoice *s) {
    SndVoiceParams *params;
    SndEnv *target;
    SndMgr *bank;
    s32 update;
    s32 down;
    f32 pitch;

    update = 0;
    params = &s->vp;
    target = &s->env;
    bank = s->mgr;
    if ((s->flags & 0x400) && !(bank->frame & 3)) {
        func_00272D3C(s);
        update = 1;
    }
    if ((s->flags & 0xA) == 0xA) {
        if (s->env.delay > 0) {
            s->env.delay--;
        } else {
            down = 0;
            if (s->envStep < 0.0f) {
                down = 1;
            }
            if (down ? s->envValue < func_0026D210(target->target) : func_0026D210(target->target) < s->envValue) {
                s->envValue -= s->envStep;
                if (down) {
                    if (func_0026D210(target->target) < s->envValue) {
                        s->envValue = func_0026D210(target->target);
                    }
                } else {
                    if (s->envValue < func_0026D210(target->target)) {
                        s->envValue = func_0026D210(target->target);
                    }
                }
                pitch = centsToPitch(s->envValue);
                params->pitch.f = pitch;
                setPitch(s, pitch);
                return;
            }
        }
    }
    if (update) {
        setPitch(s, params->pitch.f);
    }
}
