/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00252450.c, func_00252450.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

u32 func_0029DFF0(void);
void func_0029E010(u32 mask);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
extern B12Mgr D_800D7BE0;
extern OSMesgQueue D_800D8DB0;
extern s32 D_800D8DCC;
extern u8 D_800D6D68[];
extern u8 D_800D1CC0[];
void func_002067C4(s32 a, u32 b);
s32 func_00201BDC(void *port, B12Msg *n, void *q);
void CList__Remove(void *l, B12Msg *n);
void CList__AddHead(void *l, B12Msg *n);
void func_002065F8(s32 a, B12Node *n);
void func_002026EC(void *heap, s32 key);
void func_002062F8(s32 a, B12Node *n);
void func_002066B0(s32 a, B12Msg *n);
void func_002044A8(s32 a, B12Msg *n, s32 b);
void func_002041E4(void) {
    B12Msg *msg;
    B12Node *p;
    u32 mask;
    u32 m2;
    while (1) {
        osRecvMesg(&D_800D7BE0.q870, (OSMesg *)&msg, 1);
        if ((u32)msg < 2) {
            mask = func_0029DFF0();
            if (++D_800D8DCC != 1) {
                func_0029E010(mask);
                osRecvMesg(&D_800D7BE0.q11D0, 0, 1);
            } else {
                func_0029E010(mask);
            }
            func_002067C4(0, (u32)msg);
            m2 = func_0029DFF0();
            if (--D_800D8DCC != 0) {
                func_0029E010(m2);
                osSendMesg(&D_800D8DB0, 0, 1);
            } else {
                func_0029E010(m2);
            }
        } else {
            msg->flags &= ~2;
            if (msg->flags & 8) {
                msg->flags &= ~8;
                if (func_00201BDC(D_800D6D68, msg, &D_800D7BE0.q870) != 0) {
                    continue;
                }
                mask = func_0029DFF0();
                if (++D_800D8DCC != 1) {
                    func_0029E010(mask);
                    osRecvMesg(&D_800D7BE0.q11D0, 0, 1);
                } else {
                    func_0029E010(mask);
                }
                CList__Remove(&D_800D7BE0.lists3[2], msg);
                CList__AddHead(&D_800D7BE0.lists3[1], msg);
                p = msg->node;
                p->flags &= ~2;
                while (p->refs != 0) {
                    p->refs--;
                    if (p->refs == 0) {
                        p->flags &= ~0x100;
                    }
                }
                if (!(p->flags & 0x702)) {
                    func_002065F8(0, p);
                    func_002026EC(D_800D1CC0, p->key);
                    func_002062F8(0, p);
                }
                func_002066B0(0, msg);
                m2 = func_0029DFF0();
                if (--D_800D8DCC != 0) {
                    func_0029E010(m2);
                    osSendMesg(&D_800D8DB0, 0, 1);
                } else {
                    func_0029E010(m2);
                }
            } else {
                msg->flags &= ~4;
                mask = func_0029DFF0();
                if (++D_800D8DCC != 1) {
                    func_0029E010(mask);
                    osRecvMesg(&D_800D7BE0.q11D0, 0, 1);
                } else {
                    func_0029E010(mask);
                }
                func_002044A8(0, msg, 1);
                m2 = func_0029DFF0();
                if (--D_800D8DCC != 0) {
                    func_0029E010(m2);
                    osSendMesg(&D_800D8DB0, 0, 1);
                } else {
                    func_0029E010(m2);
                }
            }
        }
    }
}
