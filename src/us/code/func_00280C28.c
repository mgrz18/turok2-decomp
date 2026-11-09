/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00238EA8.c, func_00238EA8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#ifdef TARGET_PORT
/* The port: the same stores through CCameraViewParams (structs/camera.h); the 4-byte slot
 * ObjInit has at 0x34 is the m_pCurrentRegion pointer, 8 bytes on the host. */
#include "common.h"
#include "structs/camera.h"

extern u32 gDefaultVisBits[];
void CCameraViewParams__Construct(CCameraViewParams *o) {
    o->m_YPosOffset = 0.0f;
    o->m_nType = 1;
    o->m_RotY = 0.0f;
    o->m_vRotOffset.x = o->m_vRotOffset.y = o->m_vRotOffset.z = 0.0f;
    o->m_vPos.x = o->m_vPos.y = o->m_vPos.z = 0.0f;
    o->m_qGround.x = o->m_qGround.y = o->m_qGround.z = 0.0f;
    o->m_qGround.w = 1.0f;
    o->m_pCurrentRegion = NULL;
    o->m_ZoomBlend = 0.0f;
    o->m_ZoomFov = 0.17453295f;
    o->m_VisBits = gDefaultVisBits[0];
}
#else
#include "rw/junk_types.h"

extern s32 gDefaultVisBits[]; /* local type: the u32 of globals/camera.h loads it first (GCC 2 alias order) */
void CCameraViewParams__Construct(ObjInit *o) {
    CVector3 *p;
    f32 z;
    o->unk20.i = 0;
    z = o->unk20.f;
    o->unk0 = 1;
    o->unk4 = 0;
    o->unk8 = 0;
    o->unkC = 0;
    o->unk10 = 0;
    o->unk14 = 0;
    o->unk18 = 0;
    o->unk1C = 0;
    p = &o->v;
    p->x = p->y = p->z = z;
    o->unk30 = 1.0f;
    o->unk34 = 0;
    o->unk38 = z;
    o->unk3C = 0.17453295f;
    o->unk40 = gDefaultVisBits[0];
}
#endif
