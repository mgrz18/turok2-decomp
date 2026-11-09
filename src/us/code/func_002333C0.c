/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802A25D0_de_3.c, func_802A25D0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py.
 * On structs/swoosh.h's Swoosh and SwooshNode (Rage Wars' ObjectLinks54 / ObjectLinksB4). */

#include "common.h"
#include "structs/swoosh.h"

/* The swoosh record (CScene__GetSwoosh): a new point's life, and whether the texture runs
 * along the swoosh's length (0) or with time. */
typedef struct SwooshInfo20 {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ f32 nodeLife;
    /* 0x0C */ u8 padC[0x10];
    /* 0x1C */ u8 byTime;
} SwooshInfo20;

extern SwooshNode *Swoosh_TakeNode(CSwooshSystem *arg0, Swoosh *arg1);
extern void CMtxF__GetTranslation(void *object, f32 *output);
extern void CMtxF__GetScale(void *arg0, f32 *arg1);
extern void CMtxF__ToMtxAffine(void *arg0, void *arg1);
extern void CVector3__Subtract(CVector3 *arg0, CVector3 *arg1, CVector3 *arg2);
extern f32 func_00298470(f32);

/* Adds a point to swoosh arg1 at matrix arg2 and spreads the points along its length. */
SwooshNode *Swoosh_NewNode(CSwooshSystem *arg0, Swoosh *arg1, void *arg2) {
    CVector3 sp10;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 var_f1;
    SwooshNode *temp_s1;
    SwooshNode *var_s0;
    SwooshNode *temp_s2;
    temp_s2 = Swoosh_TakeNode(arg0, arg1);
    var_s0 = temp_s2;
    if (temp_s2 != 0) {
        arg1->unk50 = 0;
        var_s0->life = ((SwooshInfo20 *) arg1->info)->nodeLife;
        var_s0->owner = 0;
        CMtxF__GetTranslation(arg2, &var_s0->pos.x);
        if (arg1->flags & 4) {
            CMtxF__GetScale(arg2, &var_s0->scale.x);
        } else {
            CMtxF__ToMtxAffine(arg2, &var_s0->mtx[0]);
            CMtxF__ToMtxAffine(arg2, &var_s0->mtx[1]);
        }
        temp_s1 = var_s0->prev;
        *(s32 *) &var_s0->length = 0;
        var_s0->fraction = 0.0f;
        if (temp_s1 != 0) {
            CVector3__Subtract(&sp10, &var_s0->pos, &temp_s1->pos);
            temp_f0 = func_00298470((sp10.x * sp10.x) + (sp10.y * sp10.y) + (sp10.z * sp10.z));
            temp_s1->length = temp_f0;
            arg1->length += temp_f0;
        }
        if (((SwooshInfo20 *) arg1->info)->byTime == 0) {
            if (arg1->length > 0.0f) {
                var_f1 = 0.0f;
                var_s0 = arg1->nodes.head;
                if (var_s0 != 0) {
                    do {
                        var_s0->fraction = var_f1 / arg1->length;
                        temp_f0_2 = var_s0->length;
                        var_s0 = var_s0->next;
                        var_f1 += temp_f0_2;
                    } while (var_s0 != 0);
                }
            }
        } else {
            if (temp_s1 != 0) {
                arg1->unk28 += temp_s1->length * 0.005f;
            }
            var_s0->fraction = arg1->unk28;
        }
    }
    return temp_s2;
}
