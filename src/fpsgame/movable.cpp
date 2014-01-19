// movable.cpp: implements physics for inanimate models
#include "game.h"

extern int physsteps;

namespace game
{
    enum
    {
        BOXWEIGHT = 25,
        BARRELHEALTH = 50,
        BARRELWEIGHT = 25,
        OBSTACLEHEALTH = 1,
        PLATFORMWEIGHT = 1000,
        PLATFORMSPEED = 8,
        EXPLODEDELAY = 200
    };

    struct movable : dynent
    {
        int etype, entidx, mapmodel, health, weight, exploding, tag, dir;
        physent *stacked;
        vec stackpos;

        movable(const entity &e) : 
            etype(e.type),
            entidx(-1),
            mapmodel(e.attr2),
            health(e.type==BARREL ? (e.attr4 ? e.attr4 : BARRELHEALTH) : (e.type==OBSTACLE ? (e.attr3 ? e.attr3 : OBSTACLEHEALTH) : 0)),
            weight(e.type!=OBSTACLE ? (e.type==PLATFORM || e.type==ELEVATOR ? PLATFORMWEIGHT : (e.attr3 ? e.attr3 : (e.type==BARREL ? BARRELWEIGHT : BOXWEIGHT))) : 0),
            exploding(0),
            tag(e.type==PLATFORM || e.type==ELEVATOR ? e.attr3 : 0),
            dir(e.type==PLATFORM || e.type==ELEVATOR ? (e.attr4 < 0 ? -1 : 1) : 0),
            stacked(NULL),
            stackpos(0, 0, 0)
        {
            state = CS_ALIVE;
            type = ENT_INANIMATE;
            yaw = e.attr1;
            if(e.type==PLATFORM || e.type==ELEVATOR) 
            {
                maxspeed = e.attr4 ? fabs(float(e.attr4)) : PLATFORMSPEED;
                if(tag) vel = vec(0, 0, 0);
                else if(e.type==PLATFORM) { vecfromyawpitch(yaw, 0, 1, 0, vel); vel.mul(dir*maxspeed); } 
                else vel = vec(0, 0, dir*maxspeed);
            }

            const char *mdlname = mapmodelname(e.attr2);
            if(mdlname) setbbfrommodel(this, mdlname);
        }
       
        void hitpush(int damage, const vec &dir, fpsent *actor, int gun)
        {
            if(etype!=BOX && etype!=BARREL) return; // TODO: hitpush obstacle?
            vec push(dir);
            push.mul(80*damage/weight);
            vel.add(push);
        }

        void explode(dynent *at)
        {
            // conoutf("movable.cpp::explode etype=%i state=%i health=%i",etype,state,health);
            state = CS_DEAD;
            exploding = 0;
            if(etype==OBSTACLE) {
                // obstacles explodes not only locally
                int damage = m_bomb ? 0 : guns[GUN_BARREL].damage; // in bomb mode, obstacles doesn't cause any damage
                game::explode(false, (fpsent *)at, o, this, damage, GUN_BARREL);

            }
            else game::explode(true, (fpsent *)at, o, this, guns[GUN_BARREL].damage, GUN_BARREL);
        }

        void damaged(int damage, fpsent *at, int gun = -1)
        {
            // conoutf("movable.cpp::damaged etype=%i state=%i health=%i damage=%i gun=%i",etype,state,health,damage,gun);
            if((etype!=BARREL && etype!=OBSTACLE) || state!=CS_ALIVE || exploding) return;
            health -= damage;
            // conoutf("movable.cpp::damaged health new=%i", health);
            if (m_obstacles) sync();
            if(health>0) return;
            if(gun==GUN_BARREL) exploding = lastmillis + EXPLODEDELAY;
            else explode(at); 
        }

        void suicide()
        {
            state = CS_DEAD;
            if(etype==BARREL || etype==OBSTACLE) explode(player1);
        }

        void sync()
        {
            addmsg(N_MOVEABLE, "riii", entidx, state, health);
        }

        void gotsync(int s, int h)
        {
            health = h;
            if (health <= 0) {
                state = CS_DEAD;
                exploding = 0;
                explodelocal(o);
            } else {
                state = s;
            }
        }
    };

