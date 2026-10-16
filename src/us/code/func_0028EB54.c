/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023A294_de_15.c, func_8023CCD4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
struct Node_func_8023CCD4_de;
typedef struct Node_func_8023CCD4_de Node_func_8023CCD4_de;
struct OwnerRecAt8V2;
struct OwnerRecAt8V2 {
    s32 unk0;
    u32 handle;
    s32 queue;
};
struct OwnerRecAt8V2;
typedef struct OwnerRecAt8V2 OwnerRecAt8V2;
extern void func_0028EB54(OwnerRecAt8V2 *arg0);
struct Node_func_8023CCD4_de;
struct Node_func_8023CCD4_de {
    struct Node_func_8023CCD4_de *next;
    u16 id;
    u16 count;
    s32 unk8;
    s32 unkC;
    u8 *slots;
};
struct Slot_func_8023CCD4_de;
typedef struct Slot_func_8023CCD4_de Slot_func_8023CCD4_de;
struct Slot_func_8023CCD4_de;
struct Slot_func_8023CCD4_de {
    s32 unk0;
    u16 unk4;
    u16 unk6;
    u16 id;
    u16 unkA;
    s32 unkC;
};
extern Slot_func_8023CCD4_de D_80120FC0[];
void func_0028EB54(OwnerRecAt8V2 *arg0) {
    extern s32 osSendMesg(s32, Node_func_8023CCD4_de *, s32);
    extern void func_0028DEFC(Slot_func_8023CCD4_de *);
    extern Node_func_8023CCD4_de D_80121378;
    Node_func_8023CCD4_de *node;
    Node_func_8023CCD4_de *prev;
    u8 *slot;
    s32 i;
    u32 key;
    node = &D_80121378;
    key = arg0->handle >> 12;
    prev = 0;
    while (node != 0) {
        if (node->id == key) {
            break;
        }
        prev = node;
        node = node->next;
    }
    prev->next = node->next;
    slot = node->slots;
    for (i = 0; i < node->count; i++) {
        if (*slot != 0xFF) {
            func_0028DEFC(&D_80120FC0[*slot]);
            D_80120FC0[*slot].id = 0xFFFF;
        }
        slot++;
    }
    osSendMesg(arg0->queue, node, 1);
}
