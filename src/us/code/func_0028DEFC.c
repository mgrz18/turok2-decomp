/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023A294_de_12.c, func_8023C084_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
extern void func_0028EEC0(int arg0);
struct EntryRecAt3;
struct EntryRecAt3 {
    u16 id;
    u8 team;
    u8 slot;
};
struct EntryRecAt3;
typedef struct EntryRecAt3 EntryRecAt3;
struct Mapping;
typedef struct Mapping Mapping;
struct Mapping;
struct Mapping {
    char pad0[8];
    u16 flags;
    char padA;
    u8 slot;
};
extern char D_80132000;
extern EntryRecAt3 D_80121318[];
void func_0028DEFC(Mapping *mapping) {
    extern void func_0028EE40(s32, s32, s32, s32, s32, s32);
    s32 slot;
    if (mapping->slot == 0xFF) {
        return;
    }
    if (mapping->flags & 1) {
        D_80121318[mapping->slot].slot = 0xFF;
        slot = mapping->slot;
        if (D_80121318[slot].team == 0xFF) {
            goto unmap;
        }
        func_0028EE40(slot, 0, D_80121318[slot].id << 12,
                      ((s32)&D_80132000 & 0x3FFFFFF) + (D_80121318[slot].team << 12), -1, 7);
    } else {
        D_80121318[mapping->slot].team = 0xFF;
        slot = mapping->slot;
        if (D_80121318[slot].slot == 0xFF) {
        unmap:
            func_0028EEC0(slot);
            D_80121318[mapping->slot].id = 0xFFFF;
        } else {
            func_0028EE40(slot, 0, D_80121318[slot].id << 12, -1,
                          ((s32)&D_80132000 & 0x3FFFFFF) + (D_80121318[slot].slot << 12), 7);
        }
    }
    mapping->slot = 0xFF;
}
