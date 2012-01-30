#ifndef FPSGAME_CLIENT_H
#define FPSGAME_CLIENT_H

#include "fpsent.h"

namespace game {
    extern int gamemode;
    extern fpsent *player1;
    extern void addmsg(int type, const char *fmt, ...);
}

#endif
