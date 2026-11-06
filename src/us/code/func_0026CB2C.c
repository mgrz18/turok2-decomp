#include "common.h"
#include "rw/abu_protos.h"

void CVector3__Add(CVector3 *, CVector3 *, CVector3 *);
void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);

extern f32 D_800A8010;

typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ CVector3 *a;
    /* 0x8 */ CVector3 *b;
    /* 0xC */ CVector3 *c;
} Triangle0026CB2C;

void CGameRegion__GetCentroid(Triangle0026CB2C *arg0, CVector3 *arg1) {
    CVector3 sp10;
    CVector3 sp20;
    CVector3 sp30;
    f32 temp_f20;

    CVector3__Add(&sp10, arg0->a, arg0->b);
    temp_f20 = D_800A8010;
    CVector3__Scale(&sp10, &sp10, temp_f20);
    CVector3__Add(&sp20, arg0->b, arg0->c);
    CVector3__Scale(&sp20, &sp20, temp_f20);
    CVector3__Add(&sp30, arg0->c, arg0->a);
    CVector3__Scale(&sp30, &sp30, temp_f20);
    CVector3__Add(arg1, &sp10, &sp20);
    CVector3__Add(arg1, arg1, &sp30);
    CVector3__Scale(arg1, arg1, 0.33333334f);
}
