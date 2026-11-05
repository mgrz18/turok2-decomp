#include "common.h"
#include "structs/vector.h"
#include "structs/intelligence.h"
#include "structs/level_actor.h"

/* Draws a simple object: rebuilds its matrix for this frame's slot once per
 * frame (from a fixed rotation for type 8, else its yaw), then draws its
 * geometry. Type 12 animates by its speed; the others pick a variant, which
 * models 0x6A9..0x6AA override from the game flags (cf. Rage Wars'
 * func_8024F634_de and func_8024F858_de). */


s32 *func_00215944(CLevelActor *);
void func_00211114(void *, f32 *);
void func_002105A4(f32 *, f32);
void func_0021034C(f32 *, f32, f32, f32);
void func_0020F510(CVector3 *, f32);
void func_00210318(f32 *, f32, f32, f32);
void func_00210C3C(f32 *);
void func_0020D274(f32 *, f32 *);
void func_0020B4F0(s32 *, f32);
void func_0020B6A0(s32 *, f32 *, s32, void *, s32, s32, s32);
void func_002051F4(s32, void *);
s32 func_002241C0(void *, s32);
s32 *func_002017D4(s32, s32);

extern u8 D_800B6D1B;
extern s32 D_800B6D1C;
extern u8 D_8010FF20[];
extern u8 D_800F7078[];
extern s32 D_8011AAD8;

void func_002152AC(CLevelActor *obj) {
    s32 *geometry;
    f32 matrix[16];
    s32 variant;
    s32 index;

    geometry = func_00215944(obj);
    if (geometry == NULL) {
        return;
    }
    if (obj->m_DrawnFrame != D_800B6D1B) {
        if (obj->header.m_pIntelligence->m_Type == 8) {
            func_00211114(D_8010FF20, matrix);
        } else {
            func_002105A4(matrix, obj->m_RotY);
        }
        func_0021034C(matrix, obj->m_ModelScale, obj->m_ModelScale, obj->m_ModelScale);
        func_0020F510(&obj->header.m_vPos, 20000.0f);
        func_00210318(matrix, obj->header.m_vPos.x, obj->header.m_vPos.y + obj->m_Height, obj->header.m_vPos.z);
        func_00210C3C(matrix);
        func_0020D274(matrix, obj->m_Matrices[D_800B6D1C]);
    }
    if (obj->header.m_pIntelligence->m_Type == 12) {
        if (obj->m_DrawnFrame != D_800B6D1B) {
            func_0020B4F0(geometry, obj->m_AnimSpeed);
        }
        func_0020B6A0(geometry, obj->m_Matrices[D_800B6D1C], 0, obj->m_Lights, 0, D_800B6D1C, obj->header.m_pIntelligence->m_nTexture);
    } else {
        variant = obj->header.m_pIntelligence->m_nModel;
        index = func_002241C0(D_800F7078, obj->header.m_nObjType);
        if (index < 0x6AB) {
            if (index >= 0x6A9) {
                if ((D_8011AAD8 & 0x800) && *func_002017D4(*geometry, 2) >= 2) {
                    variant = 1;
                }
                if ((D_8011AAD8 & 0x1000) && *func_002017D4(*geometry, 2) >= 3) {
                    variant = 2;
                }
            }
        }
        func_0020B6A0(geometry, obj->m_Matrices[D_800B6D1C], 0, obj->m_Lights, 0, variant, obj->header.m_pIntelligence->m_nTexture);
    }
    func_002051F4(0, geometry);
    obj->m_DrawnFrame = D_800B6D1B;
}
