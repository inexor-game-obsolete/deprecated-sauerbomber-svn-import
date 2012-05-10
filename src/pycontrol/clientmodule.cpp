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

#ifndef STANDALONE

#include "clientmodule.h"

#include <iostream>

namespace SbPy {

    /*
    static PyObject *guiStart(PyObject *self, PyObject *args) {
        PythonGui::guiStart();
        return Py_None;
    }

    static PyObject *guiEnd(PyObject *self, PyObject *args) {
        PythonGui::guiEnd();
        return Py_None;
    }
    */

    static PyObject *guiShow(PyObject *self, PyObject *args) {
        bool on;
        if(!PyArg_ParseTuple(args, "b", &on)) return 0;
        PythonGui::guiShow(on);
        return Py_None;
    }

    static PyObject *guiClear(PyObject *self, PyObject *args) {
        PythonGui::guiClear();
        return Py_None;
    }

    static PyObject *guiPushList(PyObject *self, PyObject *args) {
        PythonGui::guiPushList();
        return Py_None;
    }

    static PyObject *guiPopList(PyObject *self, PyObject *args) {
        PythonGui::guiPopList();
        return Py_None;
    }

    static PyObject *guiStrut(PyObject *self, PyObject *args) {
        float size;
        if(!PyArg_ParseTuple(args, "f", &size)) return 0;
        PythonGui::guiStrut(size);
        return Py_None;
    }

    static PyObject *guiSpace(PyObject *self, PyObject *args) {
        float size;
        if(!PyArg_ParseTuple(args, "f", &size)) return 0;
        PythonGui::guiSpace(size);
        return Py_None;
    }

    static PyObject *guiText(PyObject *self, PyObject *args) {
        int color;
        char* text;
        char* icon;
        if(!PyArg_ParseTuple(args, "sis", &text, &color, &icon)) return 0;
        PythonGui::guiText(text, color, icon);
        return Py_None;
    }

    static PyObject *guiButton(PyObject *self, PyObject *args) {
        int color;
        char* text;
        char* icon;
        if(!PyArg_ParseTuple(args, "sis", &text, &color, &icon)) return 0;
        PythonGui::guiButton(text, color, icon);
        return Py_None;
    }

    static PyObject *guiTitle(PyObject *self, PyObject *args) {
        int color;
        char* text;
        if(!PyArg_ParseTuple(args, "si", &text, &color)) return 0;
        PythonGui::guiTitle(text, color);
        return Py_None;
    }

    static PyObject *guiImage(PyObject *self, PyObject *args) {
        char* path;
        if(!PyArg_ParseTuple(args, "s", &path)) return 0;
        PythonGui::guiImage(path);
        return Py_None;
    }

    static PyObject *guiBar(PyObject *self, PyObject *args) {
        PythonGui::guiBar();
        return Py_None;
    }

    static PyObject *guiTab(PyObject *self, PyObject *args) {
        char* name;
        if(!PyArg_ParseTuple(args, "s", &name)) return 0;
        PythonGui::guiTab(name);
        return Py_None;
    }

    static PyObject *switchName(PyObject *self, PyObject *args) {
        fprintf(stderr, "switchName was called\n");
        char *name;
        if(!PyArg_ParseTuple(args, "s", &name)) return 0;
        game::switchname(name);
        return Py_None;
    }

    static PyObject *switchTeam(PyObject *self, PyObject *args) {
        fprintf(stderr, "switchTeam was called\n");
        char *team;
        if(!PyArg_ParseTuple(args, "s", &team)) return 0;
        game::switchteam(team);
        return Py_None;
    }

    static PyObject *switchPlayerModel(PyObject *self, PyObject *args) {
        fprintf(stderr, "switchTeam was called\n");
        int playermodel;
        if(!PyArg_ParseTuple(args, "i", &playermodel)) return 0;
        game::switchplayermodel(playermodel);
        return Py_None;
    }

    static PyObject *getClientName(PyObject *self, PyObject *args) {
        fprintf(stderr, "getClientName was called\n");
        int cn;
        if(!PyArg_ParseTuple(args, "i", &cn)) return 0;
        return Py_BuildValue("s", game::getclientname(cn));
    }

    static PyObject *getClientTeam(PyObject *self, PyObject *args) {
        fprintf(stderr, "getClientTeam was called\n");
        int cn;
        if(!PyArg_ParseTuple(args, "i", &cn)) return 0;
        return Py_BuildValue("s", game::getclientteam(cn));
    }

