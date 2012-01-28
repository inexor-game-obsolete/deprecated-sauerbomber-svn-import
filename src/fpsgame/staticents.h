#ifndef _FPSGAME_STATICENTS_H
#define _FPSGAME_STATICENTS_H

#include "ents.h"

enum static_ents                // static entity types
{
    NOTUSED = ET_EMPTY,         // entity slot not in use in map
    LIGHT = ET_LIGHT,           // lightsource, attr1 = radius, attr2 = intensity
    MAPMODEL = ET_MAPMODEL,     // attr1 = angle, attr2 = idx
    PLAYERSTART,                // attr1 = angle, attr2 = team
    ENVMAP = ET_ENVMAP,         // attr1 = radius
    PARTICLES = ET_PARTICLES,
    MAPSOUND = ET_SOUND,
    SPOTLIGHT = ET_SPOTLIGHT,
    I_SHELLS, I_BULLETS, I_ROCKETS, I_ROUNDS, I_GRENADES, I_CARTRIDGES,
    I_HEALTH, I_BOOST,
    I_GREENARMOUR, I_YELLOWARMOUR,
    I_QUAD,
    TELEPORT,                   // attr1 = idx, attr2 = model, attr3 = tag
    TELEDEST,                   // attr1 = angle, attr2 = idx
    MONSTER,                    // attr1 = angle, attr2 = monstertype
    CARROT,                     // attr1 = tag, attr2 = type
    JUMPPAD,                    // attr1 = zpush, attr2 = ypush, attr3 = xpush
    BASE,
    RESPAWNPOINT,
    MOV_BOX,                        // attr1 = angle, attr2 = idx, attr3 = weight
    MOV_BARREL,                     // attr1 = angle, attr2 = idx, attr3 = weight, attr4 = health
    MOV_PLATFORM,                   // attr1 = angle, attr2 = idx, attr3 = tag, attr4 = speed
    MOV_ELEVATOR,                   // attr1 = angle, attr2 = idx, attr3 = tag, attr4 = speed
    FLAG,                       // attr1 = angle, attr2 = team
    I_BOMBS,
    I_BOMBRADIUS,
    I_BOMBDELAY,
    I_BOMBRESERVED2,
    I_BOMBRESERVED3,
    I_BOMBRESERVED4,
    I_BOMBRESERVED5,
    I_BOMBRESERVED6,
    MOV_OBSTACLE,                   // attr1 = angle, attr2 = idx (mapmodel index), attr3 = health, attr4 = weight, attr5 = respawnmillis
    RACE_START,
    RACE_FINISH,
    RACE_CHECKPOINT,            // attr1 = angle, attr2 = checkpoint no
    P_GRAVITY,                  // attr1 = ?, attr2 = radius, attr3 = manipulation value
    P_FRICTION,
    P_JUMP,
    P_SPEED,
    P_YAW,
    P_PITCH,
    P_MOVE,
    P_INERTIA,
    P_STEERCONTROL,
    // I_INVISIBLE,                // hide and seek
    MAXENTTYPES
};

#endif
