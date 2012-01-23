/*
 *   Copyright (C) 2012 Hanack <hanack@nooblounge.net>
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

#ifndef SBPY_CLIENTMODULE_H
#define SBPY_CLIENTMODULE_H

extern int lastmillis;

namespace SbPy {
    // python setting (defined in engine/server)
    extern PyObject *pyscriptspath(PyObject *self, PyObject *args);
    extern PyObject *configdir(PyObject *self, PyObject *args);

}

namespace PythonGui {
    // python gui (defined in pycontrol/menus.cpp)
    extern void guiShow(bool on);
    extern void guiClear();
    extern void guiPushList();
    extern void guiPopList();
    extern void guiStrut(float size);
    extern void guiSpace(float size);
    extern void guiText(const char *text, int color, const char *icon);
    extern void guiButton(const char *text, int color, const char *icon);
    extern void guiTitle(char *text, int color);
    extern void guiImage(char *path);
    extern void guiBar();
    extern void guiTab(char *name);
}

namespace game {
    // client functions (defined in fpsgame/client.cpp)
    extern int mastermode;
    extern int gamemode;
    extern int maplimit;
    extern void showscores(bool on);
    extern void switchname(const char *name);
    extern void switchteam(const char *team);
    extern void switchplayermodel(int playermodel);
    // void addauthkey(const char *name, const char *key, const char *desc)
    // bool hasauthkey(const char *name, const char *desc)
    // void genauthkey(const char *secret)
    // void saveauthkeys()
    extern const char *getclientname(int cn);
    extern const char *getclientteam(int cn);
    extern int getclientmodel(int cn);
    extern const char *getclienticon(int cn);
    extern bool ismaster(int cn);
    extern bool isadmin(int cn);
    extern bool isspectator(int cn);
    extern bool isai(int cn, int type);
    extern int parseplayer(const char *arg);
    extern void listclients(bool local);
    extern void clearbans();
    extern void kick(const char *arg);
    extern void ignore(int cn);
    extern void unignore(int cn);
    extern bool isignored(int cn);
    extern void setteam(const char *arg1, const char *arg2);
    // void hashpwd(const char *pwd)
    extern void setmaster(const char *arg);
    extern void setmastermode(int mm);
    // bool tryauth(const char *desc)
    extern void setmode(int mode);
    extern void changemap(const char *name, int mode);
    extern void pausegame(int *val);
    extern bool ispaused();
    extern void toserver(char *text);
    extern void sayteam(char *text);
    extern void getmap();
    extern void stopdemo();
    extern void recorddemo(int val);
    extern void cleardemos(int val);
    extern void getdemo(int i);
    extern void listdemos();
    extern void sendmap();
    extern void gotoplayer(const char *arg);
    extern void gotosel();

    // client functions (defined in fpsgame/fps.cpp)
    extern void taunt();
    // getfollow
    extern void follow(char *arg);
    extern void nextfollow(int dir);
    extern const char *getclientmap();
    extern void stopfollowing();

    // ...


};

#endif

