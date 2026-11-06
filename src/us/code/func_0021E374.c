#include "common.h"
#include "structs/game_object.h"
#include "globals/game.h"

typedef struct Box {
    CVector3 corners[8];            /* 0x00 */
    u8 pad60[0x68 - 0x60];
    f32 unk68;                  /* 0x68 */
    s32 mask;                   /* 0x6C: parts it belongs to */
} Box;                          /* 0x70 bytes */



void **CGameObjectInstance__LoadGeometry(CGameObjectInstance *actor);
void *CISet__GetBlock(void *table, s32 index);
void CVector3__Add(CVector3 *dst, CVector3 *a, CVector3 *b);    /* add */
void CVector3__Scale(CVector3 *dst, CVector3 *src, f32 scale);
void CMtxF__FromMtxAffine(CMtxF *dst, CMtxF *src);
void CMtxF__TransformPoint(CMtxF *m, CVector3 *in, CVector3 *out);
void CVector3__Subtract(CVector3 *dst, CVector3 *a, CVector3 *b);    /* subtract */
void CCache__Unlock(s32 arg0, void **handle);

/* Copies the actor's bounding box whose center is nearest to pos into out. */
s32 CGameObjectInstance__GetNearestBox(CGameObjectInstance *actor, CVector3 pos, Box *out) {
    CVector3 center;
    CVector3 d;
    CMtxF mtx;
    void **handle;
    void *boxes;
    CMtxF *matrices;
    Box *box;
    Box *best;
    s32 count;
    s32 i;
    s32 j;
    f32 dist;
    f32 bestDist;

    matrices = ((CMtxF *)actor->m_pmtDrawMtxs);
    if (matrices == NULL) {
        matrices = ((CMtxF *)actor->m_pmtLastFrameDrawMtxs);
        if (matrices == NULL) {
            return 0;
        }
    }
    handle = CGameObjectInstance__LoadGeometry(actor);
    if (handle == NULL) {
        return 0;
    }
    boxes = CISet__GetBlock(*handle, 3);
    best = NULL;
    count = *(s32 *)boxes;
    bestDist = 3.4028235e38f;
    for (i = 0; i < count; i++) {
        box = CISet__GetBlock(CISet__GetBlock(boxes, i), 1);
        if (!(box->mask & actor->m_AI.m_VisBits)) {
            continue;
        }
        center = box->corners[0];
        for (j = 1; j < 8; j++) {
            CVector3__Add(&center, &center, &box->corners[j]);
        }
        CVector3__Scale(&center, &center, 0.125f);
        CMtxF__FromMtxAffine(&mtx, &matrices[i]);
        CMtxF__TransformPoint(&mtx, &center, &d);
        CVector3__Subtract(&d, &d, &pos);
        dist = d.x * d.x + d.y * d.y + d.z * d.z;
        if (dist < bestDist) {
            best = box;
            bestDist = dist;
        }
    }
    if (best != NULL) {
        *out = *best;
        if ((actor->m_dwFlags & 0x300000) && gEngineApp_m_Settings.multiplayer == 0) {
            out->unk68 = 1.0f;
        }
    }
    CCache__Unlock(0, handle);
    return best != NULL;
}
