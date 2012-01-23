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
 *
 *
 *   Adapted for Sauerbomber by Hanack 2012
 *
 */

#include "sbpy.h"

#include <string>
#include <iostream>

#include <Python.h>

#ifdef _WIN32
#include <direct.h> // for _chdir()
#define chdir _chdir
#define getcwd _getcwd
#endif

extern int totalmillis;

namespace SbPy {

    static char *pyscripts_path;
    static char *pn;
    static PyObject *eventsModule, *triggerEventFunc, *triggerPolicyEventFunc, *updateFunc;

    PyMODINIT_FUNC initModule(const char *);

    void loadPyscriptsPath() {
        char *path = getenv("SB_PYSCRIPTS_PATH");
        if(!path) pyscripts_path = path;
    }

    void initEnv() {
        std::string newpath;
        const char *pp;
        if(!pyscripts_path) return;
       	pp = getenv("PYTHONPATH");
        char absolute_path[1024];
        getcwd(absolute_path, 1024);
        if(!pp) {
            newpath = absolute_path;
            newpath.append("/");
            newpath.append(pyscripts_path);
        } else {
            newpath = pp;
            newpath.append(":");
            newpath.append(absolute_path);
            newpath.append("/");
            newpath.append(pyscripts_path);
        }
#ifndef _WIN32
        setenv("PYTHONPATH", newpath.c_str(), 1);
#endif
    }

#define SBPY_ERR(x) \
	  if(!x) { \
		    if(PyErr_Occurred()) \
			      PyErr_Print(); \
		    return false;\
	  }

    bool initPy() {
        PyObject *pFunc = 0, *pArgs = 0, *pValue = 0, *pluginsModule = 0;
        fprintf(stderr, "Python %s\n", Py_GetVersion());
        eventsModule = PyImport_ImportModule("core.events");
        SBPY_ERR(eventsModule)
        triggerEventFunc = PyObject_GetAttrString(eventsModule, "triggerServerEvent");
        SBPY_ERR(triggerEventFunc);
        if(!PyCallable_Check(triggerEventFunc)) {
            fprintf(stderr, "Error: triggerEvent function could not be loaded.\n");
            return false;
        }
        triggerPolicyEventFunc = PyObject_GetAttrString(eventsModule, "triggerPolicyEvent");
        SBPY_ERR(triggerPolicyEventFunc);
        if(!PyCallable_Check(triggerPolicyEventFunc)) {
            fprintf(stderr, "Error: triggerPolicyEvent function could not be loaded.\n");
            return false;
        }
        updateFunc = PyObject_GetAttrString(eventsModule, "update");
        SBPY_ERR(updateFunc);
        if(!PyCallable_Check(updateFunc)) {
            fprintf(stderr, "Error: update function could not be loaded.\n");
            return false;
        }
        pluginsModule = PyImport_ImportModule("core.plugins");
        SBPY_ERR(pluginsModule)
        pFunc = PyObject_GetAttrString(pluginsModule, "loadPlugins");
        SBPY_ERR(pFunc)
        if(!PyCallable_Check(pFunc)) {
            fprintf(stderr, "Error: loadPlugins function could not be loaded.\n");
            return false;
        }
        pArgs = PyTuple_New(0);
        pValue = PyObject_CallObject(pFunc, pArgs);
        Py_DECREF(pArgs);
        Py_DECREF(pFunc);
        if(!pValue) {
            PyErr_Print();
            return false;
        }
        Py_DECREF(pValue);
        Py_DECREF(pluginsModule);
        return true;
    }

    void deinitPy() {
        Py_XDECREF(triggerEventFunc);
        Py_XDECREF(triggerPolicyEventFunc);
        Py_XDECREF(updateFunc);
        Py_XDECREF(eventsModule);
        Py_Finalize();
    }

