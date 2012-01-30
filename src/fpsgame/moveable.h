#ifndef _FPSGAME_MOVEABLE_H
#define _FPSGAME_MOVEABLE_H

#include "game.h"
#include "ents.h"
#include "guns.h"
#include "weapon.h"
#include "modes.h"
#include "nmessages.h"
#include "client.h"

namespace game {
  /*
    struct movable : dynent {
        int etype, entidx, mapmodel, health, weight, exploding, tag, dir;
        physent *stacked;
        vec stackpos;
        virtual ~movable();
        virtual void hitpush(int damage, const vec &dir, fpsent *actor, int gun);
        virtual void explode(dynent *at);
        virtual void damaged(int damage, fpsent *at, int gun = -1);
        virtual void suicide();
        virtual void sync();
        virtual void gotsync(int s, int h);
    };
*/

    enum {
        BOXWEIGHT = 25,
        BARRELHEALTH = 50,
        BARRELWEIGHT = 25,
        OBSTACLEHEALTH = 1,
        PLATFORMWEIGHT = 1000,
        PLATFORMSPEED = 8,
        EXPLODEDELAY = 200
    };

    struct movable : dynent {
        int etype, entidx, mapmodel, health, weight, exploding, tag, dir;
        physent *stacked;
        vec stackpos;

        movable(const entity &e) :
            etype(e.type),
            entidx(-1),
            mapmodel(e.attr2),
            health(e.type==MOV_BARREL ? (e.attr4 ? e.attr4 : BARRELHEALTH) : (e.type==MOV_OBSTACLE ? (e.attr3 ? e.attr3 : OBSTACLEHEALTH) : 0)),
            weight(e.type!=MOV_OBSTACLE ? (e.type==MOV_PLATFORM || e.type==MOV_ELEVATOR ? PLATFORMWEIGHT : (e.attr3 ? e.attr3 : (e.type==MOV_BARREL ? BARRELWEIGHT : BOXWEIGHT))) : 0),
            exploding(0),
            tag(e.type==MOV_PLATFORM || e.type==MOV_ELEVATOR ? e.attr3 : 0),
            dir(e.type==MOV_PLATFORM || e.type==MOV_ELEVATOR ? (e.attr4 < 0 ? -1 : 1) : 0),
            stacked(NULL),
            stackpos(0, 0, 0)
        {
            state = CS_ALIVE;
            type = ENT_INANIMATE;
            yaw = e.attr1;
            if(e.type==MOV_PLATFORM || e.type==MOV_ELEVATOR) {
                maxspeed = e.attr4 ? fabs(float(e.attr4)) : PLATFORMSPEED;
                if(tag) vel = vec(0, 0, 0);
                else if(e.type==MOV_PLATFORM) { vecfromyawpitch(yaw, 0, 1, 0, vel); vel.mul(dir*maxspeed); }
                else vel = vec(0, 0, dir*maxspeed);
            }

            const char *mdlname = mapmodelname(e.attr2);
            if(mdlname) setbbfrommodel(this, mdlname);
        }

        movable(const extentity &e) :
            etype(e.type),
            entidx(-1),
            mapmodel(e.attr2),
            health(e.type==MOV_BARREL ? (e.attr4 ? e.attr4 : BARRELHEALTH) : (e.type==MOV_OBSTACLE ? (e.attr3 ? e.attr3 : OBSTACLEHEALTH) : 0)),
            weight(e.type!=MOV_OBSTACLE ? (e.type==MOV_PLATFORM || e.type==MOV_ELEVATOR ? PLATFORMWEIGHT : (e.attr3 ? e.attr3 : (e.type==MOV_BARREL ? BARRELWEIGHT : BOXWEIGHT))) : 0),
            exploding(0),
            tag(e.type==MOV_PLATFORM || e.type==MOV_ELEVATOR ? e.attr3 : 0),
            dir(e.type==MOV_PLATFORM || e.type==MOV_ELEVATOR ? (e.attr4 < 0 ? -1 : 1) : 0),
            stacked(NULL),
            stackpos(0, 0, 0)
        {
            state = CS_ALIVE;
            type = ENT_INANIMATE;
            yaw = e.attr1;
            if(e.type==MOV_PLATFORM || e.type==MOV_ELEVATOR) {
                maxspeed = e.attr4 ? fabs(float(e.attr4)) : PLATFORMSPEED;
                if(tag) vel = vec(0, 0, 0);
                else if(e.type==MOV_PLATFORM) { vecfromyawpitch(yaw, 0, 1, 0, vel); vel.mul(dir*maxspeed); }
                else vel = vec(0, 0, dir*maxspeed);
            }

            const char *mdlname = mapmodelname(e.attr2);
            if(mdlname) setbbfrommodel(this, mdlname);
        }

        void hitpush(int damage, const vec &dir, fpsent *actor, int gun) {
            if(etype!=MOV_BOX && etype!=MOV_BARREL) return; // TODO: hitpush obstacle?
            vec push(dir);
            push.mul(80*damage/weight);
            vel.add(push);
        }

        void explode(dynent *at) {
            // conoutf("movable.cpp::explode etype=%i state=%i health=%i",etype,state,health);
            state = CS_DEAD;
            exploding = 0;
            if(etype==MOV_OBSTACLE) {
                // obstacles explodes not only locally
                int damage = m_bomb ? 0 : guns[GUN_BARREL].damage; // in bomb mode, obstacles doesn't cause any damage
                game::explode(false, (fpsent *)at, o, this, damage, GUN_BARREL);
            } else {
                game::explode(true, (fpsent *)at, o, this, guns[GUN_BARREL].damage, GUN_BARREL);
            }
        }

        void damaged(int damage, fpsent *at, int gun = -1) {
            if((etype!=MOV_BARREL && etype!=MOV_OBSTACLE) || state!=CS_ALIVE || exploding) return;
            health -= damage;
            if (m_obstacles) sync();
            if(health>0) return;
            if(gun==GUN_BARREL) exploding = lastmillis + EXPLODEDELAY;
            else explode(at);
        }

        void suicide() {
            state = CS_DEAD;
            if(etype==MOV_BARREL || etype==MOV_OBSTACLE) explode(player1);
        }

        void sync() {
            addmsg(N_MOVEABLE, "riii", entidx, state, health);
        }

        void gotsync(int s, int h) {
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

    extern vector<movable *> movables;

}

#endif
