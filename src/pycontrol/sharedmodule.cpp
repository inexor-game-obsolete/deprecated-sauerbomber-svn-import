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

#include "sharedmodule.h"

namespace SbPy {

    extern PyMethodDef ModuleMethods[];
    bool isserver;
    bool isclient;

    PyObject *isClient(PyObject *self, PyObject *args) {
        return Py_BuildValue("b", SbPy::isclient);
    }

    PyObject *isServer(PyObject *self, PyObject *args) {
      return Py_BuildValue("b", SbPy::isserver);
    }

    PyObject *pyscriptspath(PyObject *self, PyObject *args) {
        return Py_BuildValue("s", server::pyscriptspath);
    }

    PyObject *configdir(PyObject *self, PyObject *args) {
        return Py_BuildValue("s", server::pyconfigpath);
    }

    PyObject *consoleOutput(PyObject *self, PyObject *args) {
        char* text;
        if(!PyArg_ParseTuple(args, "s", &text)) return 0;
        // PythonConsole::conoutf(text);
        return Py_None;
    }

    PyMODINIT_FUNC initModule(const char *module_name) {
        (void) Py_InitModule(module_name, ModuleMethods);
        return;
    }

}