    static PyObject *getClientModel(PyObject *self, PyObject *args) {
        fprintf(stderr, "getClientModel was called\n");
        int cn;
        if(!PyArg_ParseTuple(args, "i", &cn)) return 0;
        return Py_BuildValue("i", game::getclientmodel(cn));
    }

    static PyObject *getClientIcon(PyObject *self, PyObject *args) {
        fprintf(stderr, "getClientIcon was called\n");
        int cn;
        if(!PyArg_ParseTuple(args, "i", &cn)) return 0;
        return Py_BuildValue("s", game::getclienticon(cn));
    }

    static PyObject *isMaster(PyObject *self, PyObject *args) {
        fprintf(stderr, "isMaster was called\n");
        int cn;
        if(!PyArg_ParseTuple(args, "i", &cn)) return 0;
        return Py_BuildValue("b", game::ismaster(cn));
    }

    static PyObject *isAdmin(PyObject *self, PyObject *args) {
        fprintf(stderr, "isAdmin was called\n");
        int cn;
        if(!PyArg_ParseTuple(args, "i", &cn)) return 0;
        return Py_BuildValue("b", game::isadmin(cn));
    }

    static PyObject *getMasterMode(PyObject *self, PyObject *args) {
        fprintf(stderr, "getMasterMode was called\n");
        return Py_BuildValue("i", game::mastermode);
    }

    static PyObject *masterModeName(PyObject *self, PyObject *args) {
        fprintf(stderr, "masterModeName was called\n");
        int mm;
        if(!PyArg_ParseTuple(args, "i", &mm)) return 0;
        return Py_BuildValue("s", server::mastermodename(mm, ""));
    }

    static PyObject *isSpectator(PyObject *self, PyObject *args) {
        fprintf(stderr, "isSpectator was called\n");
        int cn;
        if(!PyArg_ParseTuple(args, "i", &cn)) return 0;
        return Py_BuildValue("b", game::isspectator(cn) ? 1 : 0);
    }

    static PyObject *isAi(PyObject *self, PyObject *args) {
        fprintf(stderr, "isAi was called\n");
        int cn, type;
        if(!PyArg_ParseTuple(args, "ii", &cn, &type)) return 0;
        return Py_BuildValue("b", game::isai(cn, type) ? 1 : 0);
    }

    static PyObject *getClientNum(PyObject *self, PyObject *args) {
        fprintf(stderr, "getClientNum was called\n");
        char *name;
        if(!PyArg_ParseTuple(args, "s", &name)) return 0;
        return Py_BuildValue("s", game::parseplayer(name));
    }

    static PyObject *listClients(PyObject *self, PyObject *args) {
        fprintf(stderr, "listClients was called\n");
        bool local;
        if(!PyArg_ParseTuple(args, "b", &local)) return 0;
        game::listclients(local);
        return Py_None;
    }

    static PyObject *clearBans(PyObject *self, PyObject *args) {
        fprintf(stderr, "clearBans was called\n");
        game::clearbans();
        return Py_None;
    }

    static PyObject *kick(PyObject *self, PyObject *args) {
        fprintf(stderr, "kick was called\n");
        char* name;
        if(!PyArg_ParseTuple(args, "s", &name)) return 0;
        game::kick(name);
        return Py_None;
    }

    static PyObject *ignore(PyObject *self, PyObject *args) {
        fprintf(stderr, "ignore was called\n");
        int cn;
        if(!PyArg_ParseTuple(args, "i", &cn)) return 0;
        game::ignore(cn);
        return Py_None;
    }

    static PyObject *unignore(PyObject *self, PyObject *args) {
        fprintf(stderr, "unignore was called\n");
        int cn;
        if(!PyArg_ParseTuple(args, "i", &cn)) return 0;
        game::unignore(cn);
        return Py_None;
    }

    static PyObject *isIgnored(PyObject *self, PyObject *args) {
        fprintf(stderr, "isIgnored was called\n");
        int cn;
        if(!PyArg_ParseTuple(args, "i", &cn)) return 0;
        return Py_BuildValue("b", game::isignored(cn) ? 1 : 0);
    }

    static PyObject *setTeam(PyObject *self, PyObject *args) {
        fprintf(stderr, "setTeam was called\n");
        char *arg1;
        char *arg2;
        if(!PyArg_ParseTuple(args, "ss", &arg1, &arg2)) return 0;
        game::setteam(arg1, arg2);
        return Py_None;
    }

    static PyObject *setMaster(PyObject *self, PyObject *args) {
        fprintf(stderr, "setMaster was called\n");
        char *arg;
        if(!PyArg_ParseTuple(args, "s", &arg)) return 0;
        game::setmaster(arg);
        return Py_None;
    }

