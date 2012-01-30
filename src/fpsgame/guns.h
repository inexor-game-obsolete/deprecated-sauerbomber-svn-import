#ifndef _FPSGAME_GUNS_H
#define _FPSGAME_GUNS_H

#include "sounds.h"
#include "particles.h"

enum {
    GUN_FIST = 0,
    GUN_SG,
    GUN_CG,
    GUN_RL,
    GUN_RIFLE,
    GUN_GL,
    GUN_PISTOL,
    GUN_BOMB,
    GUN_FIREBALL,
    GUN_ICEBALL,
    GUN_SLIMEBALL,
    GUN_BITE,
    GUN_BARREL,
    GUN_SPLINTER,
    NUMGUNS
};

static const struct guninfo {
    short sound;
    short attackdelay;
    short damage;
    short projspeed;
    short part;
    short kickamount;
    short range;
    const char *name;
    const char *file;
} guns[NUMGUNS] = {
    { S_PUNCH1,    250,  50, 0,   0,  0,   14, "fist",            "fist"  },
    { S_SG,       1400,  10, 0,   0, 20, 1024, "shotgun",         "shotg" },  // *SGRAYS
    { S_CG,        100,  30, 0,   0,  7, 1024, "chaingun",        "chaing"},
    { S_RLFIRE,    800, 120, 80,  0, 10, 1024, "rocketlauncher",  "rocket"},
    { S_RIFLE,    1500, 100, 0,   0, 30, 2048, "rifle",           "rifle" },
    { S_FLAUNCH,   500,  75, 80,  0, 10, 1024, "grenadelauncher", "gl" },
    { S_PISTOL,    500,  25, 0,   0,  7, 1024, "pistol",          "pistol" },
    { S_FEXPLODE,  375,   1, 8,   0,  2,    0, "bomb",            "cannon" }, // TODO: other sound, other hudmodel
    { S_FLAUNCH,   200,  20, 50,  PART_FIREBALL1,  1, 1024, "fireball",  NULL },
    { S_ICEBALL,   200,  40, 30,  PART_FIREBALL2,  1, 1024, "iceball",   NULL },
    { S_SLIMEBALL, 200,  30, 160, PART_FIREBALL3,  1, 1024, "slimeball", NULL },
    { S_PIGR1,     250,  50, 0,   0,  1,   12, "bite",            NULL },
    { -1,            0, 120, 0,   0,  0,    0, "barrel",          NULL },
    { -1,            0,   1, 200, 0,  0,    0, "splinter",        NULL }
};


#endif
