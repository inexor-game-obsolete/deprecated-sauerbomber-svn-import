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

#include "menus.h"

namespace game {

    enum {
        PYGUI_PUSHLIST,
        PYGUI_POPLIST,
        PYGUI_STRUT,
        PYGUI_SPACE,
        PYGUI_TEXT,
        PYGUI_BUTTON,
        PYGUI_TITLE,
        PYGUI_IMAGE,
        PYGUI_BAR,
        PYGUI_TAB
    };
    struct pyguie {
        int type;
        int i;
        char* s1;
        char* s2;
        float f;
    };

    struct pygui : g3d_callback {
        bool showing;
        vec menupos;
        int menustart;

        vector<pyguie*> pyguiElements;

        pygui() : showing(false) {}

        void show(bool on) {
            if(!showing && on) {
                menupos = menuinfrontofplayer();
                menustart = starttime();
            }
            showing = on;
        }

        void renderpygui(g3d_gui &g, bool firstpass) {
            loopv(pyguiElements) {
                switch(pyguiElements[i]->type) {
                    case PYGUI_PUSHLIST:
                        g.pushlist();
                        break;
                    case PYGUI_POPLIST:
                        g.poplist();
                        break;
                    case PYGUI_STRUT:
                        g.strut(5.0f);
                        break;
                    case PYGUI_SPACE:
                        g.space(1.0f);
                        break;
                    case PYGUI_TEXT:
                        g.text(pyguiElements[i]->s1, pyguiElements[i]->i, strcmp(pyguiElements[i]->s2, "") ? NULL : pyguiElements[i]->s2);
                        break;
                    case PYGUI_BUTTON:
                        g.button(pyguiElements[i]->s1, pyguiElements[i]->i, strcmp(pyguiElements[i]->s2, "") ? NULL : pyguiElements[i]->s2);
                        break;
                    case PYGUI_TITLE:
                        g.title(pyguiElements[i]->s1, pyguiElements[i]->i);
                        break;
                    case PYGUI_IMAGE:
                    {
                        Texture *t = textureload(pyguiElements[i]->s1, 0, true, false);
                        if(t==notexture) break;
                        g.image(t, 1.0f, 0);
                        break;
                    }
                    case PYGUI_BAR:
                        g.separator();
                        break;
                    case PYGUI_TAB:
                        g.tab(pyguiElements[i]->s1, GUI_TITLE_COLOR);
                        break;
                }
            }
        }

        void gui(g3d_gui &g, bool firstpass) {
            g.start(menustart, 0.03f, NULL, false);
            renderpygui(g, firstpass);
            g.end();
        }

        void render() {
            if(showing) g3d_addgui(this, menupos, GUI_FORCE_2D | GUI_BOTTOM);
        }

        void reset() {
            pyguiElements.deletecontents();
        }

    } pymenu;

    void g3d_pymenus() {
        pymenu.render();
    }

}

namespace PythonGui {

    void guiShow(bool on) {
        game::pymenu.show(on);
    }

    void guiClear() {
        game::pymenu.reset();
    }

    void guiPushList() {
        game::pyguie* e = new game::pyguie;
        e->type = game::PYGUI_PUSHLIST;
        game::pymenu.pyguiElements.add(e);
    }

    void guiPopList() {
        game::pyguie* e = new game::pyguie;
        e->type = game::PYGUI_POPLIST;
        game::pymenu.pyguiElements.add(e);
    }

    void guiStrut(float size) {
        game::pyguie* e = new game::pyguie;
        e->type = game::PYGUI_STRUT;
        e->f = size;
        game::pymenu.pyguiElements.add(e);
    }

    void guiSpace(float size) {
        game::pyguie* e = new game::pyguie;
        e->type = game::PYGUI_SPACE;
        e->f = size;
        game::pymenu.pyguiElements.add(e);
    }

    void guiText(const char *text, int color, const char *icon = NULL) {
        game::pyguie* e = new game::pyguie;
        e->type = game::PYGUI_TEXT;
        e->s1 = newstring(text);
        e->s2 = newstring(icon);
        e->i = color;
        game::pymenu.pyguiElements.add(e);
    }

    void guiButton(const char *text, int color, const char *icon = NULL) {
        game::pyguie* e = new game::pyguie;
        e->type = game::PYGUI_BUTTON;
        e->s1 = newstring(text);
        e->s2 = newstring(icon);
        e->i = color;
        game::pymenu.pyguiElements.add(e);
    }

    void guiTitle(char *text, int color) {
        game::pyguie* e = new game::pyguie;
        e->type = game::PYGUI_TITLE;
        e->s1 = newstring(text);
        e->i = color;
        game::pymenu.pyguiElements.add(e);
    }

    void guiImage(char *path) {
        game::pyguie* e = new game::pyguie;
        e->type = game::PYGUI_IMAGE;
        e->s1 = newstring(path);
        game::pymenu.pyguiElements.add(e);
    }

    void guiBar() {
        game::pyguie* e = new game::pyguie;
        e->type = game::PYGUI_BAR;
        game::pymenu.pyguiElements.add(e);
    }

    void guiTab(char *name) {
        game::pyguie* e = new game::pyguie;
        e->type = game::PYGUI_TITLE;
        e->s1 = newstring(name);
        game::pymenu.pyguiElements.add(e);
    }

}


/*
    if(cgui) {
        cgui->pushlist();
        g->pushlist();
        g->text(names[i], 0xFFFF80, !i ? " " : NULL);
        g->strut(3);
        g->mergehits(true);
        cgui->poplist();
    }
    // if(header) execute(header);
    serverinfo *sc = NULL;
    for(int start = 0; start < servers.length();)
    {
        if(start > 0) cgui->tab();
        if(header) execute(header);
        int end = servers.length();
        cgui->pushlist();
        loopi(10)
        {
            if(!game::serverinfostartcolumn(cgui, i)) break;
            for(int j = start; j < end; j++)
            {
                if(!i && cgui->shouldtab()) { end = j; break; }
                serverinfo &si = *servers[j];
                const char *sdesc = si.sdesc;
                if(si.address.host == ENET_HOST_ANY) sdesc = "[unknown host]";
                else if(si.ping == serverinfo::WAITING) sdesc = "[waiting for response]";
                if(game::serverinfoentry(cgui, i, si.name, si.port, sdesc, si.map, sdesc == si.sdesc ? si.ping : -1, si.attr, si.numplayers))
                    sc = &si;
            }
            game::serverinfoendcolumn(cgui, i);
        }
        cgui->poplist();
        start = end;
    }
    if(selectedserver || !sc) return NULL;
    selectedserver = sc;
    return newstring("connectselected");
*/


