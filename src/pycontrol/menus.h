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

#ifndef SBPY_MENUS_H
#define SBPY_MENUS_H

#include "engine.h"

#define GUI_TITLE_COLOR  0xFFDD88
#define GUI_BUTTON_COLOR 0xFFFFFF
#define GUI_TEXT_COLOR   0xDDFFDD

namespace PythonGui {

    void guiShow(bool on);
    void guiClear();
    void guiPushList();
    void guiPopList();
    void guiStrut(float size);
    void guiSpace(float size);
    void guiText(const char *text, int color, const char *icon);
    void guiButton(const char *text, int color, const char *icon);
    void guiTitle(char *text, int color);
    void guiImage(char *path);
    void guiBar();
    void guiTab(char *name);

}

namespace game {

    // struct g3d_callback;
    void g3d_pymenus();

}

#endif
