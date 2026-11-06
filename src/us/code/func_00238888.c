/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80267198_de_2.c, func_80267CE4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void ImpactEvent_FlashScreens(s32 arg0, s32 arg1, s32 arg2, Triple arg3, SevenBytes arg6);
extern void CCamera__StartFlash(void *view, s32 red, s32 green, s32 blue, s32 mode, s32 alpha, s32 duration, s32 delay);
extern void *gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_pHead;
extern s32 CCamera__IsPointVisible(Node_func_80267CE4_de *arg0, Triple arg1);
void ImpactEvent_FlashScreens(s32 arg0, s32 arg1, s32 arg2, Triple arg3, SevenBytes arg6) {
    Node_func_80267CE4_de *node;
    node = gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_pHead;
    while (node != 0) {
        if (CCamera__IsPointVisible(node, arg3) != 0) {
            CCamera__StartFlash(node, arg6.a, arg6.b, arg6.c, arg6.d,
                          arg6.e, arg6.f, arg6.g);
        }
        node = node->next;
    }
}
