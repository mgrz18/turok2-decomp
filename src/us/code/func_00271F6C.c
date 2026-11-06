/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802591A0_de_13.c, func_80259C5C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 Random_Blend(f32 arg0, f32 arg1);
extern f32 HalfToFloat(s32);
extern float D_800A80FC;
extern float D_800A8100;
extern float D_800A8104;
extern float D_800A8108;
extern float D_800A810C;
extern float sPitchRatioTable[];
extern struct Bytes12 *CAudioWorld__GetPitchEnvelope(ContextRecAt2BC0 *, s32);
extern s16 FloatToHalf(f32);
extern s32 SoundEffectInfo_GetRandomRange0(ControllerCode802591C0 *);
extern s32 SoundEffectInfo_GetRandomRange1(ControllerCode802591C0 *);
void SndVoice_InitPitch(ViewRecAtB0 *view, ControllerCode802591C0 *controller, s32 flags) {
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
    struct Motion *motion;
    struct Angles *angles;
    motion = &view->motion;
    angles = &view->angles;
    motion->rate1 = motion->rate2 = 0.0f;
    if (controller->preset != -1) {
        *(struct Bytes12 *)angles = *CAudioWorld__GetPitchEnvelope(view->presets, controller->preset);
    } else {
        angles->heading = FloatToHalf(controller->yaw);
    }
    x = SoundEffectInfo_GetRandomRange0(controller);
    y = SoundEffectInfo_GetRandomRange1(controller);
    fy = y;
    fx = x;
    turn = (s16)(s32)(fx + ((Random_Blend(fy, fx)) < 0.0f ? -(Random_Blend(fy, fx)) : (Random_Blend(fy, fx))));
    angles->heading = FloatToHalf(HalfToFloat(angles->heading) + turn);
    if (flags & 0x40) {
        angles->heading = FloatToHalf(HalfToFloat(angles->heading) + D_800A80FC);
    }
    if (flags & 0x80) {
        angles->heading = FloatToHalf(HalfToFloat(angles->heading) - D_800A8100);
    }
    if (HalfToFloat(angles->heading) >= D_800A8104) {
        angles->heading = FloatToHalf(D_800A8104);
    }
    if (controller->flags & 2) {
        motion->heading = HalfToFloat(angles->heading);
        diff = HalfToFloat(angles->pitch);
        diff -= HalfToFloat(angles->target);
        if (angles->steps == 0) {
            view->pitchStep = 0.0f;
        } else {
            view->pitchStep = diff / angles->steps;
        }
        view->pitch = HalfToFloat(angles->pitch);
    }
    position = (HalfToFloat(angles->heading) + D_800A8108) * D_800A810C;
    index = position;
    if (index != position) {
        next = index + 1;
        base = sPitchRatioTable[index];
        result = base + (position - index) * (sPitchRatioTable[next] - base);
    } else {
        result = sPitchRatioTable[index];
    }
    motion->heading = result;
    angles->heading = FloatToHalf(result);
}
