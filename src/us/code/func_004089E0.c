#include "common.h"
#include "structs/game_object.h"
#include "structs/model.h"

/* As CGameObjectInstance__DrawModelLitVertices (engine), for a model drawn from its cached "lit vertices"
 * resource: the region sets come from CGameObjectInstance__GetDeathModels and the colour from the
 * module global D_8012FB64. */

extern s32 D_800ACAD8;
extern s32 D_8012FB64;
extern char D_800C0570[];   /* "lit vertices" */

void CGameObjectInstance__GetDeathModels(CGameObjectInstance *, s32, s32 *, s32 *);
void CGameObjectInstance__DrawShadow(CGameObjectInstance *, s32, CDrawArgs *);
void Render_QueueModelInstance(s32, s32, s32, s32, char *, s32, s32);
s32 **CCache__RequestDecompression(s32, s32, s32, s32, s32, CGameObjectInstance *, void *, char *, s32);
void CCache__LockForDisplayList(s32, s32 **);
void Model_PulseVertexColors(char *, char *);
void CCache__Unlock(s32, s32 **);
void CGameObjectInstance__ConstructLitVertices();

void TotemDefense_DrawTotem(CGameObjectInstance *actor, s32 arg1, CDrawArgs *params) {
    s32 **resource;
    s32 *header;
    char *object;
    char *data;

    CGameObjectInstance__GetDeathModels(actor, params->m_nVariant, &params->m_nModel, &params->m_nShadowModel);
    actor->m_AI.m_VisBits = 1 << params->m_nModel;
    if (params->m_bShadow != 0) {
        CGameObjectInstance__DrawShadow(actor, arg1, params);
    }
    if (params->m_bDraw == 0) {
        return;
    }
    resource = CCache__RequestDecompression(0, actor->m_dwUniqueID | D_800ACAD8, params->m_hModel, 0, 0, actor,
                             CGameObjectInstance__ConstructLitVertices, D_800C0570, 1);
    if (resource == NULL) {
        return;
    }
    CCache__LockForDisplayList(0, resource);
    header = *resource;
    if (header[0] == 1) {
        object = (char *) header + header[1];
        Model_PulseVertexColors(object, (char *) header + header[2]);
        data = object;
    } else {
        data = (char *) (header + 2);
    }
    Render_QueueModelInstance(params->m_hModel, ((s32)actor->m_pmtDrawMtxs), 1, 0, data, D_8012FB64, -1);
    CCache__Unlock(0, resource);
}
