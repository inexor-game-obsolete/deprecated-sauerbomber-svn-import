// bomb.h: client and server state for bomb gamemode
#ifndef PARSEMESSAGES

#ifdef SERVMODE
struct buildanddestroyservmode : servmode {
#else
struct buildanddestroyclientmode : clientmode {
#endif

    struct baseinfo {
        vec o;
        string owner;
        int ammotype;
#ifndef SERVMODE
        vec ammopos;
#endif

        baseinfo() { reset(); }

        void reset() {
            owner[0] = '\0';
        }

    };

    vector<baseinfo> bases;

    void resetbases() {
        bases.shrink(0);
    }


#ifndef SERVMODE

  	void setup() {
        resetbases();
        loopv(entities::ents) {
            extentity *e = entities::ents[i];
            if(e->type!=BASE) continue;
            baseinfo &b = bases.add();
            b.o = e->o;
            b.ammopos = b.o;
            abovemodel(b.ammopos, "base/neutral");
            b.ammotype = e->attr1;
            e->type = MOV_OBSTACLE;
            movable *m = new movable(*e);
            movables.add(m);
            m->o = e->o;
            m->entidx = i;
            m->o.z += m->eyeheight;
            m->physstate = PHYS_FLOOR;
            m->resetinterp();
            updatedynentcache(m);
        }
  	}

    void senditems(packetbuf &p) {
        putint(p, N_BASES);
        putint(p, bases.length());
        loopv(bases)
        {
            baseinfo &b = bases[i];
            putint(p, b.ammotype);
            putint(p, int(b.o.x*DMF));
            putint(p, int(b.o.y*DMF));
            putint(p, int(b.o.z*DMF));
        }
    }

    void drawhud(fpsent *d, int w, int h) {
    }

    void preload() {
        static const char *basemodels[3] = { "base/neutral", "base/red", "base/blue" };
        loopi(3) preloadmodel(basemodels[i]);
    }

    void rendergame() {
        loopv(bases) {
            baseinfo &b = bases[i];
            const char *basename = b.owner[0] ? (strcmp(b.owner, player1->team) ? "base/red" : "base/blue") : "base/neutral";
            rendermodel(basename, ANIM_MAPMODEL|ANIM_LOOP, b.o, 0, 0, MDL_CULL_VFC | MDL_CULL_OCCLUDED);
            float fradius = 1.0f, fheight = 0.5f;
            regular_particle_flame(PART_FLAME, vec(b.ammopos.x, b.ammopos.y, b.ammopos.z - 4.5f), fradius, fheight, b.owner[0] ? (strcmp(b.owner, player1->team) ? 0x802020 : 0x2020FF) : 0x208020, 3, 2.0f);
        }
    }

    void renderscoreboard(g3d_gui &g, game::scoregroup &sg, int fgcolor, int bgcolor) {
    }

    void killed(fpsent *d, fpsent *actor) {
    }

    void gameconnect(fpsent *d) {
    }

#else

    void setup() {
    }

    void leavegame(clientinfo *ci, bool disconnecting) {
    }

    void died(clientinfo *target, clientinfo *actor) {
    }

    void cleanup(){
    }

#endif

};

#ifndef SERVMODE
extern buildanddestroyclientmode buildanddestroymode;
#endif

#elif SERVMODE

#endif
