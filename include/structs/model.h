#ifndef STRUCTS_MODEL_H
#define STRUCTS_MODEL_H

/* Drawing models: what an actor's draw callback gets (CDrawArgs, filled by
 * CGameObjectInstance__Draw) and the model batch (Render_QueueModelInstance queues one instance of a
 * model's level of detail, Render_QueueModelByKey / func_0020B9F4 load the model
 * first, Render_FlushModelBatch flushes the batch through its material sort tree).
 * Rage Wars has the batch in src/RENDER_4 (Render_QueueModelInstance).
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "common.h"

/* An actor's draw callback (CGameObjectInstance + 0x234) gets this from
 * CGameObjectInstance__Draw, and passes it on to CGameObjectInstance__DrawShadow (the shadow). */
typedef struct CDrawArgs {
    /* 0x00 */ s32 m_bDraw;                 /* in the view: draw the model */
    /* 0x04 */ s32 m_bShadow;               /* near enough: draw the shadow */
    /* 0x08 */ struct CCamera *m_pCamera;
    /* 0x0C */ s32 m_hModel;                /* the model resource (a handle, void **) */
    /* 0x10 */ f32 *m_pOffset;              /* where the shadow goes, from the actor */
    /* 0x14 */ s32 m_nVariant;              /* which of the intelligence's models */
    /* 0x18 */ s32 m_nModel;                /* set by the callback: (&m_nModel)[m_nVariant] or a death model */
    /* 0x1C */ s32 m_nShadowModel;          /* likewise from m_nShadowModel */
} CDrawArgs; /* size 0x20 */

/* One queued draw of the batched model. */
typedef struct CModelInstance {
    /* 0x00 */ void *m_pMatrix;
    /* 0x04 */ void *m_pLights;
    /* 0x08 */ void *m_pTextures;           /* NULL: the model's own (Render_FlushModelBatch) */
    /* 0x0C */ s32 m_Segment;
} CModelInstance; /* size 0x10 */

extern CModelInstance D_800DD578[32];    /* the batch: D_800D8E34 of them queued */

#endif
