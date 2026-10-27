#include "common.h"

typedef struct Sweep {
    /* 0x00 */ u8 pad0[0x9C];
    /* 0x9C */ s32 done;
} Sweep;

typedef struct SweepModel {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ struct { u8 pad[0xE]; s8 variant; } *def;
    /* 0x18 */ u8 pad18[0x10];
    /* 0x28 */ u8 matrix[0x40];
} SweepModel;

typedef struct ModelSweep {
    /* 0x00 */ Sweep *sweep;
    /* 0x04 */ f32 pos[0x12];
    /* 0x4C */ SweepModel *model;
    /* 0x50 */ s32 kind;
    /* 0x54 */ f32 mtx[0x10];
    /* 0x94 */ f32 inverse[0x10];
    /* 0xD4 */ void *meshes;
} ModelSweep;

char *func_002017D4(s32 *list, s32 index);
void func_002051F4(s32 arg0, s32 *arg1);
void func_0020C618(f32 *dst, f32 *a, f32 *b);
void func_0020D908(f32 *mtx, void *packed);
s32 *func_00214DBC(void *model);
void func_0023D7CC(ModelSweep *ms, char *group);

/* Sweep against every polygon group of the model's current variant, until a hit. */
void func_0023DBBC(ModelSweep *ms) {
    SweepModel *model;
    s32 *data;
    s32 *root;
    s32 *groups;
    s32 count;
    Sweep *sweep;
    s32 i;

    model = ms->model;
    data = func_00214DBC(model);
    if (data != NULL) {
        func_0020D908(ms->mtx, model->matrix);
        func_0020C618(ms->inverse, ms->mtx, ms->pos);
        root = (s32 *)*data;
        ms->meshes = func_002017D4(root, 0);
        groups = (s32 *)func_002017D4((s32 *)func_002017D4(root, 2), model->def->variant);
        count = *groups;
        sweep = ms->sweep;
        for (i = 0; i < count && sweep->done == 0; i++) {
            func_0023D7CC(ms, func_002017D4(groups, i));
        }
        func_002051F4(0, data);
    }
}
