/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/80246E34_6.CPP, func_8024A7A0_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/model.h"
#include "globals/scene.h"
#include "protos/cache.h"

/* As CGameObjectInstance__DrawModel, for a model drawn from its cached "lit vertices"
 * resource (built by CGameObjectInstance__ConstructLitVertices, fixed up once by Model_PulseVertexColors). */


void CGameObjectInstance__DrawShadow(CGameObjectInstance *, sptr, CDrawArgs *);
void Render_QueueModelInstance(s32, s32, s32, void *, char *, s32, s32);
void Model_PulseVertexColors(char *, char *);
void CGameObjectInstance__ConstructLitVertices();

static __inline__ void pickSets(CGameObjectInstance *actor, s32 index, s32 *set0, s32 *set1) {
    CIntelligence *intel = actor->header.m_pIntelligence;

    if (intel->m_Type == 1) {
        s32 state;

        if (actor->m_AI.m_Mode == 0x3C) {
            state = actor->m_AI.m_ModeBefore;
        } else {
            state = actor->m_AI.m_Mode;
        }
        switch (state) {
            case 0x36:
                *set0 = *set1 = intel->m_Enemy.m_DeathModels[0];
                break;
            case 0x37:
                *set0 = *set1 = intel->m_Enemy.m_DeathModels[1];
                break;
            case 0x38:
                *set0 = *set1 = intel->m_Enemy.m_DeathModels[2];
                break;
            case 0x39:
                *set0 = *set1 = intel->m_Enemy.m_DeathModels[3];
                break;
            case 0x3a:
                *set0 = *set1 = intel->m_Enemy.m_DeathModels[4];
                break;
            case 0x3b:
                *set0 = *set1 = intel->m_Enemy.m_DeathModels[5];
                break;
            default:
                *set0 = (&intel->m_nModel)[index];
                *set1 = (&intel->m_nShadowModel)[index];
                break;
        }
    } else {
        *set0 = (&intel->m_nModel)[index];
        *set1 = (&intel->m_nShadowModel)[index];
    }
}

void CGameObjectInstance__DrawModelLitVertices(CGameObjectInstance *actor, s32 arg1, CDrawArgs *params) {
    s32 **resource;
    s32 *header;
    char *object;
    char *data;

    pickSets(actor, params->m_nVariant, &params->m_nModel, &params->m_nShadowModel);
    actor->m_AI.m_VisBits = 1 << params->m_nModel;
    if (params->m_bShadow != 0) {
        CGameObjectInstance__DrawShadow(actor, arg1, params);
    }
    if (params->m_bDraw == 0) {
        return;
    }
    resource = CCache__RequestDecompression(0, actor->m_dwUniqueID | gLitVerticesKey, params->m_hModel, 0, 0, actor,
                             CGameObjectInstance__ConstructLitVertices, "lit vertices", 1);
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
    Render_QueueModelInstance(params->m_hModel, ((sptr)actor->m_pmtDrawMtxs), 1, 0, data, params->m_nModel, actor->m_AI.m_nTexture);
    CCache__Unlock(0, resource);
}
