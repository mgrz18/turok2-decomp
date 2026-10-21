/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/CAMERA/80233920_2.CPP,
 * func_80233DFC_de, with Turok 2's CCamera. */
#include "common.h"
#include "structs/camera.h"

typedef struct {
    u8 ambient[3], pad3;
    u8 ambientCopy[3], pad7;
    u8 diffuse[3], padB;
    u8 diffuseCopy[3], padF;
    s8 direction[3];
    u8 pad13[5];
} __attribute__((aligned(4))) CLights;

extern CLights D_800AB0D0[];    /* a light of the display lists */
extern u32 D_800B6D20;          /* frame counter */
f32 func_002974C0(f32 x);       /* cosine */
f32 func_002982F0(f32 x);       /* sine */
void func_0020F040(f32 *v);     /* normalise */

/* Cycles a light and the camera's fog colour with the frame counter: the
 * light's colours pulse on sine waves, its direction turns, and the fog
 * drifts between dark and light tones (rounded to 5 bits a channel). */
void func_0027C4E4(CCamera *cam) {
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
            phaseScale * (f32) D_800B6D20 * ((f32) (channel + 2) * 4.0f) *
            0.01745329424738884f) * 16.0f)) + 16.0f + 48.0f;
        D_800AB0D0[0].ambientCopy[channel] = D_800AB0D0[0].ambient[channel] = (u32) ambient;
        diffuse = (((func_002982F0(
            phaseScale * (f32) D_800B6D20 * ((f32) (channel + 2) * 4.0f) *
            0.01745329424738884f) * 64.0f) + 255.0f) - 64.0f) - 1.0f;
        D_800AB0D0[0].diffuseCopy[channel] = D_800AB0D0[0].diffuse[channel] = (u32) diffuse;
        channel += 1;
    } while (channel < 3);
    direction.x = func_002982F0((f32) (D_800B6D20 * 6) * 0.01745329424738884f);
    direction.y = func_002982F0((f32) (D_800B6D20 * 8) * 0.01745329424738884f);
    direction.z = func_002974C0((f32) (D_800B6D20 * 10) * 0.01745329424738884f);
    func_0020F040(&direction.x);
    direction.x *= 120.0f;
    direction.y *= 120.0f;
    direction.z *= 120.0f;
    D_800AB0D0[0].direction[0] = (u32) direction.x;
    D_800AB0D0[0].direction[1] = (u32) direction.y;
    D_800AB0D0[0].direction[2] = (u32) direction.z;
    red = (func_002982F0(2.0f * (phaseScale * (f32) D_800B6D20) * 0.06981317698955536f) * 64.0f) + 128.0f;
    redByte = (u32) red & 0xF8;
    cam->m_FogColor[0] = redByte;
    cam->m_LastFogColor[0] = redByte;
    green = (func_002982F0(phaseScale * (f32) D_800B6D20 * 0.20943953096866608f) * 64.0f) + 128.0f;
    greenByte = (u32) green & 0xF8;
    cam->m_FogColor[1] = greenByte;
    cam->m_LastFogColor[1] = greenByte;
    blue = (func_002982F0(phaseScale * (f32) D_800B6D20 * 0.27925270795822144f) * 64.0f) + 128.0f;
    blueByte = (u32) blue & 0xF8;
    cam->m_FogColor[2] = blueByte;
    cam->m_LastFogColor[2] = blueByte;
}
