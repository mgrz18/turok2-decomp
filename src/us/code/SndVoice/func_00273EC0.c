/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AUDIO/8025A3EC_4.CPP, func_8025BB84_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. Rewritten over
 * structs/sound.h (round 29, part r2: Rage Wars' View_func_8025AE1C_de "priority" 0x28 = vp.pan,
 * envelope 0x8C = env3, sweeping 0x88 = hasEnv, trackId 0xA8 = soundId, fixed 0xC0 = reset; the
 * port's typed copy is port/audio/t2audio_world.c). */
#include "common.h"
#include "structs/sound.h"
#include "structs/camera.h"
#include "protos/half.h"

void SndVoice_InitPitch(SndVoice *v, SoundEffectInfo *info, s32 flags);
void SndVoice_InitVolume(SndVoice *v, SoundEffectInfo *info);
SndEnv *CAudioWorld__GetPanEnvelope(CAudioWorld *m, s32 id);
s32 CAudioWorld__GetStereo(CAudioWorld *m);
s32 Sound_CalcPan(CVector3 *pos, CCamera *cam);

void SndVoice_Setup(SndVoice *v, SoundEffectInfo *info, s32 flags) {
    f32 first;
    s16 index;
    s16 value;

    SndVoice_InitPitch(v, info, flags);
    /* the ROM passes flags too (a2) */
    ((void (*)(SndVoice *, SoundEffectInfo *, s32))SndVoice_InitVolume)(v, info, flags);
    v->vp.pan = 0x40;
    index = info->unkC;
    if (index != -1) {
        v->env3 = *CAudioWorld__GetPanEnvelope(v->mgr, index);
        first = HalfToFloat(v->env3.unk2);
        v->env3Step = (first - HalfToFloat(v->env3.target)) / v->env3.frames;
        v->env3Value = HalfToFloat(v->env3.unk2);
        v->vp.pan = (s16)(s32)HalfToFloat(v->env3.unk2);
        v->hasEnv = 1;
        return;
    }
    v->hasEnv = 0;
    if (v->soundId < 0x100) {
        if (v->reset == 0 && CAudioWorld__GetStereo(v->mgr) != 0) {
            value = (s8)v->mgr->cueVolume;
            goto store_value;
        }
    } else if (v->reset == 0 && CAudioWorld__GetStereo(v->mgr) != 0) {
        value = Sound_CalcPan(&v->vp.pos, v->mgr->listener);
store_value:
        v->vp.pan = value;
    }
}
