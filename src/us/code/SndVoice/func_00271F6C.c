/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802591A0_de_13.c, func_80259C5C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. Over
 * structs/sound.h (round 29, part r2: Rage Wars' ViewRecAtB0 is the SndVoice, its "angles" the
 * pitch envelope env, its "motion" vp.pitch/vp.distSq; ControllerCode802591C0 is SoundEffectInfo). */

#include "common.h"
#include "structs/sound.h"
#include "globals/audio.h"
#include "protos/half.h"

extern f32 Random_Blend(f32 arg0, f32 arg1);
SndEnv *CAudioWorld__GetPitchEnvelope(CAudioWorld *m, s32 id);
extern s16 SoundEffectInfo_GetRandomRange0(SoundEffectInfo *); /* ABI: returns s16 (the host leaves the upper bits of the return register undefined) */
extern s16 SoundEffectInfo_GetRandomRange1(SoundEffectInfo *); /* ABI: returns s16 (the host leaves the upper bits of the return register undefined) */
void SndVoice_InitPitch(SndVoice *v, SoundEffectInfo *info, s32 flags) {
    s16 x;
    s16 y;
    f32 fx;
    f32 fy;
    s32 turn;
    f32 diff;
    f32 position;
    s16 index;
    s16 next;
    f32 base;
    f32 result;
    SndVoiceParams *vp;
    SndEnv *env;
    vp = &v->vp;
    env = &v->env;
    vp->distSq = vp->prevDistSq = 0.0f;
    if (info->unkA != -1) {
        *env = *CAudioWorld__GetPitchEnvelope(v->mgr, info->unkA);
    } else {
        env->level = FloatToHalf(info->unk10);
    }
    x = SoundEffectInfo_GetRandomRange0(info);
    y = SoundEffectInfo_GetRandomRange1(info);
    fy = y;
    fx = x;
    turn = (s16)(s32)(fx + ((Random_Blend(fy, fx)) < 0.0f ? -(Random_Blend(fy, fx)) : (Random_Blend(fy, fx))));
    env->level = FloatToHalf(HalfToFloat(env->level) + turn);
    if (flags & 0x40) {
        env->level = FloatToHalf(HalfToFloat(env->level) + 1.0e3f);
    }
    if (flags & 0x80) {
        env->level = FloatToHalf(HalfToFloat(env->level) - 1.0e3f);
    }
    if (HalfToFloat(env->level) >= 1.2e3f) {
        env->level = FloatToHalf(1.2e3f);
    }
    if (info->flags & 2) {
        vp->pitch.f = HalfToFloat(env->level);
        diff = HalfToFloat(env->unk2);
        diff -= HalfToFloat(env->target);
        if (env->frames == 0) {
            v->envStep = 0.0f;
        } else {
            v->envStep = diff / env->frames;
        }
        v->envValue = HalfToFloat(env->unk2);
    }
    position = (HalfToFloat(env->level) + 6.0e3f) * 0.016666668f;
    index = position;
    if (index != position) {
        next = index + 1;
        base = sPitchRatioTable[index];
        result = base + (position - index) * (sPitchRatioTable[next] - base);
    } else {
        result = sPitchRatioTable[index];
    }
    vp->pitch.f = result;
    env->level = FloatToHalf(result);
}
