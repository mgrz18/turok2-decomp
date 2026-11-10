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

#ifdef TARGET_PORT
#include "structs/vector.h"
#include <string.h>
/* ABI (docs/port/ABI.md, abicheck struct_class): the caller (func_00251FC4) passes the position
 * as a CVector3; MIPS passes it in the same words as this three-s32 view, x86-64 and arm64 in
 * SSE/FP registers. Only copied: the host takes the CVector3 and stores its bytes. */
void Automap_SetPlayer(Obj00401B30 *obj, sptr a, s32 b, CVector3 pos) {
    Triple c;
    memcpy(&c, &pos, sizeof(c));
#else
void Automap_SetPlayer(Obj00401B30 *obj, sptr a, s32 b, Triple c) {
#endif
    obj->a = a;
    obj->b = b;
    obj->c = c;
    Automap_RevealAroundPlayer(obj);
}
