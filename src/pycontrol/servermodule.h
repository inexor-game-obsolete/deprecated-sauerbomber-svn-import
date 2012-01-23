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

#include "sbpy.h"
#include "sharedmodule.h"

#ifndef SBPY_SERVERMODULE_H
#define SBPY_SERVERMODULE_H

namespace SbPy {
    // python setting (defined in engine/server)
    extern PyObject *pyscriptspath(PyObject *self, PyObject *args);
    extern PyObject *configdir(PyObject *self, PyObject *args);
}

namespace server {
    // server functions (defined in fpsgame/server.cpp)
    struct clientinfo;
    // struct demofile;
    // struct servmode;
    extern vector<clientinfo *> connects, clients, bots;
    // extern vector<demofile> demos;
    extern bool demonextmatch;

    // extern servmode *smode;

    extern char *pyconfigpath;
    extern int numclients(int exclude = -1, bool nospec = true, bool noai = true, bool priv = false);
    extern clientinfo *getinfo(int n);

    extern void senddemo(clientinfo *ci, int num);
    extern void suicide(clientinfo *ci);
}


#endif

