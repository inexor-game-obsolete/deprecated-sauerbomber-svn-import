#ifndef _FPSGAME_ITEMS_H
#define _FPSGAME_ITEMS_H

#include "guns.h"
#include "icons.h"
#include "sounds.h"

static struct itemstat {
    int add;
    int max;
    int sound;
    const char *name;
    int icon;
    int info;
} itemstats[] = {
    {10,    30,    S_ITEMAMMO,   "SG", HICON_SG,                  GUN_SG                  },
    {20,    60,    S_ITEMAMMO,   "CG", HICON_CG,                  GUN_CG                  },
    {5,     15,    S_ITEMAMMO,   "RL", HICON_RL,                  GUN_RL                  },
    {5,     15,    S_ITEMAMMO,   "RI", HICON_RIFLE,               GUN_RIFLE               },
    {10,    30,    S_ITEMAMMO,   "GL", HICON_GL,                  GUN_GL                  },
    {30,    120,   S_ITEMAMMO,   "PI", HICON_PISTOL,              GUN_PISTOL              },
    {25,    100,   S_ITEMHEALTH, "H",  HICON_HEALTH                                       },
    {10,    1000,  S_ITEMHEALTH, "MH", HICON_HEALTH                                       },
    {100,   100,   S_ITEMARMOUR, "GA", HICON_GREEN_ARMOUR,        A_GREEN                 },
    {200,   200,   S_ITEMARMOUR, "YA", HICON_YELLOW_ARMOUR,       A_YELLOW                },
    {20000, 30000, S_ITEMPUP,    "Q",  HICON_QUAD                                         },
    {1,     12,    S_ITEMAMMO,   "BO", HICON_BOMB,                GUN_BOMB                },
    {1,     10,    S_ITEMPUP,    "BR", HICON_BOMBRADIUS                                   },
    {1,     7,     S_ITEMPUP,    "BD", HICON_BOMBDELAY                                    },
    {5,     30,    S_ITEMAMMO,   "FG", HICON_FGL,                 GUN_FGL                 },
    {5,     100,   S_ITEMAMMO,   "PT", HICON_PULSED_THRUSTER,     GUN_PULSED_THRUSTER     },
    {20,    1000,  S_ITEMAMMO,   "CT", HICON_CONTINOUS_THRUSTER,  GUN_CONTINOUS_THRUSTER  },
};

enum pickupitems {                      // pickup items
    P_AMMO_SG = 0,
    P_AMMO_CG,
    P_AMMO_RL,
    P_AMMO_RI,
    P_AMMO_GL,
    P_AMMO_PI,
    P_HEALTH_H,
    P_HEALTH_MH,
    P_ARMOUR_GA,
    P_ARMOUR_YA,
    P_UP_Q,
    P_AMMO_BO,
    P_UP_BR,
    P_UP_BD,
    P_AMMO_FG,
    P_AMMO_PT,
    P_AMMO_CT

};

#endif
