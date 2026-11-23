#include "common.h"
#include "globals/audio.h"

s32 CAudioWorld__Initialize(s32 *);                   /* extern */

#ifndef TARGET_PORT /* the port defines Sound__Initialize in port/runtime/t2boot.c */
void Sound__Initialize(void) {
    CAudioWorld__Initialize(&gAudioWorld);
}
#endif /* !TARGET_PORT */

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025D928_de_8.c, func_8025DE54_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/game.h"

extern s32 CAudioWorld__PlaySound(void *, s32, CVector3, sptr, s32);
#ifdef TARGET_PORT
int T2Port_AudioReady(void);
#endif

s32 Sound__Play(s16 arg0, CVector3 arg1, sptr arg4, sptr arg5) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return 0;
    }
#endif
    if (gEngineApp_m_Settings.unk1C8 != 0) {
        return -1;
    }
    return CAudioWorld__PlaySound(&gAudioWorld, arg0, arg1, arg4, arg5);
}

extern s32 CAudioWorld__PlaySoundPitched(struct ContextRecAt2BC0 *arg0, s32 arg1, CVector3 arg2,
                         sptr arg3, s32 arg4, f32 arg5);

s32 Sound__PlayPitched(s32 arg0, CVector3 arg1, s32 arg4, sptr arg5, f32 arg6) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return 0;
    }
#endif
    if (gEngineApp_m_Settings.unk1C8 != 0) {
        return -1;
    }
    return CAudioWorld__PlaySoundPitched(&gAudioWorld, (s16)arg0, arg1, arg4, arg5, arg6);
}

extern s32 CAudioWorld__GetForce2D(CAudioWorld *arg0);
extern void CAudioWorld__SetForce2D(CAudioWorld *arg0, s32 arg1);

s32 Sound__Play2D(s32 arg0) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return 0;
    }
#endif
    s32 temp_s0;
    s32 temp_s1;
    if (gEngineApp_m_Settings.unk1C8 != 0) {
        return -1;
    }
    {
        CVector3 vec;
        register f32 zero = 0.0f;
        vec.z = zero;
        vec.y = zero;
        vec.x = zero;
        temp_s1 = CAudioWorld__GetForce2D(&gAudioWorld);
        CAudioWorld__SetForce2D(&gAudioWorld, 1);
        temp_s0 = CAudioWorld__PlaySound(&gAudioWorld, (s16)arg0, vec, 0, -1);
        CAudioWorld__SetForce2D(&gAudioWorld, temp_s1);
    }
    return temp_s0;
}

extern s32 CAudioWorld__PlaySoundIndex(void *, s32, CVector3, s32);
extern int CAudioWorld__PlaySoundIndex(void *arg0, int arg1, CVector3 arg2, int arg3);

s32 Sound__PlayIndex(s32 arg0, CVector3 arg1, s32 arg2) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return 0;
    }
#endif
    if (gEngineApp_m_Settings.unk1C8 != 0) {
        return -1;
    }
    return CAudioWorld__PlaySoundIndex(&gAudioWorld, arg0, arg1, arg2);
}
int Sound__PlayIndex2D(int arg0);

int Sound__PlayIndex2D(int arg0) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return 0;
    }
#endif
    int temp_s0;
    int temp_s1;
    if (gEngineApp_m_Settings.unk1C8 != 0) {
        return -1;
    }
    {
        CVector3 vec;
        register float zero = (float)0;
        vec.z = zero;
        vec.y = zero;
        vec.x = zero;
        temp_s1 = CAudioWorld__GetForce2D(&gAudioWorld);
        CAudioWorld__SetForce2D(&gAudioWorld, 1);
        temp_s0 = CAudioWorld__PlaySoundIndex(&gAudioWorld, arg0, vec, 0);
        CAudioWorld__SetForce2D(&gAudioWorld, temp_s1);
    }
    return temp_s0;
}

s32 CAudioWorld__CountHandleVoices(s32 *, s32);              /* extern */

#ifndef TARGET_PORT /* the port defines Sound__IsPlaying in port/audio/t2audio_world.c */
void Sound__IsPlaying(s32 arg0) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    CAudioWorld__CountHandleVoices(&gAudioWorld, arg0);
}
#endif /* !TARGET_PORT */

s32 CAudioWorld__PlaySpeech(s32 *, s32);              /* extern */

void Sound__PlaySpeech(s32 arg0) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    if (gEngineApp_m_Settings.unk1C8 == 0) {
        CAudioWorld__PlaySpeech(&gAudioWorld, arg0);
    }
}

s32 CAudioWorld__StopSpeech(s32 *);                   /* extern */

