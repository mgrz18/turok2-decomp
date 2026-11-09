/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER_4/8026AC38_3.CPP, func_8026C484_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/model.h"
#include "globals/batch.h"
#include "protos/iset.h"

extern char D_800A53D8[];
extern char D_800A53EC[];

void **CCache__Request(s32, s32 key, s32 size, s32 tag, s32, void *callback, void *name, s32 flag);
void CCache__Unlock(s32 unused, void **resource);
void Render_FlushModelBatch(void);
void CCache__TouchAndLock(s32 unused, void **resource);
void Model_DecompressCallbackTerminated();

/* Loads the model resource for key through its 0x18-byte header, then queues an instance of one
 * level of detail of it with the model's own textures, as a loaded matrix (see Render_QueueModelInstance), and
 * releases both. */
void Render_QueueModelByKey(s32 key, void *matrix, void *lights, s32 lod, s32 pass) {
    void **header;
    void **resource;
    s32 *parts;
    s32 size;
    s32 dataKey;
    CModelInstance *inst;
    s32 n;

    header = CCache__Request(0, key, 0x18, 0, 0, 0, D_800A53D8, 1);
    if (header == 0) {
        return;
    }
    dataKey = CISet__GetROMAddressAndSize(*header, key, 1, &size);
    resource = CCache__Request(0, dataKey, size, 0, 0, Model_DecompressCallbackTerminated, D_800A53EC, 1);
    if (resource != 0) {
        parts = CISet__GetBlock(*resource, 2);
        if (lod >= 0 && lod < *parts) {
            if (sBatchResource != resource || sBatchLod != lod || sBatchPass != pass || sBatchInstanceCount == 0x20) {
                Render_FlushModelBatch();
                if (sBatchResourceCount == 0x100) {
                    goto done;
                }
                CCache__TouchAndLock(0, resource);
                sBatchResources[sBatchResourceCount] = resource;
                sBatchResourceCount += 1;
            }
            sBatchResource = resource;
            sBatchLod = lod;
            sBatchPass = pass;
            n = sBatchInstanceCount;
            inst = &sBatchInstances[n++];
            sBatchInstanceCount = n;
            inst->m_pMatrix = matrix;
            inst->m_Segment = 0;
            inst->m_pLights = lights;
            inst->m_pTextures = 0;
        }
    done:
        CCache__Unlock(0, resource);
    }
    CCache__Unlock(0, header);
}
