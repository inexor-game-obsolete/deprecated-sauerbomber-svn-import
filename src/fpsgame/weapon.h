#ifndef _FPSGAME_WEAPON_H
#define _FPSGAME_WEAPON_H

#include "fpsent.h"
#include "geom.h"
#include "ents.h"

namespace game {
    extern void explodelocal(const vec &v);
    extern void explode(bool local, fpsent *owner, const vec &v, dynent *safe, int dam, int gun);
}

#endif