    vector<movable *> movables;
   
    void clearmovables()
    {
        if(movables.length())
        {
            cleardynentcache();
            movables.deletecontents();
        }
        if(!m_dmsp && !m_classicsp && !m_obstacles) return;
        loopv(entities::ents) 
        {
            const entity &e = *entities::ents[i];
            if(e.type!=BOX && e.type!=BARREL && e.type!=OBSTACLE && e.type!=PLATFORM && e.type!=ELEVATOR) continue;
            movable *m = new movable(e);
            movables.add(m);
            m->o = e.o;
            m->entidx = i;
            if(e.type!=OBSTACLE) entinmap(m);
            else // obstacles doesn't care about collision
            {
                m->o.z += m->eyeheight;
                m->physstate = PHYS_FLOOR;
                m->resetinterp();
            }
            updatedynentcache(m);
        }
    }

    void triggerplatform(int tag, int newdir)
    {
        newdir = max(-1, min(1, newdir));
        loopv(movables)
        {
            movable *m = movables[i];
            if(m->state!=CS_ALIVE || (m->etype!=PLATFORM && m->etype!=ELEVATOR) || m->tag!=tag) continue;
            if(!newdir)
            {
                if(m->tag) m->vel = vec(0, 0, 0);
                else m->vel.neg();
            }
            else
            {
                if(m->etype==PLATFORM) { vecfromyawpitch(m->yaw, 0, 1, 0, m->vel); m->vel.mul(newdir*m->dir*m->maxspeed); }
                else m->vel = vec(0, 0, newdir*m->dir*m->maxspeed);
            }
        }
    }
    ICOMMAND(platform, "ii", (int *tag, int *newdir), triggerplatform(*tag, *newdir));

    void stackmovable(movable *d, physent *o)
    {
        d->stacked = o;
        d->stackpos = o->o;
    }

    void updatemovables(int curtime)
    {
        if(!curtime) return;
        loopv(movables)
        {
            movable *m = movables[i];
            if(m->state!=CS_ALIVE) continue;
            if(m->etype==PLATFORM || m->etype==ELEVATOR)
            {
                if(m->vel.iszero()) continue;
                for(int remaining = curtime; remaining>0;)
                {
                    int step = min(remaining, 20);
                    remaining -= step;
                    if(!moveplatform(m, vec(m->vel).mul(step/1000.0f)))
                    {
                        if(m->tag) { m->vel = vec(0, 0, 0); break; }
                        else m->vel.neg();
                    }
                }
            }
            else if(m->exploding && lastmillis >= m->exploding)
            {
                m->explode(m);
                adddecal(DECAL_SCORCH, m->o, vec(0, 0, 1), RL_DAMRAD/2);
            }
            else if(m->maymove() || (m->stacked && (m->stacked->state!=CS_ALIVE || m->stackpos != m->stacked->o)))
            {
                if(physsteps > 0) m->stacked = NULL;
                moveplayer(m, 1, true);
            }
        }
    }

    void rendermovables()
    {
        loopv(movables)
        {
            movable &m = *movables[i];
            if(m.state!=CS_ALIVE) continue;
            vec o = m.feetpos();
            const char *mdlname = mapmodelname(m.mapmodel);
            if(!mdlname) continue;
            rendermodel(NULL, mdlname, ANIM_MAPMODEL|ANIM_LOOP, o, m.yaw, 0, MDL_LIGHT | MDL_SHADOW | MDL_CULL_VFC | MDL_CULL_DIST | MDL_CULL_OCCLUDED, &m);
        }
    }
    
    void suicidemovable(movable *m)
    {
        m->suicide();
    }

    void hitmovable(int damage, movable *m, fpsent *at, const vec &vel, int gun)
    {
        m->hitpush(damage, vel, at, gun);
        m->damaged(damage, at, gun);
    }

    void syncmovable(int entidx, int state, int health)
    {
        loopv(movables) if (movables[i]->entidx == entidx) movables[i]->gotsync(state, health);
    }

    bool isobstaclealive(movable *m)
    {
        return (m->state==CS_ALIVE && m->etype==OBSTACLE);
    }

}