    bool init(const char *prog_name, const char *arg_pyscripts_path, const char *module_name) {
        // Setup env vars and chdir
        if(!pn) {
            pn = new char[strlen(prog_name)+1];
            strcpy(pn, prog_name);
        }
        if(arg_pyscripts_path[0] || pyscripts_path) {
            if(!pyscripts_path) {
                pyscripts_path = new char[strlen(arg_pyscripts_path)+1];
                strcpy(pyscripts_path, arg_pyscripts_path);
            }
        } else loadPyscriptsPath();
        if(!pyscripts_path) {
            fprintf(stderr, "Fatal Error: Could not locate a pyscripts directory.\n");
            return false;
        }
        initEnv();

        // Set program name
        Py_SetProgramName(pn);

        // Initialize
        Py_Initialize();
        initModule(module_name);
        if(!initPy()) {
            fprintf(stderr, "Error initializing python modules.\n");
            return false;
        }
        return true;
    }

    PyObject *callPyFunc(PyObject *func, PyObject *args) {
        PyObject *val;
        val = PyObject_CallObject(func, args);
        Py_DECREF(args);
        if(!val) PyErr_Print();
        return val;
    }

    bool triggerFuncEvent(const char *name, vector<PyObject*> *args, PyObject *func) {
        PyObject *pArgs, *pArgsArgs, *pName, *pValue;

        if(!func) {
            fprintf(stderr, "Python Error: Invalid handler to triggerEvent function.\n");
            return false;
        }
        pArgs = PyTuple_New(2);
        pName = PyString_FromString(name);
        SBPY_ERR(pName)
        PyTuple_SetItem(pArgs, 0, pName);
        if(args) {
            pArgsArgs = PyTuple_New(args->length());
            loopv(*args) {
                PyObject* pyO = (*args)[i];
                PyTuple_SetItem(pArgsArgs, i, pyO);
            }
        } else {
            pArgsArgs = PyTuple_New(0);
        }
        PyTuple_SetItem(pArgs, 1, pArgsArgs);
        pValue = callPyFunc(func, pArgs);
        if(!pValue) {
            fprintf(stderr, "Error triggering event.\n");
            return false;
        }
        if(PyBool_Check(pValue)) {
            bool ret = (pValue == Py_True);
            Py_DECREF(pValue);
            return ret;
        }
        Py_DECREF(pValue);
        return true;
    }

#undef SBPY_ERR

    bool triggerFuncEventInt(const char *name, int cn, PyObject *func) {
        vector<PyObject*> args;
        PyObject *pCn = PyInt_FromLong(cn);
        args.add(pCn);
        return triggerFuncEvent(name, &args, func);
    }

    bool triggerFuncEventString(const char *name, const char *str, PyObject *func) {
        vector<PyObject*> args;
        PyObject *pCn = PyString_FromString(str);
        args.add(pCn);
        return triggerFuncEvent(name, &args, func);
    }

    bool triggerFuncEventIntBool(const char *name, int cn, bool b, PyObject *func) {
        vector<PyObject*> args;
        PyObject *pCn = PyInt_FromLong(cn);
        PyObject *pB;
        if(b) pB = Py_True;
        else pB = Py_False;
        args.add(pCn);
        args.add(pB);
        return triggerFuncEvent(name, &args, func);
    }

    bool triggerFuncEventIntString(const char *name, int cn, const char *text, PyObject *func) {
        vector<PyObject*> args;
        PyObject *pText = PyString_FromString(text);
        PyObject *pCn = PyInt_FromLong(cn);
        args.add(pCn);
        args.add(pText);
        return triggerFuncEvent(name, &args, func);
    }

    bool triggerFuncEventIntStringInt(const char *name, int cn, const char *text, int cn2, PyObject *func) {
        vector<PyObject*> args;
        PyObject *pText = PyString_FromString(text);
        PyObject *pCn = PyInt_FromLong(cn);
        PyObject *pCn2 = PyInt_FromLong(cn2);
        args.add(pCn);
        args.add(pText);
        args.add(pCn2);
        return triggerFuncEvent(name, &args, func);
    }

    bool triggerFuncEventIntStringString(const char *name, int cn, const char *text, const char *text2, PyObject *func) {
        vector<PyObject*> args;
        PyObject *pText2 = PyString_FromString(text2);
        PyObject *pText = PyString_FromString(text);
        PyObject *pCn = PyInt_FromLong(cn);
        args.add(pCn);
        args.add(pText);
        args.add(pText2);
        return triggerFuncEvent(name, &args, func);
    }

