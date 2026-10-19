/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80446F60_de_6.c, func_804480E0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_00299680(void *queue, int channel, u16 address, u8 *buffer);
extern s32 func_00299680(void *queue, int channel, u16 address, u8 *buffer);
extern s32 func_00429DFC(OSPfs *pfs);
s32 func_004292E0(OSPfs *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name, s32 *file_no)
{
    s32 j;
    int i;
    __OSDir dir;
    s32 ret = 0;
    int fail;
    if (func_00429DFC(pfs) == 2)
        return 2;
    for (j = 0; j < pfs->dir_size; j++) {
        ret = func_00299680(pfs->queue, pfs->channel, pfs->dir_table + j, (u8 *)&dir);
        if (ret != 0)
            return ret;
        if ((dir.company_code == company_code) && dir.game_code == game_code) {
            fail = 0;
            if (game_name != 0) {
                for (i = 0; i < 16; i++) {
                    if (dir.game_name[i] != game_name[i]) {
                        fail = 1;
                        break;
                    }
                }
            }
            if (ext_name != 0 && !fail) {
                for (i = 0; i < 4; i++) {
                    if (dir.ext_name[i] != ext_name[i]) {
                        fail = 1;
                        break;
                    }
                }
            }
            if (!fail) {
                *file_no = j;
                return ret;
            }
        }
    }
    *file_no = -1;
    return 5;
}