    static PyObject *setMasterMode(PyObject *self, PyObject *args) {
        fprintf(stderr, "setMasterMode was called\n");
        int mm;
        if(!PyArg_ParseTuple(args, "i", &mm)) return 0;
        game::setmastermode(mm);
        return Py_None;
    }

    static PyObject *setMode(PyObject *self, PyObject *args) {
        fprintf(stderr, "setMode was called\n");
        int mode;
        if(!PyArg_ParseTuple(args, "i", &mode)) return 0;
        game::setmode(mode);
        return Py_None;
    }

    static PyObject *getMode(PyObject *self, PyObject *args) {
        fprintf(stderr, "getMode was called\n");
        return Py_BuildValue("i", game::gamemode);
    }

    static PyObject *getRemaining(PyObject *self, PyObject *args) {
        fprintf(stderr, "getRemaining was called\n");
        return Py_BuildValue("i", game::maplimit - lastmillis);
    }

    static PyObject *changeMap(PyObject *self, PyObject *args) {
        fprintf(stderr, "changeMap was called\n");
        int mode;
        char* map;
        if(!PyArg_ParseTuple(args, "si", &map, &mode)) return 0;
        game::changemap(map, mode);
        return Py_None;
    }

    static PyObject *pauseGame(PyObject *self, PyObject *args) {
        fprintf(stderr, "pauseGame was called\n");
        int val;
        if(!PyArg_ParseTuple(args, "i", &val)) return 0;
        game::pausegame(&val);
        return Py_None;
    }

    static PyObject *isPaused(PyObject *self, PyObject *args) {
        fprintf(stderr, "isPaused was called\n");
        return Py_BuildValue("b", game::ispaused());
    }

    static PyObject *say(PyObject *self, PyObject *args) {
        fprintf(stderr, "say was called\n");
        char* text;
        if(!PyArg_ParseTuple(args, "s", &text)) return 0;
        game::toserver(text);
        return Py_None;
    }

    static PyObject *sayTeam(PyObject *self, PyObject *args) {
        fprintf(stderr, "sayTeam was called\n");
        char* text;
        if(!PyArg_ParseTuple(args, "s", &text)) return 0;
        game::sayteam(text);
        return Py_None;
    }

    static PyObject *getMap(PyObject *self, PyObject *args) {
        fprintf(stderr, "getMap was called\n");
        game::getmap();
        return Py_None;
    }

    static PyObject *stopDemo(PyObject *self, PyObject *args) {
        fprintf(stderr, "stopDemo was called\n");
        game::stopdemo();
        return Py_None;
    }

    static PyObject *recordDemo(PyObject *self, PyObject *args) {
        fprintf(stderr, "recordDemo was called\n");
        int val;
        if(!PyArg_ParseTuple(args, "i", &val)) return 0;
        game::recorddemo(val);
        return Py_None;
    }

    static PyObject *clearDemos(PyObject *self, PyObject *args) {
        fprintf(stderr, "clearDemos was called\n");
        int val;
        if(!PyArg_ParseTuple(args, "i", &val)) return 0;
        game::cleardemos(val);
        return Py_None;
    }

    static PyObject *getDemo(PyObject *self, PyObject *args) {
        fprintf(stderr, "getDemo was called\n");
        int val;
        if(!PyArg_ParseTuple(args, "i", &val)) return 0;
        game::getdemo(val);
        return Py_None;
    }

    static PyObject *listDemos(PyObject *self, PyObject *args) {
        fprintf(stderr, "listDemos was called\n");
        game::listdemos();
        return Py_None;
    }

    static PyObject *sendMap(PyObject *self, PyObject *args) {
        fprintf(stderr, "sendMap was called\n");
        game::sendmap();
        return Py_None;
    }

    static PyObject *gotoPlayer(PyObject *self, PyObject *args) {
        fprintf(stderr, "gotoPlayer was called\n");
        char* arg;
        if(!PyArg_ParseTuple(args, "s", &arg)) return 0;
        game::gotoplayer(arg);
        return Py_None;
    }

    static PyObject *gotoSel(PyObject *self, PyObject *args) {
        fprintf(stderr, "gotoSel was called\n");
        game::gotosel();
        return Py_None;
    }

    static PyObject *showScores(PyObject *self, PyObject *args) {
        fprintf(stderr, "showscores was called\n");
        game::showscores(true);
        return Py_None;
    }

