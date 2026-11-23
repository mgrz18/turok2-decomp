#include "common.h"
#include "m2c_macros.h"
#include "structs/sound.h"

s16 SoundEffectInfo_GetGroup(SoundEffectInfo *arg0) {
    return arg0->group;
}

s16 SoundEffectInfo_GetSample(SoundEffectInfo *arg0) {
    return arg0->sample;
}

s8 SoundEffectInfo_GetPriority(SoundEffectInfo *arg) {
    return arg->priority;
}

s8 SoundEffectInfo_GetChance(SoundEffectInfo *arg0) {
    return arg0->chance;
}

s16 SoundEffectInfo_GetField08(SoundEffectInfo *arg0) {
    return arg0->unk8;
}

s16 SoundEffectInfo_GetField0A(SoundEffectInfo *arg0) {
    return arg0->unkA;
}

u32 SoundEffectInfo_GetPan(SoundEffectInfo *arg0) {
    return (u16) arg0->flags >> 0xE;
}

s8 SoundEffectInfo_GetPitch(SoundEffectInfo *arg0) {
    return arg0->pitch;
}

s8 SoundEffectInfo_GetField0F(SoundEffectInfo *arg) {
    return arg->unkF;
}

s16 SoundEffectInfo_GetField10(SoundEffectInfo *arg0) {
    return arg0->unk10;
}

s32 SoundEffectInfo_GetFlags(SoundEffectInfo *arg0) {
    return arg0->flags & 0x3FFF;
}

s16 SoundEffectInfo_GetRandomRange0(SoundEffectInfo *arg0) {
    return arg0->randomRange0;
}

s16 SoundEffectInfo_GetRandomRange1(SoundEffectInfo *arg0) {
    return arg0->randomRange1;
}

s16 SoundEffectInfo_GetPanEnvelope(SoundEffectInfo *arg0) {
    return arg0->unkC;
}
