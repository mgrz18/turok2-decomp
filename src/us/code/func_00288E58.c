/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER/80293A04.CPP, func_80293A20_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/game.h"

/* Turok 2: the mode block sits at 0x23FD0 (Rage Wars 0x26DB0). */

extern s32 D_800F5D2C;
extern s32 Cinema_IsIdle(s32 *flags);
extern s32 SavedPlayers_IsActive(void);

void CEngineApp__UpdateCinemaMode(CEngineApp *session, s32 arg1, s32 target) {
    s32 pending;
    s32 *flags;
    flags = &D_800F5D2C;
    pending = *flags & 0x1000;
    if ((unsigned)(session->m_Settings.mode - 0xB) < 2) {
        pending = 0;
    }
    if (pending != 0 && target != -1) {
        session->m_Settings.fadeMode = 2;
        session->m_Settings.nextMode = target;
        *flags = 0;
        return;
    }
    if (Cinema_IsIdle(flags) != 0) {
        s32 mode = session->m_Settings.mode;
        session->m_Settings.fadeMode = 1;
        session->m_Settings.fade = 0;
        session->m_Settings.mode = 0x11;
        session->m_Settings.nextMode = arg1;
        session->m_Settings.prevMode = mode;
        if (SavedPlayers_IsActive() != 0) {
            session->m_Settings.fade = (1.0f);
        }
    }
}
