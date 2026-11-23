#ifndef GLOBALS_MENU_TEXT_H
#define GLOBALS_MENU_TEXT_H

/* The menu and controller pak texts: the fixed-width option lines ("difficulty   :     easy"),
 * the mission, note, pak-message and cheat-menu lines, the multiplayer level and texture-set
 * labels, the menus' printf formats. Initialised data after .rodata (0x800B3E70-0x800B66FF) and
 * in the VM tail pool (0x800C0560-0x800C1F70), read by address by both VM modules' menu code.
 * Named in round 21 (versions/names/data_j2.us.txt: the words of the string, the
 * functions that read it). Declared `char []`; a file that needs another view keeps a
 * local declaration with a `local type` comment (docs/GLOBALS.md). */

extern char sOptionsMenuText_DifficultyEasy[];           /* "difficulty   :     easy" */
extern char sOptionsMenuText_DifficultyNormal[];         /* "difficulty   :   normal" */
extern char sOptionsMenuText_DifficultyHard[];           /* "difficulty   :     hard" */
extern char sOptionsMenuText_ControlTypeExpert[];        /* "control type :   expert" */
extern char sOptionsMenuText_ControlTypeArcade[];        /* "control type :   arcade" */
extern char sOptionsMenuText_ControlSideLeft[];          /* "control side :     left" */
extern char sOptionsMenuText_ControlSideRight[];         /* "control side :    right" */
extern char sOptionsMenuText_LookSpringOn[];             /* "look spring  :       on" */
extern char sOptionsMenuText_LookSpringOff[];            /* "look spring  :      off" */
extern char sOptionsMenuText_AutoAimOn[];                /* "auto aim     :       on" */
extern char sOptionsMenuText_AutoAimOff[];               /* "auto aim     :      off" */
extern char sOptionsMenuText_BloodColorOff[];            /* "blood color  :      off" */
extern char sOptionsMenuText_BloodColorRed[];            /* "blood color  :      red" */
extern char sOptionsMenuText_BloodColorGreen[];          /* "blood color  :    green" */
extern char sOptionsMenuText_2PlyrLayoutHorz[];          /* "2 plyr layout:     horz" */
extern char sOptionsMenuText_2PlyrLayoutVert[];          /* "2 plyr layout:     vert" */
extern char sOptionsMenuText_OpacityClear[];             /* "opacity      :    clear" */
extern char sOptionsMenuText_OpacityOpaque[];            /* "opacity      :   opaque" */
extern char sOptionsMenuText_Opacity[];                  /* "opacity      :         " */
extern char sOptionsMenuText_ControlStickHorzNorm[];     /* "control stick horz:norm" */
extern char sOptionsMenuText_ControlStickHorz[];         /* "control stick horz:    " */
extern char sOptionsMenuText_ControlStickVertNorm[];     /* "control stick vert:norm" */
extern char sOptionsMenuText_ControlStickVert[];         /* "control stick vert:    " */
extern char sOptionsMenuText_WeaponSelectQuick[];        /* "Weapon Select:    quick" */
extern char sOptionsMenuText_WeaponSelectNorm[];         /* "Weapon Select:     norm" */
extern char sOptionsMenuText_ResolutionLoRez[];          /* "Resolution   :   lo-rez" */
extern char sOptionsMenuText_ResolutionHiRez[];          /* "Resolution   :   hi-rez" */
extern char sOptionsMenuText_ResolutionHiRezLb[];        /* "Resolution   :hi-rez-lb" */
extern char sOptionsMenuText_ResolutionLoRezLb[];        /* "Resolution   :lo-rez-lb" */
extern char sOptionsMenuText_BrightnessNorm[];           /* "brightness   :     norm" */
extern char sOptionsMenuText_Brightness[];               /* "brightness   :         " */
extern char sPlayerSetupMenuText_Handicap_B41B0[];       /* "handicap :     " */
extern char sPlayerSetupMenuText_Handicap_B41C0[];       /* "handicap :     " */
extern char sPlayerSetupMenuText_Handicap_B41D0[];       /* "handicap :     " */
extern char sPlayerSetupMenuText_Handicap_B41E0[];       /* "handicap :     " */
extern char sPlayerSetupMenuText_HandicapNone[];         /* "handicap : none" */
extern char sPlayerSetupMenuText_HandicapMax[];          /* "handicap :  max" */
extern char sPlayerSetupMenuText_ControlExpert[];        /* "control :expert" */
extern char sPlayerSetupMenuText_ControlArcade[];        /* "control :arcade" */
extern char sFinalScoreMenuText_BlueBloodScore[];        /* "blue blood score :    " */
extern char sFinalScoreMenuText_RedBloodScore[];         /* " red blood score :    " */
extern char sMissionMenuText_MissionStatus[];            /* "mission status" */
extern char sMissionMenuText_MissionFailed[];            /* "mission failed" */
extern char sMissionMenuText_Unavailable[];              /* "unavailable" */
extern char sMissionMenuText_PortOfAdia[];               /* "port of adia" */
extern char sMissionMenuText_ActivatedXOf3[];            /* "activated x of 3" */
extern char sMissionMenuText_DistressBeacons[];          /* "distress beacons" */
extern char sMissionMenuText_RescuedXOf4[];              /* "rescued x of 4" */
extern char sMissionMenuText_Children[];                 /* "children" */
extern char sMissionMenuText_RiverOfSouls[];             /* "river of souls" */
extern char sMissionMenuText_DestroyedXOf2[];            /* "destroyed x of 2" */
extern char sMissionMenuText_SoulGates[];                /* "soul gates" */
extern char sMissionMenuText_DestroyedXOf3_B441C[];      /* "destroyed x of 3" */
extern char sMissionMenuText_SistersOfDespair[];         /* "sisters of despair" */
extern char sMissionMenuText_DeathMarshes[];             /* "death marshes" */
extern char sMissionMenuText_RescuedXOf5[];              /* "rescued x of 5" */
extern char sMissionMenuText_Prisoners[];                /* "prisoners" */
extern char sMissionMenuText_DestroyedXOf3_B4470[];      /* "destroyed x of 3" */
extern char sMissionMenuText_AmmoStorageFacilities[];    /* "ammo storage facilities" */
extern char sMissionMenuText_LairOfTheBlindOnes[];       /* "lair of the blind ones" */
extern char sMissionMenuText_SealedXOf3[];               /* "sealed x of 3" */
extern char sMissionMenuText_ThermalVents[];             /* "thermal vents" */
extern char sMissionMenuText_HiveOfTheMantids[];         /* "hive of the mantids" */
extern char sMissionMenuText_DestroyedXOf3_B44E8[];      /* "destroyed x of 3" */
extern char sMissionMenuText_QueenEmbryos[];             /* "queen embryos" */
extern char sMissionMenuText_DestroyedXOf1[];            /* "destroyed x of 1" */
extern char sMissionMenuText_MasterComputers[];          /* "master computers" */
extern char sMissionMenuText_PrimagensLightship[];       /* "primagen's lightship" */
extern char sMissionMenuText_RecalibratedXOf4[];         /* "recalibrated x of 4" */
extern char sMissionMenuText_PowerGenerators[];          /* "power generators" */
extern char sMissionMenuText_DestroyedXOf3_B4574[];      /* "destroyed x of 3" */
extern char sMissionMenuText_DestroyedXOf[];             /* "destroyed x of ?" */
extern char sMissionMenuText_AssemblyPlants[];           /* "assembly plants" */
extern char sPakMenuText_ControllerPakX_B45E0[];         /* "controller pak: #x" */
extern char sPakMenuText_SaveCharacter[];                /* "save character" */
extern char sPakMenuText_SaveGame[];                     /* "save game" */
extern char sPakMenuText_LoadNote_B4870[];               /* "load note" */
extern char sPakMenuText_SaveNote_B487C[];               /* "save note" */
extern char sPakMenuText_LoadNote_B4888[];               /* "load note" */
extern char sPakMenuText_SaveNote_B4894[];               /* "save note" */
extern char sPakMenuText_DeleteNote[];                   /* "delete note" */
extern char sPakMenuText_FreePagesXxxNeededXxx_B48B4[];  /* "free pages :xxx - needed :xxx" */
extern char sPakMenuText_OtherGame[];                    /* "other game" */
extern char sPakMenuText_Empty[];                        /* "" */
extern char sPakMenuText_ControllerPakMenu[];            /* "controller pak menu" */
extern char sPakMenuText_ControllerPakX_B48F8[];         /* "controller pak :x" */
extern char sPakMenuText_Exit[];                         /* "exit" */
extern char sPakMenuText_FreePagesXxxNeededXxx_B4914[];  /* "free pages :xxx - needed :xxx" */
extern char sCharNoteMenuText_Pathetic[];                /* "   pathetic" */
extern char sCharNoteMenuText_Poor[];                    /* "       poor" */
extern char sCharNoteMenuText_Harmless[];                /* "   harmless" */
extern char sCharNoteMenuText_Nuisance[];                /* "   nuisance" */
extern char sCharNoteMenuText_Acceptable[];              /* " acceptable" */
extern char sCharNoteMenuText_Average[];                 /* "    average" */
extern char sCharNoteMenuText_Good[];                    /* "       good" */
extern char sCharNoteMenuText_Excellent[];               /* "  excellent" */
extern char sCharNoteMenuText_Dangerous[];               /* "  dangerous" */
extern char sCharNoteMenuText_Brutal[];                  /* "     brutal" */
extern char sCharNoteMenuText_Savage[];                  /* "     savage" */
extern char sCharNoteMenuText_Elite[];                   /* "      elite" */
extern char sGameNoteMenuText_NoteInfo[];                /* "note info" */
extern char sGameNoteMenuText_DeleteNote[];              /* "delete note" */
extern char sGameNoteMenuText_DifficultyEasy[];          /* "difficulty :  easy" */
extern char sGameNoteMenuText_DifficultyNormal[];        /* "difficulty :normal" */
extern char sGameNoteMenuText_DifficultyHard[];          /* "difficulty :  hard" */
extern char sGameNoteMenuText_PortOfAdia[];              /* "port of adia" */
extern char sGameNoteMenuText_RiverOfSouls[];            /* "river of souls" */
extern char sGameNoteMenuText_DeathMarshes[];            /* "death marshes" */
extern char sGameNoteMenuText_LairOfTheBlindOnes[];      /* "lair of the blind ones" */
extern char sGameNoteMenuText_HiveOfTheMantids[];        /* "hive of the mantids" */
extern char sGameNoteMenuText_PrimagensLightship[];      /* "primagen's lightship" */
extern char sGameNoteMenuText_TheHub[];                  /* "the hub" */
extern char sGameNoteMenuText_UnknownLevelId[];          /* "unknown level id" */
extern char sCharNoteMenuText_NoteInfo[];                /* "note info" */
extern char sCharNoteMenuText_DeleteNote_B4AEC[];        /* "delete note" */
extern char sCharNoteMenuText_DeleteNote_B4AF8[];        /* "delete note" */
extern char sCharNoteMenuText_NameExt[];                 /* "name             ext " */
extern char sCharNoteMenuText_Empty[];                   /* "                     " */
extern char sCharNoteMenuText_Name[];                    /* "name  :            " */
extern char sCharNoteMenuText_Type[];                    /* "type  :            " */
extern char sCharNoteMenuText_Load[];                    /* "load?" */
extern char sCharNoteMenuText_Delete_B4B8C[];            /* "delete?" */
extern char sCharNoteMenuText_Delete_B4B94[];            /* "delete?" */
extern char sPakMessageText_Error[];                     /* "error" */
extern char sPakMessageText_Important[];                 /* "important" */
extern char sPakMessageText_Ok[];                        /* "ok" */
extern char sPakMessageText_Empty_B4BB4[];               /* "" */
extern char sPakMessageText_ControllerPakIsFull[];       /* "controller pak is full!" */
extern char sPakMessageText_DeleteANoteToFree[];         /* "delete a note to free" */
extern char sPakMessageText_UpSpace[];                   /* "up space." */
extern char sPakMessageText_Empty_B4BF4[];               /* "" */
extern char sPakMessageText_Empty_B4BF8[];               /* "" */
extern char sPakMessageText_ControllerPakNot_B4BFC[];    /* "controller pak not" */
extern char sPakMessageText_Found[];                     /* "found!" */
extern char sPakMessageText_PleaseInsertA[];             /* "please insert a" */
extern char sPakMessageText_ControllerPakAnd[];          /* "controller pak and" */
extern char sPakMessageText_TryAgain[];                  /* "try again." */
extern char sPakMessageText_ThisGameIsController_B4C48[]; /* "this game is controller" */
extern char sPakMessageText_PakCompatiblePlease_B4C60[]; /* "pak compatible. please" */
extern char sPakMessageText_InsertYourController_B4C78[]; /* "insert your controller" */
extern char sPakMessageText_PakNowIfYouWishTo_B4C90[];   /* "pak now if you wish to" */
extern char sPakMessageText_ManageIt[];                  /* "manage it." */
extern char sPakMessageText_ThisGameIsController_B4CB4[]; /* "this game is controller" */
extern char sPakMessageText_PakCompatiblePlease_B4CCC[]; /* "pak compatible. please" */
extern char sPakMessageText_InsertYourController_B4CE4[]; /* "insert your controller" */
extern char sPakMessageText_PakNowIfYouWishTo_B4CFC[];   /* "pak now if you wish to" */
extern char sPakMessageText_Load[];                      /* "load." */
extern char sPakMessageText_ThisGameIsController_B4D1C[]; /* "this game is controller" */
extern char sPakMessageText_PakCompatiblePlease_B4D34[]; /* "pak compatible. please" */
extern char sPakMessageText_InsertYourController_B4D4C[]; /* "insert your controller" */
extern char sPakMessageText_PakNowIfYouWishTo_B4D64[];   /* "pak now if you wish to" */
extern char sPakMessageText_Save[];                      /* "save." */
extern char sPakMessageText_ThisGameIsRumblePak_B4D84[]; /* "this game is rumble pak" */
extern char sPakMessageText_CompatiblePlease_B4D9C[];    /* "compatible. please" */
extern char sPakMessageText_InsertYourRumblePak_B4DB0[]; /* "insert your rumble pak" */
extern char sPakMessageText_NowIfYouWishToPlay_B4DC8[];  /* "now if you wish to play" */
extern char sPakMessageText_WithIt_B4DE0[];              /* "with it." */
extern char sPakMessageText_ControllerPakNot_B4DEC[];    /* "controller pak not" */
extern char sPakMessageText_FoundYouWillNot[];           /* "found! you will not" */
extern char sPakMessageText_BeAbleToSaveYour[];          /* "be able to save your" */
extern char sPakMessageText_Game[];                      /* "game." */
extern char sPakMessageText_Empty_B4E34[];               /* "" */
extern char sPakMessageText_ControllerPak[];             /* "controller pak" */
extern char sPakMessageText_WasChanged[];                /* "was changed!" */
extern char sPakMessageText_PleaseTry[];                 /* "please try" */
extern char sPakMessageText_Again[];                     /* "again." */
extern char sPakMessageText_Empty_B4E6C[];               /* "" */
extern char sPakMessageText_Empty_B4E70[];               /* "" */
extern char sPakMessageText_Empty_B4E74[];               /* "" */
extern char sPakMessageText_NoController[];              /* "no controller" */
extern char sPakMessageText_Empty_B4E88[];               /* "" */
extern char sPakMessageText_Empty_B4E8C[];               /* "" */
extern char sPakMessageText_ThisGameIsRumblePak_B4E90[]; /* "this game is rumble pak" */
extern char sPakMessageText_CompatiblePlease_B4EA8[];    /* "compatible. please" */
extern char sPakMessageText_InsertYourRumblePak_B4EBC[]; /* "insert your rumble pak" */
extern char sPakMessageText_NowIfYouWishToPlay_B4ED4[];  /* "now if you wish to play" */
extern char sPakMessageText_WithIt_B4EEC[];              /* "with it." */
extern char sPakMessageText_SaveWas[];                   /* "save was" */
extern char sPakMessageText_Successful[];                /* "successful!" */
extern char sPakMessageText_Empty_B4F10[];               /* "" */
extern char sPakMessageText_Empty_B4F14[];               /* "" */
extern char sPakMessageText_Empty_B4F18[];               /* "" */
extern char sPakMessageText_ThisNoteIsCorrupt[];         /* "this note is corrupt" */
extern char sPakMessageText_AndCannotBeUsed[];           /* "and cannot be used." */
extern char sPakMessageText_YouMayDeleteThis[];          /* "you may delete this" */
extern char sPakMessageText_Note[];                      /* "note." */
extern char sPakMessageText_Empty_B4F64[];               /* "" */
extern char sPakMessageText_NoteWas[];                   /* "note was" */
extern char sPakMessageText_Successfully[];              /* "successfully" */
extern char sPakMessageText_Deleted[];                   /* "deleted." */
extern char sPakMessageText_Empty_B4F90[];               /* "" */
extern char sPakMessageText_Empty_B4F94[];               /* "" */
extern char sPakMessageText_ProblemOccurred_B4F98[];     /* "problem occurred" */
extern char sPakMessageText_WhileTryingTo_B4FAC[];       /* "while trying to" */
extern char sPakMessageText_SaveANote[];                 /* "save a note." */
extern char sPakMessageText_Empty_B4FCC[];               /* "" */
extern char sPakMessageText_Empty_B4FD0[];               /* "" */
extern char sPakMessageText_ProblemOccurred_B4FD4[];     /* "problem occurred" */
extern char sPakMessageText_WhileTryingTo_B4FE8[];       /* "while trying to" */
extern char sPakMessageText_DeleteANote[];               /* "delete a note." */
extern char sPakMessageText_Empty_B5008[];               /* "" */
extern char sPakMessageText_Empty_B500C[];               /* "" */
extern char sPakMessageText_ControllerPakWas[];          /* "controller pak was" */
extern char sPakMessageText_SuccessfullyRepaired[];      /* "successfully repaired." */
extern char sPakMessageText_PleaseRememberThat[];        /* "please remember that" */
extern char sPakMessageText_SavedNotesMayHave[];         /* "saved notes may have" */
extern char sPakMessageText_BeenDeleted[];               /* "been deleted." */
extern char sDamagedPakMenuText_ControllerPakIs[];       /* "controller pak is" */
extern char sDamagedPakMenuText_DamagedGameCannot[];     /* "damaged, game cannot" */
extern char sDamagedPakMenuText_BeSaved[];               /* "be saved." */
extern char sDamagedPakMenuText_DamagedController[];     /* "damaged controller" */
extern char sDamagedPakMenuText_PakFoundIn[];            /* "pak found in" */
extern char sDamagedPakMenuText_ControllerX[];           /* "controller #x." */
extern char sDamagedPakMenuText_Continue[];              /* "continue" */
extern char sDamagedPakMenuText_ContinueWithoutSaving[]; /* "continue without saving" */
extern char sPakConfirmMenuText_AttemptRepair[];         /* "attempt repair" */
extern char sPakConfirmMenuText_PreviouslySaved[];       /* "previously saved" */
extern char sPakConfirmMenuText_DataMayBeLost[];         /* "data may be lost." */
extern char sPakConfirmMenuText_WouldYouStillLike[];     /* "would you still like" */
extern char sPakConfirmMenuText_ToAttemptRepair[];       /* "to attempt repair?" */
extern char sPakConfirmMenuText_Error_B5194[];           /* "error" */
extern char sPakConfirmMenuText_NotEnoughNotesOr_B519C[]; /* "not enough notes or" */
extern char sPakConfirmMenuText_PagesAvailableTo_B51B0[]; /* "pages available to" */
extern char sPakConfirmMenuText_Save[];                  /* "save." */
extern char sPakConfirmMenuText_Empty_B51CC[];           /* "" */
extern char sPakConfirmMenuText_Error_B51D0[];           /* "error" */
extern char sPakConfirmMenuText_NotEnoughNotesOr_B51D8[]; /* "not enough notes or" */
extern char sPakConfirmMenuText_PagesAvailableTo_B51EC[]; /* "pages available to" */
extern char sPakConfirmMenuText_SaveAllNoteTypes[];      /* "save all note types." */
extern char sPakConfirmMenuText_Empty_B5218[];           /* "" */
extern char sPakConfirmMenuText_Overwrite[];             /* "overwrite" */
extern char sPakConfirmMenuText_YouWillBeDeleting[];     /* "you will be deleting" */
extern char sPakConfirmMenuText_TheOldNote[];            /* "the old note." */
extern char sPakConfirmMenuText_WouldYouLikeTo_B5250[];  /* "would you like to" */
extern char sPakConfirmMenuText_OverwriteThisNote[];     /* "overwrite this note?" */
extern char sPakConfirmMenuText_Error_B527C[];           /* "error" */
extern char sPakConfirmMenuText_ThisNoteIsCorrupt[];     /* "this note is corrupt" */
extern char sPakConfirmMenuText_AndCannotBeUsed[];       /* "and cannot be used." */
extern char sPakConfirmMenuText_WouldYouLikeTo_B52B0[];  /* "would you like to" */
extern char sPakConfirmMenuText_DeleteThisNote[];        /* "delete this note?" */
extern char sPakConfirmMenuText_ManageTheControllerPak[]; /* "manage the controller pak" */
extern char sPakConfirmMenuText_ContinueWithoutSaving[]; /* "continue without saving" */
extern char sPakConfirmMenuText_Yes[];                   /* "yes" */
extern char sPakConfirmMenuText_No[];                    /* "no" */
extern char sCharNoteMenuText_Turok[];                   /* "      Turok" */
extern char sCharNoteMenuText_Adon[];                    /* "       Adon" */
extern char sCharNoteMenuText_Gant[];                    /* "       Gant" */
extern char sCharNoteMenuText_FleshEater[];              /* "Flesh Eater" */
extern char sCharNoteMenuText_PurrLin[];                 /* "   Purr Lin" */
extern char sCharNoteMenuText_EndTrail[];                /* "  End Trail" */
extern char sCharNoteMenuText_Fireborn[];                /* "   Fireborn" */
extern char sCharNoteMenuText_Iggy[];                    /* "       Iggy" */
extern char sCharNoteMenuText_Campaigner[];              /* " Campaigner" */
extern char sCharNoteMenuText_Telset[];                  /* "     Telset" */
extern char sCheatMenuText_InvincibilityOn[];            /* "invincibility         :  on" */
extern char sCheatMenuText_InvincibilityOff[];           /* "invincibility         : off" */
extern char sCheatMenuText_InfiniteLivesOn[];            /* "infinite lives        :  on" */
extern char sCheatMenuText_InfiniteLivesOff[];           /* "infinite lives        : off" */
extern char sCheatMenuText_AllWeaponsOn[];               /* "all weapons           :  on" */
extern char sCheatMenuText_AllWeaponsOff[];              /* "all weapons           : off" */
extern char sCheatMenuText_UnlimitedAmmoOn[];            /* "unlimited ammo        :  on" */
extern char sCheatMenuText_UnlimitedAmmoOff[];           /* "unlimited ammo        : off" */
extern char sCheatMenuText_AllSpecialObjectsOn[];        /* "all special objects   :  on" */
extern char sCheatMenuText_AllSpecialObjectsOff[];       /* "all special objects   : off" */
extern char sCheatMenuText_BigHeadModeOn[];              /* "big head mode         :  on" */
extern char sCheatMenuText_BigHeadModeOff[];             /* "big head mode         : off" */
extern char sCheatMenuText_TinyModeOn[];                 /* "tiny mode             :  on" */
extern char sCheatMenuText_TinyModeOff[];                /* "tiny mode             : off" */
extern char sCheatMenuText_StickModeOn[];                /* "stick mode            :  on" */
extern char sCheatMenuText_StickModeOff[];               /* "stick mode            : off" */
extern char sCheatMenuText_BigHandsFeetModeOn[];         /* "big hands / feet mode :  on" */
extern char sCheatMenuText_BigHandsFeetModeOff[];        /* "big hands / feet mode : off" */
extern char sCheatMenuText_PenAndInkModeOn[];            /* "pen and ink mode      :  on" */
extern char sCheatMenuText_PenAndInkModeOff[];           /* "pen and ink mode      : off" */
extern char sCheatMenuText_GouraudModeOn[];              /* "gouraud mode          :  on" */
extern char sCheatMenuText_GouraudModeOff[];             /* "gouraud mode          : off" */
extern char sCheatMenuText_JuansCheatOn[];               /* "juans cheat           :  on" */
extern char sCheatMenuText_JuansCheatOff[];              /* "juans cheat           : off" */
extern char sCheatMenuText_ZachAttackCheatOn[];          /* "zach attack cheat     :  on" */
extern char sCheatMenuText_ZachAttackCheatOff[];         /* "zach attack cheat     : off" */
extern char sCheatMenuText_AllMapOn[];                   /* "all map               :  on" */
extern char sCheatMenuText_AllMapOff[];                  /* "all map               : off" */
extern char sCheatMenuText_BlackoutOn[];                 /* "blackout              :  on" */
extern char sCheatMenuText_BlackoutOff[];                /* "blackout              : off" */
extern char sCheatMenuText_FrootyStripesOn[];            /* "frooty stripes        :  on" */
extern char sCheatMenuText_FrootyStripesOff[];           /* "frooty stripes        : off" */
extern char sCheatMenuText_WarpPortOfAdia[];             /* "warp:          port of adia" */
extern char sCheatMenuText_WarpRiverOfSouls[];           /* "warp:        river of souls" */
extern char sCheatMenuText_WarpDeathMarshes[];           /* "warp:         death marshes" */
extern char sCheatMenuText_WarpLairOfTheBlindOnes[];     /* "warp:lair of the blind ones" */
extern char sCheatMenuText_WarpHiveOfTheMantids[];       /* "warp:   hive of the mantids" */
extern char sCheatMenuText_WarpPrimagensLightship[];     /* "warp:  primagen's lightship" */
extern char sCheatMenuText_WarpQueenBoss[];              /* "warp:            queen boss" */
extern char sCheatMenuText_WarpBlindOneBoss[];           /* "warp:        blind one boss" */
extern char sCheatMenuText_WarpMotherBoss[];             /* "warp:           mother boss" */
extern char sCheatMenuText_WarpPrimagenBoss[];           /* "warp:         primagen boss" */
extern char sCheatMenuText_WarpCredits[];                /* "warp:               credits" */
extern char sStatsMenuText_YouFragged[];                 /* "you fragged" */
extern char sStatsMenuText_YouTagged[];                  /* "you tagged" */
extern char sPlayerNameText_Player1[];                   /* "player 1" */
extern char sPlayerNameText_Player2[];                   /* "player 2" */
extern char sPlayerNameText_Player3[];                   /* "player 3" */
extern char sPlayerNameText_Player4[];                   /* "player 4" */
extern char sBloodLustMenuText_LevelRandom[];            /* "level:          random" */
extern char sBloodLustMenuText_LevelExperimentata[];     /* "level:   experimentata" */
extern char sBloodLustMenuText_LevelMoonlitMayhem[];     /* "level:  moonlit mayhem" */
extern char sBloodLustMenuText_LevelCloseQuartered[];    /* "level: close quartered" */
extern char sBloodLustMenuText_LevelVulcansForge[];      /* "level:  vulcan's forge" */
extern char sBloodLustMenuText_LevelTeleportastic[];     /* "level:   teleportastic" */
extern char sBloodLustMenuText_LevelMirrororrim[];       /* "level:     mirrororrim" */
extern char sBloodLustMenuText_LevelFishInABarrel[];     /* "level:fish in a barrel" */
extern char sBloodLustMenuText_LevelH2Whoa[];            /* "level:       h 2 whoa!" */
extern char sBloodLustMenuText_LevelEscheria[];          /* "level:        escheria" */
extern char sBloodLustMenuText_LevelMoshCrypt[];         /* "level:      mosh crypt" */
extern char sBloodLustMenuText_LevelSquaresCubed[];      /* "level:   squares cubed" */
extern char sBloodLustMenuText_LevelCrazyEight[];        /* "level:     crazy eight" */
extern char sBloodLustMenuText_LevelBullseye[];          /* "level:        bullseye" */
extern char sBloodLustMenuText_TextureSetCastle[];       /* "texture set:    castle" */
extern char sBloodLustMenuText_TextureSetSteelMill[];    /* "texture set:steel mill" */
extern char sBloodLustMenuText_TextureSetOakFort[];      /* "texture set:  oak fort" */
extern char sBloodLustMenuText_TextureSetCatacombs[];    /* "texture set: catacombs" */
extern char sBloodLustMenuText_TextureSetJailhouse[];    /* "texture set: jailhouse" */
extern char sBloodLustMenuText_TextureSetBigCity[];      /* "texture set:  big city" */
extern char sBloodLustMenuText_TextureSetRedDragon[];    /* "texture set:red dragon" */
extern char sBloodLustMenuText_TextureSetPaintball[];    /* "texture set: paintball" */
extern char sBloodLustMenuText_TextureSetMrHappy[];      /* "texture set:  mr happy" */
extern char sBloodLustMenuText_CrosshairsOn[];           /* "crosshairs       :  on" */
extern char sBloodLustMenuText_CrosshairsOff[];          /* "crosshairs       : off" */
extern char sFragTagMenuText_LevelMadhouse[];            /* "level:       madhouse!" */
extern char sFragTagMenuText_LevelSnap[];                /* "level:            snap" */
extern char sFragTagMenuText_LevelSneaksNLadders[];      /* "level:sneaks n ladders" */
extern char sFragTagMenuText_LevelPrimattica[];          /* "level:      primattica" */
extern char sFragTagMenuText_LevelCrossfire[];           /* "level:       crossfire" */
extern char sFragTagMenuText_LevelYNGWTSF[];             /* "level:  y.n.g.w.t.s.f." */
extern char sWeaponsMenuText_ChargeDartRifleOn[];        /* "charge dart rifle : on" */
extern char sWeaponsMenuText_ChargeDartRifleOff[];       /* "charge dart rifle :off" */
extern char sWeaponsMenuText_FirestormOn[];              /* "firestorm         : on" */
extern char sWeaponsMenuText_FirestormOff[];             /* "firestorm         :off" */
extern char sWeaponsMenuText_AssaultRifleOn[];           /* "assault rifle     : on" */
extern char sWeaponsMenuText_AssaultRifleOff[];          /* "assault rifle     :off" */
extern char sWeaponsMenuText_PlasmaRifleOn[];            /* "plasma rifle      : on" */
extern char sWeaponsMenuText_PlasmaRifleOff[];           /* "plasma rifle      :off" */
extern char sWeaponsMenuText_CerebralBoreOn[];           /* "cerebral bore     : on" */
extern char sWeaponsMenuText_CerebralBoreOff[];          /* "cerebral bore     :off" */
extern char sWeaponsMenuText_GrenadeLauncherOn[];        /* "grenade launcher  : on" */
extern char sWeaponsMenuText_GrenadeLauncherOff[];       /* "grenade launcher  :off" */
extern char sWeaponsMenuText_ScorpionLauncherOn[];       /* "scorpion launcher : on" */
extern char sWeaponsMenuText_ScorpionLauncherOff[];      /* "scorpion launcher :off" */
extern char sWeaponsMenuText_TorpedoLauncherOn[];        /* "torpedo launcher  : on" */
extern char sWeaponsMenuText_TorpedoLauncherOff[];       /* "torpedo launcher  :off" */
extern char sSoundMenuText_MusicVolumeOff[];             /* "music volume  :   off" */
extern char sSoundMenuText_MusicVolumeMax[];             /* "music volume  :   max" */
extern char sSoundMenuText_MusicVolumeNorm[];            /* "music volume  :  norm" */
extern char sSoundMenuText_MusicVolume[];                /* "music volume  :      " */
extern char sSoundMenuText_SfxVolumeOff[];               /* "sfx volume    :   off" */
extern char sSoundMenuText_SfxVolumeMax[];               /* "sfx volume    :   max" */
extern char sSoundMenuText_SfxVolumeNorm[];              /* "sfx volume    :  norm" */
extern char sSoundMenuText_SfxVolume[];                  /* "sfx volume    :      " */
extern char sSoundMenuText_SpeechVolumeOff[];            /* "speech volume :   off" */
extern char sSoundMenuText_SpeechVolumeMax[];            /* "speech volume :   max" */
extern char sSoundMenuText_SpeechVolumeNorm[];           /* "speech volume :  norm" */
extern char sSoundMenuText_SpeechVolume[];               /* "speech volume :      " */
extern char sSoundMenuText_MusicNone[];                  /* "music         :  none" */
extern char sSoundMenuText_Music[];                      /* "music         :      " */
extern char sMpMenuText_PlayerSpeed[];                   /* "player speed :        " */
extern char sMpMenuText_PlayerSpeedDefault[];            /* "player speed : default" */
extern char sTotemDefenseText_Enemies[];                 /* "Enemies" */
extern char sTotemDefenseText_Time[];                    /* "Time" */
extern char sFinalScoreMenuFmt_04d[];                    /* "%04d" */
extern char sLimitLabelFmt_02d[];                        /* "  %02d" */
extern char sLimitLabelText_None[];                      /* "none" */
extern char sMpMenuFmt_3d[];                             /* "%3d" */
extern char sSoundMenuFmt_2d[];                          /* "  %2d" */
extern char sSoundMenuFmt_3d[];                          /* " %3d" */
extern char sOptionsMenuFmt_02d[];                       /* "  %02d" */
extern char sOptionsMenuFmt_03d[];                       /* " %03d" */
extern char sStatsMenuFmt_04d[];                         /* "%04d" */
extern char sStatsMenuFmt_02d[];                         /* "%02d" */
extern char sPakMenuText_SingleSaveBlock[];              /* "Single Save Block" */
extern char sPakMenuFmt_3d[];                            /* "%3d" */
extern char sPakMenuText_Spaces3[];                      /* "   " */
extern char sPakMenuFmt_1d[];                            /* "%1d" */
extern char sGfxPoolText_AudioHeap[];                    /* "audio heap" */
extern char sPakText_MempakReadTempData[];               /* "Mempak read temp data" */
extern char sDebugMenuText_DebugOff[];                   /* "debug         :   off" */
extern char sDebugMenuText_DebugOn[];                    /* "debug         :    on" */
extern char sDebugMenuText_DebugVis[];                   /* "debug         :   vis" */
extern char sDebugMenuText_DebugAis[];                   /* "debug         :   ais" */
extern char sDebugMenuText_DebugSnd[];                   /* "debug         :   snd" */
extern char sDebugMenuText_DebugLinks[];                 /* "debug         : links" */

#endif
