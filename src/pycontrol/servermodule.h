/*
 *   Copyright (C) 2009 Gregory Haynes <greg@greghaynes.net>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef SBPY_SERVERMODULE_H
#define SBPY_SERVERMODULE_H

#include "sbpy.h"
#include "sharedmodule.h"
#include "server.h"
#include "modes.h"
#include "mastermodes.h"
#include "nmessages.h"
#include "disconnect.h"

extern int totalmillis;
extern char *serverip;
extern int serverport;
extern int maxclients;

// server functions (defined in engine/server.cpp)
extern ENetPacket *sendf(int cn, int chan, const char *format, ...);
extern uint getclientip(int n);
extern void disconnect_client(int n, int reason);

namespace server {
    // server functions (defined in fpsgame/server.cpp)
    extern const char *modename(int n, const char *unknown = "unknown");
    extern const char *mastermodename(int n, const char *unknown = "unknown");
    extern int numclients(int exclude = -1, bool nospec = true, bool noai = true, bool priv = false);
}

namespace SbPy {
    // python settings (defined in engine/server)
    extern PyObject *isClient(PyObject *self, PyObject *args);
    extern PyObject *isServer(PyObject *self, PyObject *args);
    extern PyObject *pyscriptspath(PyObject *self, PyObject *args);
    extern PyObject *configdir(PyObject *self, PyObject *args);
}

#endif

