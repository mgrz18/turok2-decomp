#include "common.h"
#include "structs/game_object.h"
#include "structs/model.h"

/* As func_0021DA8C (engine), for a model drawn from its cached "lit vertices"
 * resource: the region sets come from func_0021EBE4 and the colour from the
 * module global D_8012FB64. */

extern s32 D_800ACAD8;
extern s32 D_8012FB64;
extern char D_800C0570[];   /* "lit vertices" */

void func_0021EBE4(CGameObjectInstance *, s32, s32 *, s32 *);
void func_0021DC84(CGameObjectInstance *, s32, CDrawArgs *);
void func_0020B6A0(s32, s32, s32, s32, char *, s32, s32);
s32 **func_00203CA0(s32, s32, s32, s32, s32, CGameObjectInstance *, void *, char *, s32);
void func_0020565C(s32, s32 **);
void func_00214F68(char *, char *);
void func_002051F4(s32, s32 **);
void func_00220208();

void func_004089E0(CGameObjectInstance *actor, s32 arg1, CDrawArgs *params) {
    s32 **resource;
    s32 *header;
    char *object;
    char *data;

    func_0021EBE4(actor, params->m_nVariant, &params->m_nModel, &params->m_nShadowModel);
    actor->m_AI.m_VisBits = 1 << params->m_nModel;
    if (params->m_bShadow != 0) {
        func_0021DC84(actor, arg1, params);
    }
    if (params->m_bDraw == 0) {
        return;
    }
    resource = func_00203CA0(0, actor->m_dwUniqueID | D_800ACAD8, params->m_hModel, 0, 0, actor,
                             func_00220208, D_800C0570, 1);
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
    func_0020B6A0(params->m_hModel, ((s32)actor->m_pmtDrawMtxs), 1, 0, data, D_8012FB64, -1);
    func_002051F4(0, resource);
}
