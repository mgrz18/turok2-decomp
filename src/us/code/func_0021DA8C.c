/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/80246E34_6.CPP, func_8024A7A0_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"

/* As func_0021D7BC, for a model drawn from its cached "lit vertices"
 * resource (built by func_00220208, fixed up once by func_00214F68). */

typedef struct {
    s32 draw;               /* 0x00 */
    s32 shadow;             /* 0x04 */
    u8 pad8[0x4];
    s32 model;              /* 0x0C */
    u8 pad10[0x4];
    s32 index;              /* 0x14 */
    s32 set0;               /* 0x18 */
    s32 set1;               /* 0x1C */
} DrawParams;


extern s32 D_800ACAD8;

void func_0021DC84(CGameObjectInstance *, s32, DrawParams *);
void func_0020B6A0(s32, s32, s32, void *, char *, s32, s32);
s32 **func_00203CA0(s32, s32, s32, s32, s32, CGameObjectInstance *, void *, char *, s32);
void func_0020565C(s32, s32 **);
void func_00214F68(char *, char *);
void func_002051F4(s32, s32 **);
void func_00220208();

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

void func_0021DA8C(CGameObjectInstance *actor, s32 arg1, DrawParams *params) {
    s32 **resource;
    s32 *header;
    char *object;
    char *data;

    pickSets(actor, params->index, &params->set0, &params->set1);
    actor->m_AI.m_VisBits = 1 << params->set0;
    if (params->shadow != 0) {
        func_0021DC84(actor, arg1, params);
    }
    if (params->draw == 0) {
        return;
    }
    resource = func_00203CA0(0, actor->m_dwUniqueID | D_800ACAD8, params->model, 0, 0, actor,
                             func_00220208, "lit vertices", 1);
    if (resource == NULL) {
        return;
    }
    func_0020565C(0, resource);
    header = *resource;
    if (header[0] == 1) {
        object = (char *) header + header[1];
        func_00214F68(object, (char *) header + header[2]);
        data = object;
    } else {
        data = (char *) (header + 2);
    }
    func_0020B6A0(params->model, ((s32)actor->m_pmtDrawMtxs), 1, 0, data, params->set0, actor->m_AI.m_nTexture);
    func_002051F4(0, resource);
}
