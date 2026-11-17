#include "common.h"

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 a;
    /* 0x08 */ s32 b;
    /* 0x0C */ s32 c;
    /* 0x10 */ s32 d;
} Block00281C08; /* size 0x14 */

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ f32 scale;
    /* 0x18 */ Block00281C08 blocks[3];
} Unk00281C08;

#ifdef TARGET_PORT
#include "structs/camera.h"
/* Round 16 e1: Unk00281C08 is CShake at the N64 offsets (two 4-byte list pointers, then m_vPos at
 * 0x08, m_Radius at 0x14, the three CShakeAxis from 0x18). On the host the pointers are 8 bytes, so the
 * view zeroed the high half of m_pNext and m_vPos.x, put the radius 1.0 into m_vPos.y and left
 * m_Radius 0: CShake__ApplyOffset (distance 0 < radius 1 on the N64) then never drew its three
 * Random_Blend per camera per frame, so the port's RNG took 6 numbers per game frame in the lair where
 * the N64 takes 12 (n64trace.py --calls Random_Blend; PLAYTHROUGH.md 27-e1). The same writes at the
 * host offsets of the members. */
void CShake__Construct(Unk00281C08 *arg0) {
    CShake *shake = (CShake *) arg0;
    CShakeAxis *axis[3];
    s32 i;

    axis[0] = &shake->m_XAxis;
    axis[1] = &shake->m_YAxis;
    axis[2] = &shake->m_ZAxis;
    *(s32 *) &shake->m_vPos.x = 0;
    *(s32 *) &shake->m_vPos.y = 0;
    *(s32 *) &shake->m_vPos.z = 0;
    shake->m_Radius = 1.0f;
    for (i = 0; i < 3; i++) {
        *(s32 *) &axis[i]->m_Shake = 0;
        *(s32 *) &axis[i]->m_LastShake = 0;
        *(s32 *) &axis[i]->m_Amp = 0;
        *(s32 *) &axis[i]->m_Angle = 0;
    }
}
#else
void CShake__Construct(Unk00281C08 *arg0) {
    Block00281C08 *temp_a0;
    Block00281C08 *temp_v0;
    Block00281C08 *temp_v0_2;

    temp_v0 = &arg0->blocks[0];
    arg0->unk8 = 0;
    arg0->unkC = 0;
    arg0->unk10 = 0;
    arg0->scale = 1.0f;
    temp_v0->a = 0;
    temp_v0->b = 0;
    temp_v0->c = 0;
    temp_v0->d = 0;
    temp_v0_2 = &arg0->blocks[1];
    temp_a0 = &arg0->blocks[2];
    temp_v0_2->a = 0;
    temp_v0_2->b = 0;
    temp_v0_2->c = 0;
    temp_v0_2->d = 0;
    temp_a0->a = 0;
    temp_a0->b = 0;
    temp_a0->c = 0;
    temp_a0->d = 0;
}
#endif
