/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER_4/CODE_8026D844.CPP, Render_QueueModelInstance, with Turok 2's layouts. */

#include "common.h"
#include "structs/model.h"
#include "globals/batch.h"
#include "protos/iset.h"
#include "protos/cache.h"

void Render_FlushModelBatch(void);

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
    if (sBatchResource != resource || sBatchLod != lod || sBatchPass != pass || sBatchInstanceCount == 0x20) {
        Render_FlushModelBatch();
        if (sBatchResourceCount == 0x100) {
            return;
        }
        CCache__TouchAndLock(0, resource);
        sBatchResources[sBatchResourceCount++] = resource;
    }
    sBatchResource = resource;
    sBatchLod = lod;
    sBatchPass = pass;
    inst = &sBatchInstances[sBatchInstanceCount++];
    inst->m_pMatrix = matrix;
    inst->m_Segment = segment;
    inst->m_pLights = lights;
    inst->m_pTextures = textures;
}
