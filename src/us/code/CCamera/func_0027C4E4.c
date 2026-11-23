/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/CAMERA/80233920_2.CPP,
 * func_80233DFC_de, with Turok 2's CCamera. */
#include "common.h"
/* float -> u32 as the R4300 does it (cvt.w.s: a negative value wraps). arm64's fcvtzu saturates
 * negatives to 0 (MACOS.md section 9, tools/port/f2ucheck.py); x86-64 happens to wrap. */
#ifdef TARGET_PORT
#define T2_F2U(x) ((u32)(s32)(x))
#else
#define T2_F2U(x) ((u32)(x))
#endif
#include "structs/camera.h"
#include "structs/light.h"
#include "globals/engine.h"

f32 func_002974C0(f32 x);       /* cosine */
f32 func_002982F0(f32 x);       /* sine */
void CVector3__Normalize(f32 *v);     /* normalise */

/* Cycles a light and the camera's fog colour with the frame counter: the
 * light's colours pulse on sine waves, its direction turns, and the fog
 * drifts between dark and light tones (rounded to 5 bits a channel). */
void CCamera__CycleLightAndFog(CCamera *cam) {
    CVector3 direction;
    f32 phaseScale;
    f32 ambientScale;
    f32 ambient;
    f32 diffuse;
    f32 red;
    f32 green;
    f32 blue;
    s32 channel;
    s8 redByte;
    s8 greenByte;
    s8 blueByte;

    phaseScale = 1.0f;
    channel = 0;
    ambientScale = phaseScale;
    do {
        ambient = (ambientScale * (func_002982F0(
            phaseScale * (f32) gFrameCountGameplay * ((f32) (channel + 2) * 4.0f) *
            0.01745329424738884f) * 16.0f)) + 16.0f + 48.0f;
        gSceneLights.m_AmbColCopy[channel] = gSceneLights.m_AmbCol[channel] = (u32) ambient;
        diffuse = (((func_002982F0(
            phaseScale * (f32) gFrameCountGameplay * ((f32) (channel + 2) * 4.0f) *
            0.01745329424738884f) * 64.0f) + 255.0f) - 64.0f) - 1.0f;
        gSceneLights.m_ColCopy[channel] = gSceneLights.m_Col[channel] = (u32) diffuse;
        channel += 1;
    } while (channel < 3);
    direction.x = func_002982F0((f32) (gFrameCountGameplay * 6) * 0.01745329424738884f);
    direction.y = func_002982F0((f32) (gFrameCountGameplay * 8) * 0.01745329424738884f);
    direction.z = func_002974C0((f32) (gFrameCountGameplay * 10) * 0.01745329424738884f);
    CVector3__Normalize(&direction.x);
    direction.x *= 120.0f;
    direction.y *= 120.0f;
    direction.z *= 120.0f;
    gSceneLights.m_Dir[0] = T2_F2U(direction.x);
    gSceneLights.m_Dir[1] = T2_F2U(direction.y);
    gSceneLights.m_Dir[2] = T2_F2U(direction.z);
    red = (func_002982F0(2.0f * (phaseScale * (f32) gFrameCountGameplay) * 0.06981317698955536f) * 64.0f) + 128.0f;
    redByte = (u32) red & 0xF8;
    cam->m_FogColor[0] = redByte;
    cam->m_LastFogColor[0] = redByte;
    green = (func_002982F0(phaseScale * (f32) gFrameCountGameplay * 0.20943953096866608f) * 64.0f) + 128.0f;
    greenByte = (u32) green & 0xF8;
    cam->m_FogColor[1] = greenByte;
    cam->m_LastFogColor[1] = greenByte;
    blue = (func_002982F0(phaseScale * (f32) gFrameCountGameplay * 0.27925270795822144f) * 64.0f) + 128.0f;
    blueByte = (u32) blue & 0xF8;
    cam->m_FogColor[2] = blueByte;
    cam->m_LastFogColor[2] = blueByte;
}
