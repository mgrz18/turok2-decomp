/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER_4/CODE_8026D844.CPP, Render_QueueModelInstance, with Turok 2's layouts. */

#include "common.h"
#include "structs/model.h"

extern void **D_800D8E28;           /* batched model */
extern s32 D_800D8E14;              /* its level of detail */
extern s32 D_800D8E10;              /* its pass */
extern s32 D_800D8E34;              /* queued instance count */
extern s32 D_800DD170;              /* registered model count */
extern void **D_800DD178[0x100];

void *CISet__GetBlock(void *resource, s32 index);
void Render_FlushModelBatch(void);
void CCache__TouchAndLock(s32 unused, void **resource);

/* Queues an instance of one level of detail of a model resource. A different model, level or
 * pass, or a full queue, first flushes the batch (Render_FlushModelBatch) and registers the model, unless
 * 256 models are already registered. */
void Render_QueueModelInstance(void **resource, void *matrix, s32 segment, void *lights, void *textures, s32 lod, s32 pass) {
    s32 *parts;
    CModelInstance *inst;

    parts = CISet__GetBlock(*resource, 2);
    if (lod < 0 || lod >= *parts) {
        return;
    }
    if (D_800D8E28 != resource || D_800D8E14 != lod || D_800D8E10 != pass || D_800D8E34 == 0x20) {
        Render_FlushModelBatch();
        if (D_800DD170 == 0x100) {
            return;
        }
        CCache__TouchAndLock(0, resource);
        D_800DD178[D_800DD170++] = resource;
    }
    D_800D8E28 = resource;
    D_800D8E14 = lod;
    D_800D8E10 = pass;
    inst = &D_800DD578[D_800D8E34++];
    inst->m_pMatrix = matrix;
    inst->m_Segment = segment;
    inst->m_pLights = lights;
    inst->m_pTextures = textures;
}
