#include "common.h"
#include "structs/game_object.h"

/* The object's intelligence (header.m_pIntelligence): only its flags. */
struct CIntelligence {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 m_dwFlags;
};

extern s32 func_00220260(s32, s32, s32);
extern void func_0026E110(void *, s32, void *);
extern u8 D_800F7078[];

void func_002604E8(CGameObjectInstance *arg0) {
    struct CIntelligence *sub = arg0->header.m_pIntelligence;

    if (!(arg0->m_dwFlags & 0x80000)) {
        if (!(sub->m_dwFlags & 0x20)) {
            func_00220260((s32) D_800F7078, (s32) arg0, 1);
        }
        func_0026E110(arg0, 1, arg0);
    }
}
