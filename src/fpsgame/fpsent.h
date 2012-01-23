#ifndef _FPSGAME_FPSENTS_H
#define _FPSGAME_FPSENTS_H

#include "fpsstate.h"
#include "priviledges.h"
#include "ai.h"
#include "ents.h"

struct fpsent : dynent, fpsstate
{
    int weight;                         // affects the effectiveness of hitpush
    int clientnum, privilege, lastupdate, plag, ping;
    int lifesequence;                   // sequence id for each respawn, used in damage test
    int respawned, suicided;
    int lastpain;
    int lastaction, lastattackgun;
    bool attacking;
    int attacksound, attackchan, idlesound, idlechan;
    int lasttaunt;
    int lastpickup, lastpickupmillis, lastpickupindex, lastbase, lastrepammo, flagpickup, tokens;
    vec lastcollect;
    int frags, flags, deaths, totaldamage, totalshots;
    editinfo *edit;
    float deltayaw, deltapitch, newyaw, newpitch;
    int smoothmillis;

    string name, team, info;
    int playermodel;
    ai::aiinfo *ai;
    int ownernum, lastnode;

    vec muzzle;

    fpsent() : weight(100), clientnum(-1), privilege(PRIV_NONE), lastupdate(0), plag(0), ping(0), lifesequence(0), respawned(-1), suicided(-1), lastpain(0), attacksound(-1), attackchan(-1), idlesound(-1), idlechan(-1), frags(0), flags(0), deaths(0), totaldamage(0), totalshots(0), edit(NULL), smoothmillis(-1), playermodel(-1), ai(NULL), ownernum(-1), muzzle(-1, -1, -1)
    {
        name[0] = team[0] = info[0] = 0;
        respawn();
    }
    ~fpsent()
    {
        freeeditinfo(edit);
        if(attackchan >= 0) stopsound(attacksound, attackchan);
        if(idlechan >= 0) stopsound(idlesound, idlechan);
        if(ai) delete ai;
    }

    void hitpush(int damage, const vec &dir, fpsent *actor, int gun)
    {
        vec push(dir);
        push.mul(80*damage/weight);
        if(gun==GUN_RL || gun==GUN_GL) push.mul(actor==this ? 5 : (type==ENT_AI ? 3 : 2));
        vel.add(push);
    }

    void stopattacksound()
    {
        if(attackchan >= 0) stopsound(attacksound, attackchan, 250);
        attacksound = attackchan = -1;
    }

    void stopidlesound()
    {
        if(idlechan >= 0) stopsound(idlesound, idlechan, 100);
        idlesound = idlechan = -1;
    }

    void respawn(int gamemode = NULL)
    {
        dynent::reset();
        fpsstate::respawn(gamemode);
        respawned = suicided = -1;
        lastaction = 0;
        lastattackgun = gunselect;
        attacking = false;
        lasttaunt = 0;
        lastpickup = -1;
        lastpickupmillis = 0;
        lastbase = lastrepammo = -1;
        flagpickup = 0;
        tokens = 0;
        lastcollect = vec(-1e10f, -1e10f, -1e10f);
        stopattacksound();
        lastnode = -1;
    }

};

#endif
