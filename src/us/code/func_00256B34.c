#include "common.h"
#include "structs/weapon.h"

/* Whether weapon arg1 fires projectile arg2 (its ammo list stops at the first NULL). */
s32 Weapon_FiresProjectile(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    WeaponInfo *p = D_800AF618[arg1];
    for (i = 0; i < 3; i++) {
        AmmoInfo *e = p->ammo[i];
        if (e != NULL) {
            if (e->projectile == arg2) {
                return 1;
            }
        } else {
            return 0;
        }
    }
    return 0;
}
