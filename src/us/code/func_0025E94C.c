#include "common.h"
#include "structs/game_object.h"

void func_002605AC();
void func_00246918(CGameObjectInstance *obj, CAI *ai);
void func_0026E19C(struct CGameRegion *region, s32 flags, CGameObjectInstance *obj);

void func_0025E94C(CGameObjectInstance *obj, CAI *ai) {
    ai->m_pfDraw = func_002605AC;
    ai->m_dwFlags |= 0x20;
    obj->m_dwHotPointFlags = 0;
    obj->m_dwFlags &= ~0x01000000;
    obj->m_dwFlags &= ~0x00010000;
    func_00246918(obj, ai);
    func_0026E19C(obj->header.m_pCurrentRegion, 0x80, obj);
}
