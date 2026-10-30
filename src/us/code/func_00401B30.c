#include "common.h"

typedef struct {
    s32 v[3];
} Triple;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 a;
    /* 0x10 */ s32 b;
    /* 0x14 */ Triple c;
} Obj00401B30;

void func_00401B70(Obj00401B30 *obj);

void func_00401B30(Obj00401B30 *obj, s32 a, s32 b, Triple c) {
    obj->a = a;
    obj->b = b;
    obj->c = c;
    func_00401B70(obj);
}
