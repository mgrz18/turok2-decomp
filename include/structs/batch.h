#ifndef STRUCTS_BATCH_H
#define STRUCTS_BATCH_H

/* The render batcher (RENDER_4 in Turok: Rage Wars): Render_QueueModelInstance
 * queues instances of one model, Render_FlushModelBatch files each part under
 * its material key into the opaque or the translucent sort tree, and
 * Render_DrawBatches emits the trees in key order. */

#include "common.h"

/* A part's material. */
typedef struct Material {
    /* 0x00 */ u32 flags;           /* 0x700 translucent; 0x4000 drawn last with its alpha */
    /* 0x04 */ u32 id;              /* the sort key's upper 24 bits */
    /* 0x08 */ u8 pad08[0xF];
    /* 0x17 */ u8 alpha;
    /* 0x18 */ u16 scaleS;          /* texture scale */
    /* 0x1A */ u16 scaleT;
} Material;

/* One draw of a part with one instance's state. */
typedef struct DrawItem {
    /* 0x00 */ void *list;
    /* 0x04 */ void *matrix;
    /* 0x08 */ char *lights;
    /* 0x0C */ void *textures;
    /* 0x10 */ u16 scaleS;
    /* 0x12 */ u16 scaleT;
    /* 0x14 */ s32 segment;
    /* 0x18 */ struct DrawItem *next;
} DrawItem; /* size 0x1C */

/* A material key in a sort tree, threaded in key order through next/prev. */
typedef struct SortNode {
    union {
        /* 0x00 */ u32 key;         /* material id << 8 | pass */
        struct {
            /* 0x00 */ u8 id[3];
            /* 0x03 */ s8 pass;     /* the key's low byte */
        };
    };
    /* 0x04 */ Material *material;
    /* 0x08 */ DrawItem *head;
    /* 0x0C */ DrawItem *tail;
    /* 0x10 */ struct SortNode *prev;
    /* 0x14 */ struct SortNode *next;
    /* 0x18 */ struct SortNode *left;
    /* 0x1C */ struct SortNode *right;
} SortNode; /* size 0x20 */

#endif
