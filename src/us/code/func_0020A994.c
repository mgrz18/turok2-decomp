/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER_4/8026AC38_3.CPP, func_8026C020_de, with Turok 2's layouts. */

#include "common.h"
#include "gfx.h"
#include "gbi_t2.h"
#include "globals/batch.h"



s32 Render_SetupMaterial(Material *material, s32 pass);
void func_00209658(void);

/* Emits the batched draws: the opaque nodes in key order (those whose material is flagged 0x4000
 * set aside and drawn afterwards with their alpha as primitive colour, after func_00209658), then
 * the translucent ones. Each item loads its lights, its matrix (or segment 1) and its textures as
 * segment 2, sets the texture scale when it changes, and calls the part's display list. */
void Render_DrawBatches(void) {
    SortNode *node;
    SortNode *deferred;
    SortNode *next;
    DrawItem *item;
    u16 scaleS;
    u16 scaleT;

    node = sOpaqueSortFirst;
    deferred = NULL;
    if (node != NULL) {
        do {
            item = node->head;
            if (item != NULL && Render_SetupMaterial(node->material, node->pass)) {
                scaleT = 0;
                scaleS = 0;
                do {
                    if (item->lights != NULL) {
                        GFX_W(gDList++, 0xDB020000, 0x18);                  /* gSPNumLights(1) */
                        GFX_W(gDList++, 0xDC08060A, GBI_ADDR(item->lights + 8)); /* gSPLight(lights + 8, 1) */
                        GFX_W(gDList++, 0xDC08090A, GBI_ADDR(item->lights));       /* gSPLight(lights, 2) */
                    }
                    if (item->segment != 0) {
                        GFX_W(gDList++, 0xDB060004, GBI_ADDR(item->matrix));       /* gSPSegment(1, matrix) */
                    } else {
                        GFX_W(gDList++, 0xDA380003, GBI_ADDR(item->matrix));       /* gSPMatrix(matrix, G_MTX_LOAD) */
                    }
                    GFX_W(gDList++, 0xDB060008, GBI_ADDR(item->textures));         /* gSPSegment(2, textures) */
                    if (item->scaleS != 0) {
                        if (scaleS != item->scaleS || scaleT != item->scaleT) {
                            if (node->material->flags & 0x2000) {
                                GFX_W(gDList++, 0xD7002802, (item->scaleS << 16) | item->scaleT); /* gSPTexture(s, t, 5, 0, on) */
                            } else {
                                GFX_W(gDList++, 0xD7000002, (item->scaleS << 16) | item->scaleT); /* gSPTexture(s, t, 0, 0, on) */
                            }
                        }
                        scaleS = item->scaleS;
                        scaleT = item->scaleT;
                    }
                    GFX_W(gDList++, 0xDE000000, GBI_ADDR(item->list));             /* gSPDisplayList */
                    item = item->next;
                } while (item != NULL);
                if (node->material->flags & 0x2000) {
                    GFX_W(gDList++, 0xE3000F00, 0);                         /* gDPSetTextureLOD(G_TL_TILE) */
                }
                if (node->material->flags & 0x4000) {
                    next = node->next;
                    node->next = deferred;
                    deferred = node;
                    node = next;
                } else {
                    node = node->next;
                }
            } else {
                node = node->next;
            }
        } while (node != NULL);
    }
    if (deferred != NULL) {
        node = deferred;
        func_00209658();
        do {
            item = node->head;
            if (item != NULL) {
                GFX_W(gDList++, 0xFA000000, node->material->alpha);         /* gDPSetPrimColor(0, 0, 0, 0, 0, alpha) */
                do {
                    if (item->lights != NULL) {
                        GFX_W(gDList++, 0xDB020000, 0x18);
                        GFX_W(gDList++, 0xDC08060A, GBI_ADDR(item->lights + 8));
                        GFX_W(gDList++, 0xDC08090A, GBI_ADDR(item->lights));
                    }
                    if (item->segment != 0) {
                        GFX_W(gDList++, 0xDB060004, GBI_ADDR(item->matrix));
                    } else {
                        GFX_W(gDList++, 0xDA380003, GBI_ADDR(item->matrix));
                    }
                    GFX_W(gDList++, 0xDB060008, GBI_ADDR(item->textures));
                    GFX_W(gDList++, 0xDE000000, GBI_ADDR(item->list));
                    item = item->next;
                } while (item != NULL);
            }
            node = node->next;
        } while (node != NULL);
    }
    node = sTranslucentSortFirst;
    if (node != NULL) {
        do {
            item = node->head;
            if (item != NULL && Render_SetupMaterial(node->material, node->pass)) {
                scaleT = 0;
                scaleS = 0;
                do {
                    if (item->lights != NULL) {
                        GFX_W(gDList++, 0xDB020000, 0x18);
                        GFX_W(gDList++, 0xDC08060A, GBI_ADDR(item->lights + 8));
                        GFX_W(gDList++, 0xDC08090A, GBI_ADDR(item->lights));
                    }
                    if (item->segment != 0) {
                        GFX_W(gDList++, 0xDB060004, GBI_ADDR(item->matrix));
                    } else {
                        GFX_W(gDList++, 0xDA380003, GBI_ADDR(item->matrix));
                    }
                    GFX_W(gDList++, 0xDB060008, GBI_ADDR(item->textures));
                    if (item->scaleS != 0) {
                        if (scaleS != item->scaleS || scaleT != item->scaleT) {
                            if (node->material->flags & 0x2000) {
                                GFX_W(gDList++, 0xD7002802, (item->scaleS << 16) | item->scaleT);
                            } else {
                                GFX_W(gDList++, 0xD7000002, (item->scaleS << 16) | item->scaleT);
                            }
                        }
                        scaleS = item->scaleS;
                        scaleT = item->scaleT;
                    }
                    GFX_W(gDList++, 0xDE000000, GBI_ADDR(item->list));
                    item = item->next;
                } while (item != NULL);
                if (node->material->flags & 0x2000) {
                    GFX_W(gDList++, 0xE3000F00, 0);
                }
            }
            node = node->next;
        } while (node != NULL);
    }
}