void Sound__StopSpeech(void) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    CAudioWorld__StopSpeech(&gAudioWorld);
}

s32 CAudioWorld__StopHandle(s32 *, s32);              /* extern */

void Sound__Stop(s32 arg0) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    CAudioWorld__StopHandle(&gAudioWorld, arg0);
}

void CAudioWorld__StopSound(void *, s32);              /* extern */

void Sound__StopSoundId(s32 arg0) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    CAudioWorld__StopSound(&gAudioWorld, arg0);
}

s32 CAudioWorld__StopSoundsByOwner(s32 *, s32);              /* extern */

void Sound__StopOwner(sptr arg0) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    CAudioWorld__StopSoundsByOwner(&gAudioWorld, arg0);
}

s32 CAudioWorld__StopSoundsByFlags(s32 *, s32);              /* extern */

void Sound__StopFlags(s32 arg0) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    CAudioWorld__StopSoundsByFlags(&gAudioWorld, arg0);
}

s32 CAudioWorld__StopAllSounds(s32 *, s32);              /* extern */

void Sound__StopAllExcept(s32 arg0) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    CAudioWorld__StopAllSounds(&gAudioWorld, arg0);
}

s32 CAudioWorld__Update(s32 *);                   /* extern */

void Sound__Update(void) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    CAudioWorld__Update(&gAudioWorld);
}
void Sound__DisableAmbientTrack(void);
void Sound__EnableAmbientTrack(void);

void Sound__SetAmbientTrack(s32 arg0) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    gAudioWorld.track = arg0;
    if (arg0 != 0) {
        Sound__EnableAmbientTrack();
        return;
    }
    Sound__DisableAmbientTrack();
}

void CAudioWorld__SetForce2D(CAudioWorld *, s32);             /* extern */

void Sound__SetForce2D(s32 arg0) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    CAudioWorld__SetForce2D(&gAudioWorld, arg0);
}

#include "m2c_macros.h"

s32 Music_GetTrack(void) {
    return gAudioWorld.seq.track;
}
void Music_FadeOut(void);

void Music_SetTrack(s32 arg0) {
    if ((gEngineApp_m_Settings_unk1C8[0] == 0) && (gAudioWorld.seq.track != arg0)) {
        gAudioWorld.seq.track = arg0;
        if (arg0 == 0) {
            Music_FadeOut();
        }
    }
}

void CAudioWorld__FadeOutMusic(CAudioWorld *);              /* extern */

void Music_FadeOut(void) {
    CAudioWorld__FadeOutMusic(&gAudioWorld);
}

s32 CAudioWorld__StopMusic(s32 *);                   /* extern */

void Music_Stop(void) {
    CAudioWorld__StopMusic(&gAudioWorld);
}

s32 CAudioWorld__DuckMusic(s32 *);                   /* extern */

void Music_DuckVolume(void) {
    CAudioWorld__DuckMusic(&gAudioWorld);
}

void CAudioWorld__UnduckMusic(void *);                   /* extern */

void Music_RestoreVolume(void) {
    CAudioWorld__UnduckMusic(&gAudioWorld);
}

s32 CAudioWorld__SetAmbientTrackEnabled(s32 *, s32);          /* extern */

void Sound__DisableAmbientTrack(void) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    CAudioWorld__SetAmbientTrackEnabled(&gAudioWorld, 0);
}

void Sound__EnableAmbientTrack(void) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    CAudioWorld__SetAmbientTrackEnabled(&gAudioWorld, 1);
}

s32 CAudioWorld__SetListener(s32 *, sptr);              /* extern */

void Sound__SetListener(sptr arg0) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    CAudioWorld__SetListener(&gAudioWorld, arg0);
}

s32 CAudioWorld__SetMuted(s32 *, s32);              /* extern */

void Sound__SetMuted(s32 arg0) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    CAudioWorld__SetMuted(&gAudioWorld, arg0);
}

s32 CAudioWorld__SetFxMix(s32 *, f32);              /* extern */

void Sound__SetReverb(f32 arg0) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    CAudioWorld__SetFxMix(&gAudioWorld, arg0);
}

s32 CAudioWorld__DetachAllVoices(s32 *);                   /* extern */

void Sound__DetachAll(void) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    CAudioWorld__DetachAllVoices(&gAudioWorld);
}

s32 CAudioWorld__Lock(s32 *);                   /* extern */

void Sound__Lock(void) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    CAudioWorld__Lock(&gAudioWorld);
}

extern void CAudioWorld__Unlock(void *);

void Sound__Unlock(void) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return;
    }
#endif
    CAudioWorld__Unlock(&gAudioWorld);
}
