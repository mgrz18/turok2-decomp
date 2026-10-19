/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8028DF90_de_7.c, func_8028FC10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_0028AFF0(struct OSSched *arg0, void *arg1);
extern void func_0028B274(void *arg0);
extern s32 osRecvMesg(OSMesgQueue *queue, void **message, s32 block);
extern int D_800B6E88;
extern s32 func_0028ABE0(struct Ctx_func_8028F544_de *, struct Node_func_8028F544_de **, struct Node_func_8028F544_de **, s32);
extern void func_0028B0FC(struct View_func_8028FA60_de *, struct Scene_func_8028FA60_de *, struct Scene_func_8028FA60_de *);
extern void func_0029B9B0(void *arg0, s32 arg1);
extern s32 D_800B6E54;
extern s32 D_800D6D68;
s32 func_0028ABE0(Ctx_func_8028F544_de *arg0, Node_func_8028F544_de **arg1, Node_func_8028F544_de **arg2, s32 arg3);
void func_0028AFF0(OSSched *arg0, void *arg1);
void func_0028B0FC(View_func_8028FA60_de *view, Scene_func_8028FA60_de *current, Scene_func_8028FA60_de *next);
void func_0028B274(void *arg0);
void func_0028B2AC(void *arg0) {
    Node_func_8028FC10_de *node = 0;
    s32 value1 = 0;
    s32 value2 = 0;
    s32 flags;
    while (osRecvMesg(&((View_func_8028FA60_de *)arg0)->messages, &node, 0) != -1) {
        if (node->type == 1 && (node->flags & 0x20) && D_800B6E54 == 0) {
            D_800B6E54 = 2;
            func_0029B9B0(&D_800D6D68, D_800B6E88);
        }
        func_0028AFF0(arg0, node);
    }
    if (((View_func_8028FA60_de *)arg0)->pending != 0 &&
        ((View_func_8028FA60_de *)arg0)->current != 0) {
        func_0028B274(arg0);
        return;
    }
    flags = (((View_func_8028FA60_de *)arg0)->current == 0) * 2 |
            (((View_func_8028FA60_de *)arg0)->next == 0);
    if (((s32 (*)(void *, s32 *, s32 *, s32))func_0028ABE0)(arg0, &value1, &value2, flags) != flags) {
        ((void (*)(void *, s32, s32))func_0028B0FC)(arg0, value1, value2);
    }
}
