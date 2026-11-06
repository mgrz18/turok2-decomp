/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MULTI/804453C4_11.CPP, func_80445D6C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"

struct ObjRecAt5D8;
typedef struct ObjRecAt5D8 ObjRecAt5D8;
struct ObjRecAt5D8;
struct ObjRecAt5D8 {
    char pad0[0x5D8];
    void * unk5D8;
};
struct EntryRecAt1C;
struct EntryRecAt1C {
    char pad[0x1C];
    ObjRecAt5D8 *unk1C;
};
struct EntryRecAt1C;
typedef struct EntryRecAt1C EntryRecAt1C;
extern s32 CPlayerPool__GetIndex(void *table, void *player);
s32 NameEntryMenu_DeleteChar_2(s32 unused, EntryRecAt1C *arg1) {
    s32 i;
    s32 count;
    i = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, arg1->unk1C);
    count = gNameEntries[i].length;
    gNameEntries[i].timer = 0;
    if (count == 0) {
        return 0;
    }
    gNameEntries[i].length = count - 1;
    gNameEntries[i].cursor--;
    gNameEntries[i].name[gNameEntries[i].cursor] = 0;
    return 0;
}
