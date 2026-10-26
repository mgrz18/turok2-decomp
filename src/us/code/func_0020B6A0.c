/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER_4/CODE_8026D844.CPP, Render_QueueModelInstance, with Turok 2's layouts. */

#include "common.h"

/* One queued draw of the batched model. */
typedef struct ModelInstance {
    /* 0x00 */ void *matrix;
    /* 0x04 */ void *lights;
    /* 0x08 */ void *textures;
    /* 0x0C */ s32 segment;
} ModelInstance;

extern void **D_800D8E28;           /* batched model */
extern s32 D_800D8E14;              /* its level of detail */
extern s32 D_800D8E10;              /* its pass */
extern s32 D_800D8E34;              /* queued instance count */
extern ModelInstance D_800DD578[32];
extern s32 D_800DD170;              /* registered model count */
extern void **D_800DD178[0x100];

void *func_002017D4(void *resource, s32 index);
void func_0020A568(void);
void func_00205110(s32 unused, void **resource);

/* Queues an instance of one level of detail of a model resource. A different model, level or
 * pass, or a full queue, first flushes the batch (func_0020A568) and registers the model, unless
 * 256 models are already registered. */
void func_0020B6A0(void **resource, void *matrix, s32 segment, void *lights, void *textures, s32 lod, s32 pass) {
    s32 *parts;
    ModelInstance *inst;

    parts = func_002017D4(*resource, 2);
    if (lod < 0 || lod >= *parts) {
        return;
    }
    if (D_800D8E28 != resource || D_800D8E14 != lod || D_800D8E10 != pass || D_800D8E34 == 0x20) {
        func_0020A568();
        if (D_800DD170 == 0x100) {
            return;
        }
        func_00205110(0, resource);
        D_800DD178[D_800DD170++] = resource;
    }
    D_800D8E28 = resource;
    D_800D8E14 = lod;
    D_800D8E10 = pass;
    inst = &D_800DD578[D_800D8E34++];
    inst->matrix = matrix;
    inst->segment = segment;
    inst->lights = lights;
    inst->textures = textures;
}
