#include "common.h"
#include "structs/collision.h"
#include "protos/iset.h"
#include "protos/cache.h"

typedef struct SweepModel {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ struct { u8 pad[0xE]; s8 variant; } *def;
    /* 0x18 */ u8 pad18[0x10];
    /* 0x28 */ u8 matrix[0x40];
} SweepModel;

void CMtxF__MultiplyAffine(f32 (*dst)[4], f32 (*a)[4], f32 (*b)[4]);
void CMtxF__FromMtxAffine(f32 (*mtx)[4], void *packed);
s32 *CStaticObject__LoadGeometry(void *model);
void ModelSweep_TestGroup(ModelSweep *ms, char *group);

/* Sweep against every polygon group of the model's current variant, until a hit. */
void ModelSweep_TestModel(ModelSweep *ms) {
    SweepModel *model;
    s32 *data;
    s32 *root;
    s32 *groups;
    s32 count;
    Sweep *sweep;
    s32 i;

    model = ms->model;
    data = CStaticObject__LoadGeometry(model);
    if (data != NULL) {
        CMtxF__FromMtxAffine(ms->mtx, model->matrix);
        CMtxF__MultiplyAffine(ms->inverse, ms->mtx, ms->view);
        root = (s32 *)*data;
        ms->meshes = CISet__GetBlock(root, 0);
        groups = (s32 *)CISet__GetBlock((s32 *)CISet__GetBlock(root, 2), model->def->variant);
        count = *groups;
        sweep = ms->sweep;
        for (i = 0; i < count && sweep->stop == 0; i++) {
            ModelSweep_TestGroup(ms, CISet__GetBlock(groups, i));
        }
        CCache__Unlock(0, data);
    }
}
