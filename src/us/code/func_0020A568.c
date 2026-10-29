/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER_4/8026AC38_2.CPP, func_8026BC60_de, with Turok 2's layouts. */

#include "common.h"

/* One queued draw of the batched model (func_0020B6A0). */
typedef struct ModelInstance {
    /* 0x00 */ void *matrix;
    /* 0x04 */ void *lights;
    /* 0x08 */ void *textures;
    /* 0x0C */ s32 segment;
} ModelInstance;

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
extern s32 D_800D8E14;              /* level of detail of the batched model */
extern SortNode *D_800D8E18;        /* opaque tree root */
extern SortNode *D_800D8E1C;        /* its first node */
extern s32 D_800D8E20;              /* draw items used */
extern void **D_800D8E28;           /* batched model */
extern s32 D_800D8E2C;              /* sort nodes used */
extern SortNode *D_800D8E30;        /* translucent tree root */
extern s32 D_800D8E34;              /* queued instance count */
extern SortNode *D_800D8E38;        /* its first node */
extern DrawItem D_800D8E40[500];
extern SortNode D_800DC4F0[100];
extern ModelInstance D_800DD578[32];

void *func_002017D4(void *resource, s32 index);

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
void func_0020A568(void) {
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
    ModelInstance *inst;

    if (D_800D8E34 != 0) {
        header = *D_800D8E28;
        for (j = 0; j < D_800D8E34; j++) {
            inst = &D_800DD578[j];
            if (inst->textures == NULL) {
                inst->textures = func_002017D4(header, 0);
            }
        }
        parts = func_002017D4(func_002017D4(header, 2), D_800D8E14);
        count = *parts;
        for (i = 0; i < count; i++) {
            part = func_002017D4(parts, i);
            material = func_002017D4(part, 0);
            key = (material->id << 8) | D_800D8E13;
            if (material->flags & 0x700) {
                parent = find_node(D_800D8E30, key);
            } else {
                parent = find_node(D_800D8E18, key);
            }
            if (parent != NULL && parent->key == key) {
                node = parent;
            } else {
                if (D_800D8E2C == 100) {
                    break;
                }
                node = &D_800DC4F0[D_800D8E2C++];
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
                            if (D_800D8E38 == parent) {
                                D_800D8E38 = node;
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
                        D_800D8E30 = node;
                        D_800D8E38 = node;
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
                            if (D_800D8E1C == parent) {
                                D_800D8E1C = node;
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
                        D_800D8E18 = node;
                        D_800D8E1C = node;
                        node->right = NULL;
                        node->left = NULL;
                        node->next = NULL;
                        node->prev = NULL;
                    }
                }
            }
            list = func_002017D4(part, 1);
            for (j = 0; j < D_800D8E34; j++) {
                inst = &D_800DD578[j];
                if (D_800D8E20 != 500) {
                    item = &D_800D8E40[D_800D8E20++];
                    item->list = list;
                    item->matrix = inst->matrix;
                    item->lights = inst->lights;
                    item->textures = inst->textures;
                    item->unk10 = material->unk18;
                    item->unk12 = material->unk1A;
                    item->segment = inst->segment;
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
        D_800D8E34 = 0;
    }
}
