#ifndef GLOBALS_PICKUP_H
#define GLOBALS_PICKUP_H

/* The pickup tables (CPlayer__CollectPickup, Pickup_Find*Entry) and the AI drop tables. */

#include "common.h"
#include "structs/pickup.h"

extern CPickupRow14 sParticleAmmoPickups[4];    /* what a particle gives when it touches the player */
extern CPickupRow18 sWeaponPickups[31];         /* handler Pickup_GiveWeapon */
extern CPickupRow14 sAmmoPickups[36];           /* handler Pickup_GiveAmmo */
#ifdef TARGET_PORT
extern CPickupRow18Health sHealthPickups[7];    /* the host: typed for gendata (structs/pickup.h) */
#else
extern CPickupRow18 sHealthPickups[7];          /* handler Pickup_GiveHealth */
#endif
extern CPickupRow14 sLifeForcePickups[2];       /* handler Pickup_GiveLifeForce */
extern CPickupRow10 sItemPickups[15];           /* handler Pickup_GiveItem */

extern CDropPickup sDropPickups[33];            /* by the first mask (AI_DropPickupsOnce) */
extern CDropPickup sDropPickups2[3];            /* by the second mask */

#endif