    PyMethodDef ModuleMethods[] = {
        /* from clientmodule.cpp */
        {"switchName", switchName, METH_VARARGS, "Switches the name."},
        {"switchTeam", switchTeam, METH_VARARGS, "Switches the team."},
        {"switchPlayerModel", switchPlayerModel, METH_VARARGS, "Switches the team."},
        {"getClientName", getClientName, METH_VARARGS, "Switches the team."},
        {"getClientTeam", getClientTeam, METH_VARARGS, "Switches the team."},
        {"getClientModel", getClientModel, METH_VARARGS, "Switches the team."},
        {"getClientIcon", getClientIcon, METH_VARARGS, "Switches the team."},
        {"isMaster", isMaster, METH_VARARGS, "Switches the team."},
        {"isAdmin", isAdmin, METH_VARARGS, "Switches the team."},
        {"getMasterMode", getMasterMode, METH_VARARGS, "Switches the team."},
        {"masterModeName", masterModeName, METH_VARARGS, "Switches the team."},
        {"isSpectator", isSpectator, METH_VARARGS, "Switches the team."},
        {"isAi", isAi, METH_VARARGS, "Switches the team."},
        {"getClientNum", getClientNum, METH_VARARGS, "Switches the team."},
        {"listClients", listClients, METH_VARARGS, "Switches the team."},
        {"clearBans", clearBans, METH_VARARGS, "Switches the team."},
        {"kick", kick, METH_VARARGS, "Switches the team."},
        {"ignore", ignore, METH_VARARGS, "Switches the team."},
        {"unignore", unignore, METH_VARARGS, "Switches the team."},
        {"isIgnored", isIgnored, METH_VARARGS, "Switches the team."},
        {"setTeam", setTeam, METH_VARARGS, "Switches the team."},
        {"setMaster", setMaster, METH_VARARGS, "Switches the team."},
        {"setMasterMode", setMasterMode, METH_VARARGS, "Switches the team."},
        {"setMode", setMode, METH_VARARGS, "Switches the team."},
        {"getMode", getMode, METH_VARARGS, "Switches the team."},
        {"getRemaining", getRemaining, METH_VARARGS, "Switches the team."},
        {"changeMap", changeMap, METH_VARARGS, "Switches the team."},
        {"pauseGame", pauseGame, METH_VARARGS, "Switches the team."},
        {"isPaused", isPaused, METH_VARARGS, "Switches the team."},
        {"say", say, METH_VARARGS, "Switches the team."},
        {"sayTeam", sayTeam, METH_VARARGS, "Switches the team."},
        {"getMap", getMap, METH_VARARGS, "Switches the team."},
        {"stopDemo", stopDemo, METH_VARARGS, "Switches the team."},
        {"recordDemo", recordDemo, METH_VARARGS, "Switches the team."},
        {"clearDemos", clearDemos, METH_VARARGS, "Switches the team."},
        {"getDemo", getDemo, METH_VARARGS, "Switches the team."},
        {"listDemos", listDemos, METH_VARARGS, "Switches the team."},
        {"sendMap", sendMap, METH_VARARGS, "Switches the team."},
        {"gotoPlayer", gotoPlayer, METH_VARARGS, "Switches the team."},
        {"gotoSel", gotoSel, METH_VARARGS, "Switches the team."},
        {"showScores", showScores, METH_VARARGS, "Shows the scoreboard."},
        /* from menus.cpp */
        {"guiShow", guiShow, METH_VARARGS, "Switches the name."},
        {"guiClear", guiClear, METH_VARARGS, "Switches the name."},
        {"guiPushList", guiPushList, METH_VARARGS, "Switches the name."},
        {"guiPopList", guiPopList, METH_VARARGS, "Switches the name."},
        {"guiStrut", guiStrut, METH_VARARGS, "Switches the name."},
        {"guiSpace", guiSpace, METH_VARARGS, "Switches the name."},
        {"guiText", guiText, METH_VARARGS, "Switches the name."},
        {"guiButton", guiButton, METH_VARARGS, "Switches the name."},
        {"guiTitle", guiTitle, METH_VARARGS, "Switches the name."},
        {"guiImage", guiImage, METH_VARARGS, "Switches the name."},
        {"guiBar", guiBar, METH_VARARGS, "Switches the name."},
        {"guiTab", guiTab, METH_VARARGS, "Switches the name."},
        /* from sharedmodule.cpp */
        {"consoleOutput", consoleOutput, METH_VARARGS, "Console output."},
        {"isClient", isClient, METH_VARARGS, "True if client."},
        {"isServer", isServer, METH_VARARGS, "True if server."},
        {"pyscriptspath", pyscriptspath, METH_VARARGS, "Plugin path."},
        {"configdir", configdir, METH_VARARGS, "Python config dir."},
        {NULL, NULL, 0, NULL}
    };

}

#endif
