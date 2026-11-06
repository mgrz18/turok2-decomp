/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/8024BA6C_6.CPP, func_8024C664_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/game.h"
#include "globals/player.h"
#include "globals/object.h"
struct ObjRecAt18V2;
typedef struct ObjRecAt18V2 ObjRecAt18V2;
struct ObjRecAt18V2;
struct ObjRecAt18V2 {
    char pad0[0x14];
    s32 * unk18; /* at 0x14 in Turok 2 */
};
void *CGameObjectInstance__GetBodySetup(char *arg0)
{
    s32 type;
    s32 i;
    s32 count;
    s32 limit;
    char *object;
    CPlayerPool *list;
    ObjRecAt18V2 *actor = (void *)arg0;
    type = *actor->unk18;
    if ((unsigned int)type >= 13) {
        return 0;
    }
    if (type == 11) {
        list = &gEngineApp_m_Game.m_PlayerList;
        count = list->m_nPlayers;
        i = 0;
        if (count > 0) {
            limit = count;
            object = (char *)list->m_Players;
            do {
                if (object == arg0) {
                    return 0;
                }
                if (object + 0x288 == arg0) {
                    return &gWeaponActorTypeInfo;
                }
                i++;
                object += 0x1328;
            } while (i < limit);
        }
    }
    return gIntelligenceTypeInfo[type];
}
