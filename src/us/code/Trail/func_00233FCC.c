/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802A25D0_de_8.c, func_802A339C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/swoosh.h"
#include "globals/engine.h"

void CMtxF__SetScale(void *mf, f32 x, f32 y, f32 z);
void CMtxF__Identity(void *mf);
void Trail_ComputeNodeFrame(SwooshNode *e0, SwooshNode *e1, u8 *arg2, SwooshFrame *out);
void CMtxF__ToMtxAffine(void *mf, void *mtx);
/* The swoosh's points (structs/swoosh.h): each one's frame (Trail_ComputeNodeFrame) toward the next
 * point at another position, as an Mtx in this frame buffer's slot. */
void Trail_ComputeFrames(void *arg0, sptr arg1) {
    f32 matrix[16];
    SwooshNode *first;
    SwooshNode *next;
    Swoosh *state = arg0;
    if (state->nodes.count < 2) {
        return;
    }
    first = state->nodes.head;
    next = first->next;
    if (first != 0) {
        CMtxF__SetScale(matrix, first->scale.x, first->scale.y, first->scale.z);
    } else {
        CMtxF__Identity(matrix);
    }
    while (first != 0) {
        SwooshNode *a;
        SwooshNode *b;
        if (next == 0) {
            break;
        }
        a = first;
        b = next;
        if (first == 0) {
            goto no_pair;
        }
        if (first->pos.x != next->pos.x || first->pos.y != next->pos.y) {
            goto boundary;
        }
z_test:
        if (a->pos.z != b->pos.z) {
            goto boundary;
        }
        a = b;
        b = b->next;
        if (a == 0) {
            goto no_pair;
        }
        if (b == 0 || a->pos.x != b->pos.x || a->pos.y != b->pos.y) {
            goto boundary;
        }
        goto z_test;
boundary:
        if (a != 0 && b != 0) {
            Trail_ComputeNodeFrame(a, b, (u8 *) arg1, (SwooshFrame *) matrix);
            goto apply;
        }
no_pair:
        {
            matrix[12] = first->pos.x;
            matrix[13] = first->pos.y;
            matrix[14] = first->pos.z;
        }
apply:
        CMtxF__ToMtxAffine(matrix, &first->mtx[gEvenOdd]);
        first = next;
        next = first->next;
    }
    matrix[12] = first->pos.x;
    matrix[13] = first->pos.y;
    matrix[14] = first->pos.z;
    CMtxF__ToMtxAffine(matrix, &first->mtx[gEvenOdd]);
}
