/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER_4/8026AC38_2.CPP, func_8026BC60_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/model.h"

/* A part's material. */
typedef struct Material {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u32 id;
    /* 0x08 */ u8 pad8[0x10];
    /* 0x18 */ u16 unk18;
    /* 0x1A */ u16 unk1A;
} Material;

/* One draw of a part with one instance's state. */
typedef struct DrawItem {
    /* 0x00 */ void *list;
    /* 0x04 */ void *matrix;
    /* 0x08 */ void *lights;
    /* 0x0C */ void *textures;
    /* 0x10 */ u16 unk10;
    /* 0x12 */ u16 unk12;
    /* 0x14 */ s32 segment;
    /* 0x18 */ struct DrawItem *next;
} DrawItem; /* size 0x1C */

/* A material key in a sort tree, threaded in key order through next/prev. */
typedef struct SortNode {
    /* 0x00 */ u32 key;
    /* 0x04 */ Material *material;
    /* 0x08 */ DrawItem *head;
    /* 0x0C */ DrawItem *tail;
    /* 0x10 */ struct SortNode *prev;
    /* 0x14 */ struct SortNode *next;
    /* 0x18 */ struct SortNode *left;
    /* 0x1C */ struct SortNode *right;
} SortNode; /* size 0x20 */

extern u8 D_800D8E13;               /* low byte of the pass */
extern s32 sBatchLod;              /* level of detail of the batched model */
extern SortNode *sOpaqueSortRoot;        /* opaque tree root */
extern SortNode *sOpaqueSortFirst;        /* its first node */
extern s32 sDrawItemCount;              /* draw items used */
extern void **sBatchResource;           /* batched model */
extern s32 sSortNodeCount;              /* sort nodes used */
extern SortNode *sTranslucentSortRoot;        /* translucent tree root */
extern s32 sBatchInstanceCount;              /* queued instance count */
extern SortNode *sTranslucentSortFirst;        /* its first node */
extern DrawItem sDrawItems[500];
extern SortNode sSortNodes[100];

void *CISet__GetBlock(void *resource, s32 index);

/* Returns the node keyed key in the tree under n, or the node a new key would hang from. */
static __inline__ SortNode *find_node(SortNode *n, u32 key) {
    SortNode *parent = NULL;

    while (n != NULL) {
        if (n->key == key) {
            return n;
        }
        parent = n;
        if (key < n->key) {
            n = n->left;
        } else {
            n = n->right;
        }
    }
    return parent;
}

/* Flushes the batched model: files each part of its level of detail under its material key into
 * the opaque or the translucent (flags 0x700) sort tree, allocating a node when the key is new,
 * and appends one draw item per queued instance to the node's list; then empties the queue. */
void Render_FlushModelBatch(void) {
    void *header;
    s32 *parts;
    s32 count;
    s32 i;
    s32 j;
    void *part;
    Material *material;
    u32 key;
    SortNode *parent;
    SortNode *node;
    void *list;
    DrawItem *item;
    CModelInstance *inst;

    if (sBatchInstanceCount != 0) {
        header = *sBatchResource;
        for (j = 0; j < sBatchInstanceCount; j++) {
            inst = &sBatchInstances[j];
            if (inst->m_pTextures == NULL) {
                inst->m_pTextures = CISet__GetBlock(header, 0);
            }
        }
        parts = CISet__GetBlock(CISet__GetBlock(header, 2), sBatchLod);
        count = *parts;
        for (i = 0; i < count; i++) {
            part = CISet__GetBlock(parts, i);
            material = CISet__GetBlock(part, 0);
            key = (material->id << 8) | D_800D8E13;
            if (material->flags & 0x700) {
                parent = find_node(sTranslucentSortRoot, key);
            } else {
                parent = find_node(sOpaqueSortRoot, key);
            }
            if (parent != NULL && parent->key == key) {
                node = parent;
            } else {
                if (sSortNodeCount == 100) {
                    break;
                }
                node = &sSortNodes[sSortNodeCount++];
                node->key = key;
                node->material = material;
                node->tail = NULL;
                node->head = NULL;
                if (material->flags & 0x700) {
                    if (parent != NULL) {
                        if (node->key < parent->key) {
                            parent->left = node;
                            node->right = NULL;
                            node->left = NULL;
                            node->next = parent;
                            node->prev = parent->prev;
                            if (parent->prev != NULL) {
                                parent->prev->next = node;
                            }
                            parent->prev = node;
                            if (sTranslucentSortFirst == parent) {
                                sTranslucentSortFirst = node;
                            }
                        } else {
                            parent->right = node;
                            node->right = NULL;
                            node->left = NULL;
                            node->next = parent->next;
                            if (parent->next != NULL) {
                                parent->next->prev = node;
                            }
                            node->prev = parent;
                            parent->next = node;
                        }
                    } else {
                        sTranslucentSortRoot = node;
                        sTranslucentSortFirst = node;
                        node->right = NULL;
                        node->left = NULL;
                        node->next = NULL;
                        node->prev = NULL;
                    }
                } else {
                    if (parent != NULL) {
                        if (node->key < parent->key) {
                            parent->left = node;
                            node->right = NULL;
                            node->left = NULL;
                            node->next = parent;
                            node->prev = parent->prev;
                            if (parent->prev != NULL) {
                                parent->prev->next = node;
                            }
                            parent->prev = node;
                            if (sOpaqueSortFirst == parent) {
                                sOpaqueSortFirst = node;
                            }
                        } else {
                            parent->right = node;
                            node->right = NULL;
                            node->left = NULL;
                            node->next = parent->next;
                            if (parent->next != NULL) {
                                parent->next->prev = node;
                            }
                            node->prev = parent;
                            parent->next = node;
                        }
                    } else {
                        sOpaqueSortRoot = node;
                        sOpaqueSortFirst = node;
                        node->right = NULL;
                        node->left = NULL;
                        node->next = NULL;
                        node->prev = NULL;
                    }
                }
            }
            list = CISet__GetBlock(part, 1);
            for (j = 0; j < sBatchInstanceCount; j++) {
                inst = &sBatchInstances[j];
                if (sDrawItemCount != 500) {
                    item = &sDrawItems[sDrawItemCount++];
                    item->list = list;
                    item->matrix = inst->m_pMatrix;
                    item->lights = inst->m_pLights;
                    item->textures = inst->m_pTextures;
                    item->unk10 = material->unk18;
                    item->unk12 = material->unk1A;
                    item->segment = inst->m_Segment;
                    item->next = NULL;
                    if (node->tail != NULL) {
                        node->tail->next = item;
                    } else {
                        node->head = item;
                    }
                    node->tail = item;
                }
            }
        }
        sBatchInstanceCount = 0;
    }
}