    bool triggerEvent(const char*name, vector<PyObject*>* args) {
        return triggerFuncEvent(name, args, triggerEventFunc);
    }

    bool triggerEventInt(const char *name, int cn) {
        return triggerFuncEventInt(name, cn, triggerEventFunc);
    }

    bool triggerEventIntBool(const char *name, int cn, bool b) {
        return triggerFuncEventIntBool(name, cn, b, triggerEventFunc);
    }

    bool triggerEventStr(const char *name, const char *str) {
        return triggerFuncEventString(name, str, triggerEventFunc);
    }

    bool triggerEventIntString(const char *name, int cn, const char *text) {
        return triggerFuncEventIntString(name, cn, text, triggerEventFunc);
    }

    bool triggerEventIntStringInt(const char *name, int n, const char *str, int n2) {
        return triggerFuncEventIntStringInt(name, n, str, n2, triggerEventFunc);
    }

    bool triggerEventIntStringString(const char *name, int cn, const char *text, const char *text2) {
        return triggerFuncEventIntStringString(name, cn, text, text2, triggerEventFunc);
    }

    bool triggerEventIntInt(const char *name, int cn1, int cn2) {
        vector<PyObject*> args;
        PyObject *pCn1 = PyInt_FromLong(cn1);
        PyObject *pCn2 = PyInt_FromLong(cn2);
        args.add(pCn1);
        args.add(pCn2);
        return triggerFuncEvent(name, &args, triggerEventFunc);
    }

    bool triggerEventIntIntInt(const char *name, int arg1, int arg2, int arg3) {
        vector<PyObject*> args;
        PyObject *pArg1 = PyInt_FromLong(arg1);
        PyObject *pArg2 = PyInt_FromLong(arg2);
        PyObject *pArg3 = PyInt_FromLong(arg3);
        args.add(pArg1);
        args.add(pArg2);
        args.add(pArg3);
        return triggerFuncEvent(name, &args, triggerEventFunc);
    }

    bool triggerEventIntIntIntInt(const char *name, int arg1, int arg2, int arg3, int arg4) {
        vector<PyObject*> args;
        PyObject *pArg1 = PyInt_FromLong(arg1);
        PyObject *pArg2 = PyInt_FromLong(arg2);
        PyObject *pArg3 = PyInt_FromLong(arg3);
        PyObject *pArg4 = PyInt_FromLong(arg4);
        args.add(pArg1);
        args.add(pArg2);
        args.add(pArg3);
        args.add(pArg4);
        return triggerFuncEvent(name, &args, triggerEventFunc);
    }

    bool triggerEventIntIntString(const char *name, int cn1, int cn2, const char *text) {
        vector<PyObject*> args;
        PyObject *pCn1 = PyInt_FromLong(cn1);
        PyObject *pCn2 = PyInt_FromLong(cn2);
        PyObject *pTxt = PyString_FromString(text);
        args.add(pCn1);
        args.add(pCn2);
        args.add(pTxt);
        return triggerFuncEvent(name, &args, triggerEventFunc);
    }

    bool triggerEventStrInt(const char *name, const char *str, int n) {
        vector<PyObject*> args;
        PyObject *pstr, *pn;
        pstr = PyString_FromString(str);
        pn = PyInt_FromLong(n);
        args.add(pstr);
        args.add(pn);
        return triggerFuncEvent(name, &args, triggerEventFunc);
    }

    bool triggerPolicyEventInt(const char *name, int cn) {
        return triggerFuncEventInt(name, cn, triggerPolicyEventFunc);
    }

    bool triggerPolicyEventIntString(const char *name, int cn, const char *text) {
        return triggerFuncEventIntString(name, cn, text, triggerPolicyEventFunc);
    }

    void update() {
        if(!updateFunc) return;
        PyObject *pargs, *pvalue;
        pargs = PyTuple_New(0);
        pvalue = callPyFunc(updateFunc, pargs);
        if(pvalue) Py_DECREF(pvalue);
    }

}
