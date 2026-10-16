/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BB6C0_de_14.c, func_802BB750_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
extern u32 func_0029DFF0(void);
extern void func_0029E010(u32 mask);
struct ThreadNode;
struct ThreadNode {
    struct ThreadNode *next;
    s32 priority;
    struct ThreadNode **queue;
    s32 unk0C;
    u16 state;
};
struct ThreadNode;
typedef struct ThreadNode ThreadNode;
extern void func_0029C8E0();
extern s32 D_800B8918;
extern ThreadNode *D_800B8920;
extern void func_0029C878(ThreadNode **queue, ThreadNode *node);
extern ThreadNode *func_0029C8C8(ThreadNode **queue);
extern void func_0029C6DC(ThreadNode **queue, ThreadNode *node);
void func_0029BB10(ThreadNode *arg0) {
    ThreadNode **sentinel;
    u32 token;
    token = func_0029DFF0();
    if (arg0->state == 1) {
        goto state_one;
    }
    if (arg0->state != 8) {
        goto state_done;
    }
    arg0->state = 2;
    func_0029C878((ThreadNode **)&D_800B8918, arg0);
    goto state_done;
state_one:
    if (arg0->queue == 0) {
        goto insert_arg;
    }
    sentinel = (ThreadNode **)&D_800B8918;
    if (arg0->queue == sentinel) {
insert_arg:
        arg0->state = 2;
        func_0029C878((ThreadNode **)&D_800B8918, arg0);
    } else {
        arg0->state = 8;
        func_0029C878(arg0->queue, arg0);
        func_0029C878(sentinel, func_0029C8C8(arg0->queue));
    }
state_done:
    if (D_800B8920 == 0) {
        func_0029C8E0();
    } else if (D_800B8920->priority < (*(ThreadNode **)&D_800B8918)->priority) {
        D_800B8920->state = 2;
        func_0029C6DC((ThreadNode **)&D_800B8918, D_800B8920);
    }
    func_0029E010(token);
}
