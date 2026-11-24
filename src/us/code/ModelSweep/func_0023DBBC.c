#include "common.h"
#include "structs/collision.h"
#include "protos/iset.h"
#include "protos/cache.h"
#include "structs/level_actor.h"
#include "structs/intelligence.h"

void CMtxF__MultiplyAffine(f32 (*dst)[4], f32 (*a)[4], f32 (*b)[4]);
void CMtxF__FromMtxAffine(f32 (*mtx)[4], void *packed);
s32 *CStaticObject__LoadGeometry(void *model);
void ModelSweep_TestGroup(ModelSweep *ms, char *group);

/* Sweep against every polygon group of the model's current variant, until a hit. */
void ModelSweep_TestModel(ModelSweep *ms) {
    CStaticObject *model;
    s32 *data;
    s32 *root;
    s32 *groups;
    s32 count;
    Sweep *sweep;
    s32 i;

    model = ms->model;
    data = CStaticObject__LoadGeometry(model);
    if (data != NULL) {
        CMtxF__FromMtxAffine(ms->mtx, model->m_Matrix);
        CMtxF__MultiplyAffine(ms->inverse, ms->mtx, ms->view);
        root = *(s32 **)data; /* the cache entry holds a host pointer */
        ms->meshes = CISet__GetBlock(root, 0);
        groups = (s32 *)CISet__GetBlock((s32 *)CISet__GetBlock(root, 2), model->header.m_pIntelligence->m_nModel);
        count = *groups;
        sweep = ms->sweep;
        for (i = 0; i < count && sweep->stop == 0; i++) {
            ModelSweep_TestGroup(ms, CISet__GetBlock(groups, i));
        }
        CCache__Unlock(0, data);
    }
}
