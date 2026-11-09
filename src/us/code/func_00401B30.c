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

void Automap_RevealAroundPlayer(Obj00401B30 *obj);

void Automap_SetPlayer(Obj00401B30 *obj, sptr a, s32 b, Triple c) {
    obj->a = a;
    obj->b = b;
    obj->c = c;
    Automap_RevealAroundPlayer(obj);
}
