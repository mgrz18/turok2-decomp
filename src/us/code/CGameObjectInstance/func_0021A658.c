#include "common.h"
#include "structs/game_object.h"
#include "structs/region.h"
#include "structs/vector.h"
#include "globals/collision.h"
#include "protos/game.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/80246E34.CPP, func_8024715C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

union func_80203908_S3_U124;
typedef union func_80203908_S3_U124 func_80203908_S3_U124;
union func_80203908_S3_U124;
union func_80203908_S3_U124 {
    f32 v0;
    s32 v1;
};
struct CollisionInfo;
struct CollisionInfo {
    s32 w[8];
};
struct ObjectLinks23C;
struct ObjectLinks23C {
    char pad0[0x4];
    CVector3 unk_8;     /* 0x04 (0x08 in Rage Wars) */
    u16 *unk_14;        /* 0x10 (0x14) */
    s32 *unk_18;        /* 0x14 (0x18) */
    char pad18[0xD4 - 0x18];
    s32 unk_100;        /* 0xD4 (0x100) */
    char padD8[0x170 - 0xD8];
    void *unk_1A0;      /* 0x170 (0x1A0) */
    char pad174[0x1C4 - 0x174];
    u16 *unk_1F4;       /* 0x1C4 (0x1F4) */
    char pad1C8[0x204 - 0x1C8];
    u16 unk_238;        /* 0x204 (0x238) */
};
struct CallbackState288 {
    unsigned char padding_0[0x23C];
    void (*callback)(void *, void *, struct CollisionInfo *); /* 0x23C (0x284) */
};
typedef struct ObjectStateC ObjectStateC;
struct ObjectLinks10;
typedef struct ObjectLinks10 ObjectLinks10;
struct CallbackState288;
typedef struct CallbackState288 CallbackState288;
struct CollisionInfo;
struct ObjectLinks10;
struct ObjectLinks10 {
    char pad0[0xC];
    struct CollisionInfo * unk_C;
};
struct ObjectLinks23C;
typedef struct ObjectLinks23C ObjectLinks23C;
struct ObjectStateC;
struct ObjectStateC {
    char pad0[0x8];
    func_80203908_S3_U124 unk_8;
};
extern void CGameObjectInstance__MoveWithCollision(char *, CVector3 *);
typedef struct CollisionInfo CollisionInfo;
extern CollisionInfo D_800F1EB0;
extern s32 Collision_MoveInstance(void *arg0, CVector3 arg1, CollisionInfo *arg2);
extern void CGameRegion__FireTriggers(sptr arg0, s32 arg1, void *arg2);
#ifdef TARGET_PORT
/* The host body on CGameObjectInstance: ObjectLinks23C/CallbackState288 are N64 offsets
 * (round 6, u3). */
void CGameObjectInstance__MoveWithCollision(char *arg0, CVector3 *arg1) {
    CGameObjectInstance *o = (CGameObjectInstance *) arg0;
    CollisionInfo *collision;
    CGameRegion *old_contact;
    s32 collided;
    f32 saved;
    void (*callback)(void *, void *, CollisionInfo *);

    if ((o->m_dwFlags & 0x10000) && o->m_AI.m_pModeCurrent != NULL) {
        collision = (CollisionInfo *) o->m_AI.m_pModeCurrent->m_pData;
        if (collision == NULL) {
            collision = &D_800F1EB0;
        }
        saved = ((ObjectStateC *) collision)->unk_8.v0;
        if ((u32) ((u16) o->m_AI.m_AnimType - 0x2B0C) < 8) {
            ((ObjectStateC *) collision)->unk_8.v1 = 0;
        }
        old_contact = o->header.m_pCurrentRegion;
        collided = Collision_MoveInstance(o, *arg1, collision);
        if (old_contact != o->header.m_pCurrentRegion) {
            o->m_AI.m_pPathAvoidRegion = old_contact;
        }
        ((ObjectStateC *) collision)->unk_8.v0 = saved;
        if (old_contact != NULL && o->header.m_pCurrentRegion != NULL
            && old_contact->m_nInfo != o->header.m_pCurrentRegion->m_nInfo && o->header.m_pIntelligence->m_Type == 1) {
            CGameRegion__FireTriggers((sptr) old_contact, 0x80, o);
            CGameRegion__FireTriggers((sptr) o->header.m_pCurrentRegion, 0x40, o);
        }
        if (collided != 0 && !(o->m_dwFlags & 0x300000) && o->header.m_pIntelligence->m_Type == 1
            && gCollisionResults.m_pInstance != NULL && *(u8 *) gCollisionResults.m_pInstance == 1) {
            GameObject_FireTriggers(gCollisionResults.m_pInstance, 0x20, arg0);
        }
        callback = (void (*)(void *, void *, CollisionInfo *)) o->m_AI.m_pfCollision;
        if (callback != NULL) {
            callback(o, &o->m_AI, collision);
        }
    } else {
        o->header.m_vPos = *arg1;
    }
}
#else
void CGameObjectInstance__MoveWithCollision(char *arg0, CVector3 *arg1) {
    CollisionInfo *collision;
    u16 *old_contact;
    s32 collided;
    f32 saved;
    void (*callback)(void *, void *, CollisionInfo *);
    if ((((ObjectLinks23C *)(arg0))->unk_100 & 0x10000) && ((ObjectLinks23C *)(arg0))->unk_1A0 != 0) {
        collision = ((ObjectLinks10 *)(((ObjectLinks23C *)(arg0))->unk_1A0))->unk_C;
        if (collision == 0) {
            collision = &D_800F1EB0;
        }
        saved = ((ObjectStateC *)(collision))->unk_8.v0;
        if ((u32)(((ObjectLinks23C *)(arg0))->unk_238 - 0x2B0C) < 8) {
            ((ObjectStateC *)(collision))->unk_8.v1 = 0;
        }
        old_contact = ((ObjectLinks23C *)(arg0))->unk_14;
        collided = Collision_MoveInstance(arg0, *arg1, collision);
        if (old_contact != ((ObjectLinks23C *)(arg0))->unk_14) {
            ((ObjectLinks23C *)(arg0))->unk_1F4 = old_contact;
        }
        ((ObjectStateC *)(collision))->unk_8.v0 = saved;
        if (old_contact != 0 && ((ObjectLinks23C *)(arg0))->unk_14 != 0 &&
                *old_contact != *((ObjectLinks23C *)(arg0))->unk_14 &&
                *((ObjectLinks23C *)(arg0))->unk_18 == 1) {
            CGameRegion__FireTriggers((sptr)old_contact, 0x80, arg0);
            CGameRegion__FireTriggers((sptr)((ObjectLinks23C *)(arg0))->unk_14, 0x40, arg0);
        }
        if (collided != 0 && !(((ObjectLinks23C *)(arg0))->unk_100 & 0x300000) &&
                *((ObjectLinks23C *)(arg0))->unk_18 == 1 && gCollisionResults.m_pInstance != NULL &&
                *(u8 *)gCollisionResults.m_pInstance == 1) {
            GameObject_FireTriggers(gCollisionResults.m_pInstance, 0x20, arg0);
        }
        callback = ((struct CallbackState288 *) ((char *) arg0))->callback;
        if (callback != 0) {
            callback(arg0, arg0 + 0x140, collision);
        }
    } else {
        ((ObjectLinks23C *)(arg0))->unk_8 = *arg1;
    }
}
#endif
