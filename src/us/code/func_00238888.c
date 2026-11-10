/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80267198_de_2.c, func_80267CE4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/camera.h"
#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck struct_class): the position is a float struct on the host.
 * MIPS passes the Rage Wars Triple (three s32) and a CVector3 in the same registers; x86-64
 * and arm64 do not (integer vs SSE/FP registers), so Triple means three floats here. */
typedef struct { float x, y, z; } T2HostTriple;
#define Triple T2HostTriple
#endif


#ifndef TARGET_PORT
extern void ImpactEvent_FlashScreens(s32 arg0, s32 arg1, s32 arg2, Triple arg3, SevenBytes arg6);
#endif
extern void CCamera__StartFlash(void *view, s32 red, s32 green, s32 blue, s32 mode, s32 alpha, s32 duration, s32 delay);
extern s32 CCamera__IsPointVisible(Node_func_80267CE4_de *arg0, Triple arg1);
#ifdef TARGET_PORT
/* Host body (round 7 v2): an event's 8-byte value reaches the handler as two host-order
 * words (the archive swaps the animation events' values as two u32: struct IntPair), word 0
 * the N64's first big-endian word; the bytes/halves this handler reads are taken from them. */
typedef struct { u32 w0, w1; } T2EventWords;

void ImpactEvent_FlashScreens(s32 arg0, s32 arg1, s32 arg2, Triple arg3, T2EventWords v) {
    Node_func_80267CE4_de *node;
    node = gEngineApp_m_Game_m_CameraPool.m_ActiveList.m_pHead;
    while (node != 0) {
        if (CCamera__IsPointVisible(node, arg3) != 0) {
            CCamera__StartFlash(node, (u8) (v.w0 >> 24), (u8) (v.w0 >> 16), (u8) (v.w0 >> 8), (u8) v.w0,
                                (u8) (v.w1 >> 24), (u8) (v.w1 >> 16), (u8) (v.w1 >> 8));
        }
        node = node->next;
    }
}
#else
void ImpactEvent_FlashScreens(s32 arg0, s32 arg1, s32 arg2, Triple arg3, SevenBytes arg6) {
    Node_func_80267CE4_de *node;
    node = gEngineApp_m_Game_m_CameraPool.m_ActiveList.m_pHead;
    while (node != 0) {
        if (CCamera__IsPointVisible(node, arg3) != 0) {
            CCamera__StartFlash(node, arg6.a, arg6.b, arg6.c, arg6.d,
                          arg6.e, arg6.f, arg6.g);
        }
        node = node->next;
    }
}
#endif
