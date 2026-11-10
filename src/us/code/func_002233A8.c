#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028A258.c, func_0028A258.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/intelligence.h"
#include "structs/world.h"
#include "structs/game_object.h"
#include "protos/ai.h"

#ifdef TARGET_PORT
#define T2_AI(e) ((u8 *) &(e)->m_AI) /* the AI by member (N64 0x140) */
#else
#define T2_AI(e) ((u8 *)(e) + 0x140)
#endif
f32 func_0026841C(CGameObjectInstance *e);
f32 GameObject_GetCollisionHeight(CGameObjectInstance *e);
s32 Action_CheckRequiredItem(CGameObjectInstance *e, u8 *p, CGameObjectInstance *me);
void CScene__CheckAutoActivation(CScene *lv, CGameObjectInstance *me, CVector3 *p0, CVector3 *p1) {
    s32 isB;
    s32 is1;
    f32 myBot;
    f32 myTop;
    s32 i;
    CGameObjectInstance *e;
    f32 eBot;
    f32 eTop;
    s32 kind;
    s32 s1;
    s32 s2;
    s32 s3;
    s32 s4;
    s32 a0;
    s32 v1;
    s32 a2;
    s32 a1;
    s32 hit;
    s32 hit2;
    f32 r1;
    register f32 r2 asm("$f20");
    f32 dx;
    f32 dz;
    f32 d0;
    f32 d1;
    f32 ex;
    f32 ez;
    s32 ok;
    u32 fl;
    if (p0->x != p1->x || p0->y != p1->y || p0->z != p1->z) {
        kind = me->header.m_pIntelligence->m_Type;
        isB = kind == 0xB;
        is1 = kind == 1;
        myBot = me->header.m_vPos.y + func_0026841C(me);
        myTop = myBot + GameObject_GetCollisionHeight(me);
        for (i = 0; i < lv->m_nFlagged; i++) {
            e = (CGameObjectInstance *)lv->m_pFlagged[i];
            if (e != me) {
                s4 = 0;
                s1 = 0;
                eBot = e->header.m_vPos.y + func_0026841C(e);
                eTop = eBot + GameObject_GetCollisionHeight(e);
                switch (e->header.m_pIntelligence->m_Type) {
                case 6:
                    s2 = 0;
                    s3 = s4 = 1;
                    fl = e->header.m_pIntelligence->m_dwTypeFlags;
                    r1 = e->header.m_pIntelligence->m_Door.m_AutoOpenRadius;
                    r2 = e->header.m_pIntelligence->m_Door.m_AutoCloseRadius;
                    a0 = fl & 4;
                    v1 = fl & 8;
                    a2 = fl & 0x10;
                    a1 = fl & 0x20;
                    break;
                case 10:
                    s2 = 1;
                    s3 = 0;
                    a2 = s3;
                    a1 = s3;
                    s1 = s2;
                    fl = e->header.m_pIntelligence->m_dwTypeFlags;
                    r1 = e->header.m_pIntelligence->m_Action.m_AutoGoRadius;
                    r2 = 0.0f;
                    a0 = fl & 4;
                    v1 = fl & 8;
                    break;
                case 7:
                    s2 = 1;
                    s3 = 0;
                    a2 = s3;
                    a1 = s3;
                    s1 = s2;
                    fl = e->header.m_pIntelligence->m_dwTypeFlags;
                    r1 = e->header.m_pIntelligence->m_Destructible.m_AutoGoRadius;
                    r2 = 0.0f;
                    a0 = fl & 8;
                    v1 = fl & 0x10;
                    break;
                default:
                    s2 = 0;
                    r1 = 0.0f;
                    a0 = 0;
                    v1 = 0;
                    s3 = 0;
                    a2 = 0;
                    a1 = 0;
                    r2 = 0.0f;
                    break;
                }
                if (s1 != 0 && (myTop < eBot || eTop < myBot)) {
                    hit2 = hit = 0;
                } else {
                    hit = 0;
                    if (isB != 0 && a0 != 0) {
                        hit = 1;
                    } else if (is1 != 0 && v1 != 0) {
                        hit = 1;
                    }
                    hit2 = 0;
                    if ((isB != 0 && a2 != 0) || (is1 != 0 && a1 != 0)) {
                        hit2 = 1;
                    }
                }
                if (hit != 0 || hit2 != 0) {
                    d0 = (p0->x - e->header.m_vPos.x) * (p0->x - e->header.m_vPos.x) + (p0->z - e->header.m_vPos.z) * (p0->z - e->header.m_vPos.z);
                    d1 = (p1->x - e->header.m_vPos.x) * (p1->x - e->header.m_vPos.x) + (p1->z - e->header.m_vPos.z) * (p1->z - e->header.m_vPos.z);
                    if (hit != 0) {
                        if (s4 != 0) {
                            ok = 0;
                            if (r1 * r1 < d0) {
                                if (d1 <= r1 * r1) {
                                    ok = 1;
                                }
                            }
                        } else {
                            ok = 1;
                            if (!(d1 <= r1 * r1)) {
                                ok = 0;
                            }
                        }
                        if (ok != 0) {
                            if (e->header.m_pIntelligence->m_Type == 10 && s2 == 1) {
                                ok = Action_CheckRequiredItem(e, T2_AI(e), me);
                            }
                            if (ok != 0) {
                                CAI__SetMode(e, T2_AI(e), s2);
                            }
                        }
                    }
                    if (hit2 != 0) {
                        if (s4 != 0) {
                            ok = 0;
                            if (d0 <= r2 * r2) {
                                if (r2 * r2 < d1) {
                                    ok = 1;
                                }
                            }
                        } else {
                            ok = 1;
                            if (!(r2 * r2 < d1)) {
                                ok = 0;
                            }
                        }
                        if (ok != 0) {
                            CAI__SetMode(e, T2_AI(e), s3);
                        }
                    }
                }
                __asm__ volatile("" : : "f"(r2));
            }
        }
    }
}
